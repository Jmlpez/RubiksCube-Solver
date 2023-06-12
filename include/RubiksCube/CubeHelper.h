#ifndef CUBE_HELPER_H
#define CUBE_HELPER_H

#include "./RubiksCube.h"
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

class CubeHelper
{
public:
    CubeHelper();
    vector<RubiksCube::MOVE> getScramble(int count);
    string convertMove(RubiksCube::MOVE idx);
    char colorHelper(RubiksCube::COLOR color);
    void scramble(RubiksCube &cube, int count);
    void moveSequence(RubiksCube &cube, vector<RubiksCube::MOVE> sequence);
    void showCubeConsole(RubiksCube &cube);
};

#endif