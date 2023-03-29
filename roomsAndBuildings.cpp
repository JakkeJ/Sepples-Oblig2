#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Room {
    public:
    int width;
    int length;
    int area;
    bool isFlipped;
    Room (int widthInput, int lengthInput, bool isFlippedInput){
        width = widthInput;
        length = lengthInput;
        isFlipped = isFlippedInput;
        area = length * width;
    }
};

int main() {
    ifstream inputData("input_oblig2.txt");

    string line;
    string roomAndBuildingCountRaw;

    int lineCount;

    vector<string> fileDataRaw;
    vector<int> roomAndBuildingCountInfo;
    vector<vector<int>> roomsInfo;
    vector<Room> rasd;
    
    //Read data from the file and store it in a vector
    while(getline(inputData, line)) {
        fileDataRaw.push_back(line);
    }

    //First index is always the information about room and building count
    roomAndBuildingCountRaw = fileDataRaw[0];

    //Splitting the line into two integers, room count and building count
    stringstream X(roomAndBuildingCountRaw);
    while (getline(X, line, ' ')){
        roomAndBuildingCountInfo.push_back(stoi(line));
    }

    //Adding each room to a list of rooms for further processing
    for (int i = 1; i < roomAndBuildingCountInfo[0] + 1; ++i){
        std::stringstream Y(fileDataRaw[i]);
        vector<int> roomInfo;
        while (getline(Y, line, ' ')){
            roomInfo.push_back(stoi(line));
        }
        roomsInfo.push_back(roomInfo);
    }

    //Printing all room dimensions
    for (int i = 0; i < roomsInfo.size(); ++i){
        std::cout << "Room " << i + 1 << ": ";
        int area = 1;
        for (int j = 0; j < roomsInfo[i].size(); ++j){
            std::cout << roomsInfo[i][j];
            area *= roomsInfo[i][j];
            if (j < roomsInfo[i].size() - 1) {
                std::cout << " x ";
            }
        }
        std::cout << ", Area: " << area << std::endl;
    }
}