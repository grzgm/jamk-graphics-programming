#include "misc.h"
#include <iostream>

void printMat(glm::mat4 mat)
{
    for (int i = 0; i < mat.length(); i++)
    {
        for (int j = 0; j < mat.length(); j++)
        {
            std::cout << mat[i][j];
        }
        std::cout << std::endl;
    }
}