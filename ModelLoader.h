#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <fstream>
#include <sstream>
#include <vector>
#include "Vector.h"
namespace ModelLoader
{
    void LoadFromObj(const char *path, std::vector<Vector<float>> &res)
    {
        printf("Loadfromobj\n");
        std::ifstream objFile;
        objFile.open(path);

        printf("open\n");
        for (std::string line; std::getline(objFile, line);)
        {
            printf("Getline\n");
            float x = 0;
            float y = 0;
            float z = 0;
            std::stringstream inStream(line);
            inStream >> x >> y >> z;
            //printf("Line: %s\n", line.c_str());
            //printf("Loaded: %f,%f,%f\n", x, y, z);
            res.push_back(Vector<float>(x, y, z));
        }

        objFile.close();
    }
};
#endif