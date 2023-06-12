#ifndef SOLVER_H
#define SOLVER_H

#include "./RubiksCube.h"
#include "./CubeHelper.h"
#include <iostream>
#include <queue>
#include <map>
#include <vector>

using namespace std;

class Solver
{
    typedef vector<long long> state;
    enum PHASE : short
    {
        PHASE1,
        PHASE2,
        PHASE3,
        PHASE4
    };

    struct Node
    {
        RubiksCube cube;
        string path;
        RubiksCube::MOVE lastMove;
        Node(RubiksCube _cube, string _path, RubiksCube::MOVE _lastMove) : cube(_cube), path(_path), lastMove(_lastMove) {}
        Node() {}
    };

public:
    Solver();
    Solver(RubiksCube &cube);
    void bidirectionalBFS(Solver::PHASE phase);
    void solveCube();
    RubiksCube getCube() { return startNode.cube; }
    vector<RubiksCube::MOVE> getSolutionSteps() { return solutionMoves; }
    Node startNode;

private:
    state getPhaseId(PHASE phase, RubiksCube &cube);
    bool prune(RubiksCube::MOVE lastMove, RubiksCube::MOVE move);
    bool goal(Solver::PHASE phase, RubiksCube &cube);
    void removeMoves(PHASE phase);
    bool allowed[18];
    CubeHelper aux;
    vector<RubiksCube::MOVE> solutionMoves;
    int totalSteps;
};

#endif