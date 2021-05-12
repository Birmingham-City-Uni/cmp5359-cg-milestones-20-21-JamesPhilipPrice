#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "GeomData.h"

class GeomUtils {
public:
	GeomUtils() {}
	~GeomUtils() {}

	bool LoadOBJFile(const char* _location, std::vector<Vertex>* _targetVertexContainer) {
		std::ifstream fileIn(_location);

		if (!fileIn) {
			std::cerr << "Couldn't load file: " << _location << std::endl;
			return false;
		}

		std::vector<std::string> fileBuffer;

		while (!fileIn.eof()) {
			std::string s;
			getline(fileIn, s);

			s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

			fileBuffer.push_back(s);
		}
		fileIn.close();

		//Sort through the information in the obj file buffer and place items into buffer lists
		std::vector<std::vector<float>> vertexBuffer;
		std::vector<std::vector<float>> vertexUVBuffer;
		std::vector<std::vector<std::vector<int>>> faceBuffer;

		for (int i = 0; i < fileBuffer.size(); i++) {
			if (fileBuffer[i].substr(0, 2) == "v ") {
				//This line contains vertex information
				std::vector<float> tempVertBuffer;
				//Strip away the now redundant information and break down the buffer line into separate floats
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertBuffer.push_back(stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertBuffer.push_back(stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertBuffer.push_back(stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				vertexBuffer.push_back(tempVertBuffer);
			}
			else if (fileBuffer[i].substr(0, 2) == "vt") {
				std::vector<float> tempUVBuffer;
				//Strip away the now redundant information and break down the buffer line into separate floats
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempUVBuffer.push_back(stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempUVBuffer.push_back(stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);

				vertexUVBuffer.push_back(tempUVBuffer);
			}
			else if (fileBuffer[i].substr(0, 2) == "f ") {
				std::vector<std::vector<int>> tempFaceBuffer;
				std::vector <int> tempVertPointOne;
				std::vector <int> tempVertPointTwo;
				std::vector <int> tempVertPointThree;
				//Strip away now redundant information and sort through faces to generate a vertex order
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				//Get first vertex
				tempVertPointOne.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointOne.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertPointTwo.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointTwo.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertPointThree.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointThree.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				tempFaceBuffer.push_back(tempVertPointOne);
				tempFaceBuffer.push_back(tempVertPointTwo);
				tempFaceBuffer.push_back(tempVertPointThree);

				faceBuffer.push_back(tempFaceBuffer);
			}
		}

		for (int i = 0; i < faceBuffer.size(); i++) {
			float x, y, z, u, v;
			for (int j = 0; j < 3; j++) {
				x = vertexBuffer[faceBuffer[i][j][0] - 1][0];
				y = vertexBuffer[faceBuffer[i][j][0] - 1][1];
				z = vertexBuffer[faceBuffer[i][j][0] - 1][2];
				u = vertexUVBuffer[faceBuffer[i][j][1] - 1][0];
				v = vertexUVBuffer[faceBuffer[i][j][1] - 1][1];
				_targetVertexContainer->emplace_back(Vertex(x, y, z, u, v));
			}
		}

		fileBuffer.clear();
		return true;
	}
};
