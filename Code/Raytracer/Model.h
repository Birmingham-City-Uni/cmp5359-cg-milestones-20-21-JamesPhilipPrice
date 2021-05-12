#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "geometry.h"

class Model {
public:
	Model(const char* _filename) {
		std::ifstream fileIn(_filename);

		if (!fileIn) {
			std::cerr << "Couldn't load file: " << _filename << std::endl;
		}

		std::vector<std::string> fileBuffer;

		while (!fileIn.eof()) {
			std::string s;
			getline(fileIn, s);

			s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

			fileBuffer.push_back(s);
		}
		fileIn.close();

		for (int i = 0; i < fileBuffer.size(); i++) {
			if (fileBuffer[i].substr(0, 2) == "v ") {
				//This line contains vertex information
				Vec3f tempVertBuffer(0, 0, 0);
				//Strip away the now redundant information and break down the buffer line into separate floats
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertBuffer.x = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertBuffer.y = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVertBuffer.z = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				vertices.push_back(tempVertBuffer);
			}
			else if (fileBuffer[i].substr(0, 2) == "vt") {
				Vec2f tempUVBuffer(0, 0);
				//Strip away the now redundant information and break down the buffer line into separate floats
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempUVBuffer.x = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempUVBuffer.y = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);

				UVData.push_back(tempUVBuffer);
			}
			else if (fileBuffer[i].substr(0, 2) == "vn") {
				Vec3f tempVNBuffer(0, 0, 0);
				//Strip away the now redundant information and break down the buffer line into separate floats
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVNBuffer.x = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVNBuffer.y = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				tempVNBuffer.z = (stof(fileBuffer[i].substr(0, fileBuffer[i].find(" "))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);

				normals.push_back(tempVNBuffer);
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
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointOne.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				//Get second vertex
				tempVertPointTwo.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointTwo.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointTwo.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find(" ") + 1);
				//Get third vertex
				tempVertPointThree.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointThree.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));
				fileBuffer[i].erase(0, fileBuffer[i].find("/") + 1);
				tempVertPointThree.push_back(stoi(fileBuffer[i].substr(0, fileBuffer[i].find("/"))));

				/*
				* Build the full face buffer  0-2: Vertex indexing, 3-5: UV indexing, 6-8: Normal indexing
				*/

				std::vector<int> fullFaceBuffer;
				fullFaceBuffer.push_back(tempVertPointOne[0]);
				fullFaceBuffer.push_back(tempVertPointTwo[0]);
				fullFaceBuffer.push_back(tempVertPointThree[0]);
				fullFaceBuffer.push_back(tempVertPointOne[1]);
				fullFaceBuffer.push_back(tempVertPointTwo[1]);
				fullFaceBuffer.push_back(tempVertPointThree[1]);
				fullFaceBuffer.push_back(tempVertPointOne[2]);
				fullFaceBuffer.push_back(tempVertPointTwo[2]);
				fullFaceBuffer.push_back(tempVertPointThree[2]);

				faceData.push_back(fullFaceBuffer);
			}
		}

		fileBuffer.clear();
	}

	~Model() {}

	std::vector<int> Face(int _idx) {
		return faceData[_idx];
	}

	Vec3f Vert(int _idx) {
		return vertices[_idx-1];
	}

	Vec2f UV(int _idx) {
		return UVData[_idx-1];
	}

	Vec3f Norm(int _idx) {
		return normals[_idx-1];
	}

	int nFaces() {
		return faceData.size();
	}

	int nVerts() {
		return vertices.size();
	}

private:
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec2f> UVData;
	std::vector<std::vector<int>> faceData;
};