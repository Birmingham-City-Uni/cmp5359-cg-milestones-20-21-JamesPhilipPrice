#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash;
            iss >> trash;
            Vec2f vt;
            for (int i = 0; i < 2; i++) iss >> vt[i]; //FIX vt import marching and indexing
            vts_.push_back(vt);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash;
            Vec3f vn;
            for (int i = 0; i < 3; i++) iss >> vn[i];
            std::cout << vn.x << " " << vn.y << " " << vn.z << std::endl;
            vns_.push_back(vn);
        }
        else if (!line.compare(0, 2, "f ")) { // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ... making assumption v1==vt1 etc.
            std::vector<int> f;
            int nidx, tidx, idx;
            iss >> trash;
            while (iss >> idx >> trash >> tidx >> trash >> nidx) {
                idx--; // in wavefront obj all indices start at 1, not zero
                tidx--;
                nidx--;
                f.push_back(idx);
                f.push_back(tidx);
                f.push_back(nidx);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2f Model::vt(int i) {
    return vts_[i];
}

Vec3f Model::vn(int i)
{
    return vns_[i];
}

