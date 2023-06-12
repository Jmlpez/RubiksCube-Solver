#ifndef DISPLAY_CUBE_CLASS_H
#define DISPLAY_CUBE_CLASS_H

#include <string>
#include <vector>
#include <queue>
#include <map>

#include "./Model.h"
#include "./RubiksCube/CubeHelper.h"
#include "./RubiksCube/Solver.h"
#include "./RubiksCube/RubiksCube.h"

using namespace std;

typedef RubiksCube::MOVE MOVE;

class DisplayCube
{
public:
    enum AXIS : short
    {
        X,
        XMINUS,
        Y,
        YMINUS,
        Z,
        ZMINUS
    };
    DisplayCube();
    void handleMove();
    void Draw(Shader &shaderProgram);
    void updateModel(glm::mat4 &model, Shader &shaderProgram);
    void processInput(GLFWwindow *window);
    void rotate(MOVE move, int layer, AXIS moveAxis);
    void rotateMeshMat(int layer, AXIS axis);
    void animateSequence(vector<MOVE> sequence, float animSpeed);
    void scramble(int count);
    void resetValues();
    void solve();

private:
    AXIS getAxis(MOVE move);
    MOVE getInvertMove(MOVE move);
    int getLayer(MOVE move);

    vector<AXIS> moveAxisBfs(map<AXIS, AXIS> init, map<AXIS, AXIS> end);
    void rotateAxisCube(AXIS axis);
    void updateMapMoves(AXIS axis, int count);
    void updateMapAxis(map<AXIS, AXIS> &mAxis, AXIS axis, int count);
    // void rotateEntireCube(AXIS axis);

    Model cubeModel;
    vector<Mesh *> cubeMeshMat[3][3];
    vector<MOVE> appliedMoves, scrambleMoves;
    queue<MOVE> waitList;

    // MOVE mapMove[6] = {};
    map<MOVE, MOVE> mapMove;
    map<AXIS, AXIS> mapAxis;
    map<AXIS, int> countMapping;

    vector<AXIS> axisSeq;

    // RubiksCube cube;
    CubeHelper aux;

    // MOVES
    short keyMoves[6] = {GLFW_KEY_F, GLFW_KEY_B, GLFW_KEY_L, GLFW_KEY_R, GLFW_KEY_U, GLFW_KEY_D};

    MOVE pressedMove;
    AXIS moveAxis, rotatingAxis;
    bool counterClockwise;
    bool isMoving;
    bool isAnimating;
    bool isRotatingAxis;
    float speed;
    int animCount;
    int animationEnd;
    int shakecube;
};

#endif