#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

class Room {
    public:
    int id;
    int width;
    int length;
    int area;
    bool isFlipped;
    bool isPlaced = false;
    Room (int idInput, int widthInput, int lengthInput, bool isFlippedInput){
        id = idInput;
        width = widthInput;
        length = lengthInput;
        isFlipped = isFlippedInput;
        area = length * width;
    }

    void flipRoom(){
        int temp = width;
        width = length;
        length = temp;
        isFlipped = !isFlipped;
    }
};

class Building {
public:
    int width;
    int length;
    int area;
    int id;
    int areaUsed;
    vector<vector<int>> grid;
    vector<Room> rooms;

    Building (int idInput, int widthInput, int lengthInput){
        id = idInput;
        width = widthInput;
        length = lengthInput;
        area = length * width;
        areaUsed = 0;
        grid.resize(length, vector<int>(width, 0));
    }

    // Sjekker om det kan plasseres et rom der det ønskes.
    // Bruker referanse til originalt romobjekt
    bool canPlaceRoomAtPosition(Room& room, int row, int col) {
        for (int i = 0; i < room.length; ++i) {
            for (int j = 0; j < room.width; ++j) {
                if (row + i >= length || col + j >= width || grid[row + i][col + j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }

    // Plasserer rom ut i bygningene der det er plass
    void placeRoomAtPosition(Room& room, int row, int col) {
        for (int i = 0; i < room.length; ++i) {
            for (int j = 0; j < room.width; ++j) {
                grid[row + i][col + j] = room.id;
            }
        }
    }

    // Sjekker om det kan plasseres et rom, så plasseres det ut med metoden over.
    bool placeRoom(Room& room) {
        for (int i = 0; i < length; ++i) {
            for (int j = 0; j < width; ++j) {
                if (canPlaceRoomAtPosition(room, i, j)) {
                    placeRoomAtPosition(room, i, j);
                    rooms.push_back(room);
                    areaUsed = areaUsed + room.area;
                    return true;
                }
                else {
                    room.flipRoom();
                    if (canPlaceRoomAtPosition(room, i, j)) {
                        placeRoomAtPosition(room, i, j);
                        rooms.push_back(room);
                        areaUsed = areaUsed + room.area;
                        return true;
                    }
                }
            }
        }
        return false;
    }

    Room& getRoomByIndex(int index){
        return rooms[index];
    }
};

int main() {
    ifstream inputData("input_oblig2.txt");
    string line;
    vector<int> roomAndBuildingCountInfo;
    vector<Room> roomObjectsList;
    vector<Building> buildingObjectsList;
    vector<Room> unusedRooms;

    // Lese data fra filen og legge rom og bygningsantall i en vector
    getline(inputData, line);
    stringstream X(line);
    while (getline(X, line, ' ')){
        roomAndBuildingCountInfo.push_back(stoi(line));
    }

    // Romdata - bruker emplace for å legge til allerede eksisterende data,
    // slipper dermed å lage variabler i for-løkka, siden det er gitt at
    // roomObjectsList-vektoren inneholder Room-objekter.
    for (int i = 0; i < roomAndBuildingCountInfo[0]; ++i){
        getline(inputData, line);
        stringstream Y(line);
        int width, length;
        Y >> width >> length;
        roomObjectsList.emplace_back(i + 1, width, length, false);
    }

    // Bygningsdata - samme som romdatabehandlingen over,
    // men den legger alltid et rom i hver bygning først.
    for (int i = 0; i < roomAndBuildingCountInfo[1]; ++i){
        getline(inputData, line);
        stringstream Z(line);
        int width, length;
        Z >> width >> length;
        buildingObjectsList.emplace_back(i + 1, width, length);
        buildingObjectsList[i].placeRoom(roomObjectsList[i]);
        roomObjectsList[i].isPlaced = true;
    }

    // Bruker referanse til bygningen og rommet og itererer over vectorene.
    for (Building& building : buildingObjectsList) {
        for (Room& room : roomObjectsList) {
            if (!room.isPlaced) {
                if (building.placeRoom(room)) {
                    room.isPlaced = true;
                }
            }
        }
    }

    // Legg rom som ikke fikk plass i en egen liste.
    for (Room& room : roomObjectsList){
        if (!room.isPlaced){
            unusedRooms.push_back(room);
        }
    }

    roomObjectsList.clear();

    // Output til fil
    ofstream outputFile("output_oblig2.txt");
    for (int i = 0; i < buildingObjectsList.size(); ++i){
        outputFile << "Bygning " << i + 1 << ":" << "\n"
                   << "  Dimensjoner: " << buildingObjectsList[i].width << " x " << buildingObjectsList[i].length << "\n"
                   << "  Areal: " << buildingObjectsList[i].area << "\n"
                   << "  Gjenstående areal: " << buildingObjectsList[i].area - buildingObjectsList[i].areaUsed << "\n"
                   << "  Brukt areal: " << buildingObjectsList[i].areaUsed << "\n";

        for (int j = 0; j < buildingObjectsList[i].rooms.size(); ++j){
            outputFile << "  Rom " << buildingObjectsList[i].rooms[j].id << ": \n"
                       << "    Dimensjoner: " << buildingObjectsList[i].rooms[j].width << " x " << buildingObjectsList[i].rooms[j].length << "\n"
                       << "    Areal: " << buildingObjectsList[i].rooms[j].area << "\n";
        }

        // Husk at setw er padding på tegning av bygning!
        outputFile << "  Oversiktsbilde av bygning " << i + 1 << ":" <<endl;
        for (int row = 0; row < buildingObjectsList[i].length; ++row) {
            outputFile << "    ";
            for (int col = 0; col < buildingObjectsList[i].width; ++col) {
                outputFile << setw (2) << buildingObjectsList[i].grid[row][col] << " ";
            }
            outputFile << "\n";
        }
        outputFile << "\n";
    }

    outputFile << "Rom som ikke kunne plasseres:\n";
    for (int i = 0; i < unusedRooms.size(); ++i){
        outputFile << "Rom " << unusedRooms[i].id << "\n"
                   << "  Dimensjoner: " << unusedRooms[i].length << " x " << unusedRooms[i].width << "\n";
    }
    outputFile.close();
}