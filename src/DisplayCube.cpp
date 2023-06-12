#include "../include/DisplayCube.h"

const string cubePath = "./resources/cube.obj";
const float degree = 90.0f;
const float defaultSpeed = 15.0f;
const int scrambleNumber = 100;
map<DisplayCube::AXIS, DisplayCube::AXIS> defaultMapAxis;

DisplayCube::DisplayCube()
{
    cubeModel.loadModel(cubePath);

    for (int k = 0; k < 3; k++)
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                // the j-i swap is because the order of the cubies on the imported model are swapped
                cubeMeshMat[j][i].push_back(&cubeModel.meshes[k * 9 + i * 3 + j]);

    speed = defaultSpeed;
    animCount = 0;
    shakecube = 0;
    animationEnd = degree / speed;
    counterClockwise = isAnimating = isRotatingAxis = false;

    mapMove[MOVE::L] = MOVE::L;
    mapMove[MOVE::F] = MOVE::F;
    mapMove[MOVE::U] = MOVE::U;
    mapMove[MOVE::B] = MOVE::B;
    mapMove[MOVE::D] = MOVE::D;
    mapMove[MOVE::R] = MOVE::R;

    defaultMapAxis[AXIS::X] = AXIS::X;
    defaultMapAxis[AXIS::Y] = AXIS::Y;
    defaultMapAxis[AXIS::Z] = AXIS::Z;
    defaultMapAxis[AXIS::XMINUS] = AXIS::XMINUS;
    defaultMapAxis[AXIS::YMINUS] = AXIS::YMINUS;
    defaultMapAxis[AXIS::ZMINUS] = AXIS::ZMINUS;

    mapAxis = defaultMapAxis;

    countMapping[AXIS::X] = countMapping[AXIS::Y] = countMapping[AXIS::Z] = 0;
}

void DisplayCube::Draw(Shader &shaderProgram)
{
    cubeModel.Draw(shaderProgram);
}

void DisplayCube::processInput(GLFWwindow *window)
{
    if (isAnimating || isMoving)
        return;
    for (int i = 0; i < 6; i++)
    {
        if (glfwGetKey(window, keyMoves[i]) == GLFW_PRESS)
        {
            if (!isMoving)
            {
                isMoving = true;
                switch (keyMoves[i])
                {
                case GLFW_KEY_F:
                    pressedMove = (!counterClockwise) ? mapMove[MOVE::F] : getInvertMove(mapMove[MOVE::F]);
                    moveAxis = mapAxis[AXIS::Z]; // update the mapAxis
                    break;
                case GLFW_KEY_B:
                    pressedMove = (!counterClockwise) ? mapMove[MOVE::B] : getInvertMove(mapMove[MOVE::B]);
                    moveAxis = mapAxis[AXIS::Z];
                    break;
                case GLFW_KEY_L:
                    pressedMove = (!counterClockwise) ? mapMove[MOVE::L] : getInvertMove(mapMove[MOVE::L]);
                    moveAxis = mapAxis[AXIS::X];
                    break;
                case GLFW_KEY_R:
                    pressedMove = (!counterClockwise) ? mapMove[MOVE::R] : getInvertMove(mapMove[MOVE::R]);
                    moveAxis = mapAxis[AXIS::X];
                    break;
                case GLFW_KEY_U:
                    pressedMove = (!counterClockwise) ? mapMove[MOVE::U] : getInvertMove(mapMove[MOVE::U]);
                    moveAxis = mapAxis[AXIS::Y];
                    break;
                case GLFW_KEY_D:
                    pressedMove = (!counterClockwise) ? mapMove[MOVE::D] : getInvertMove(mapMove[MOVE::D]);
                    moveAxis = mapAxis[AXIS::Y];
                    break;
                default:
                    break;
                }
                appliedMoves.push_back(pressedMove);
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        solve();
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        scramble(scrambleNumber);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        moveAxis = mapAxis[AXIS::X];
        rotatingAxis = AXIS::X;
        rotateAxisCube(moveAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        // rotate the Entire Cube
        moveAxis = mapAxis[AXIS::Y];
        rotatingAxis = AXIS::Y;
        rotateAxisCube(moveAxis);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        // rotate the Entire Cube
        moveAxis = mapAxis[AXIS::Z];
        rotatingAxis = AXIS::Z;
        rotateAxisCube(moveAxis);
    }

    counterClockwise = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
}

void DisplayCube::updateModel(glm::mat4 &model, Shader &shaderProgram)
{
    if (isRotatingAxis)
    {
        glm::vec3 axisVector = glm::vec3(1.0f, 0.0f, 0.0f);
        if (moveAxis == AXIS::Y || moveAxis == AXIS::YMINUS)
            axisVector = glm::vec3(0.0f, 1.0f, 0.0f);
        else if (moveAxis == AXIS::Z || moveAxis == AXIS::ZMINUS)
            axisVector = glm::vec3(0.0f, 0.0f, 1.0f);

        axisVector *= (moveAxis == AXIS::XMINUS || moveAxis == AXIS::YMINUS || moveAxis == AXIS::ZMINUS) ? -1 : 1;

        model = glm::rotate(model, glm::radians(speed), axisVector);
        shaderProgram.setMat4("model", model);
    }
    // model = glm::translate(model, glm::radians(glm::cos(shakecube)), glm::vec3(0.0f, 1.0f, 0.0f));
    // glm::translate()
}

void DisplayCube::handleMove()
{
    if (isAnimating)
    {
        pressedMove = waitList.front();
    }
    if (isMoving)
    {
        int layer = getLayer(pressedMove);

        // if is animating
        if (animCount < animationEnd)
            animCount++;
        else
        {

            isMoving = false;
            animCount = 0;

            if (isRotatingAxis)
            {
                isRotatingAxis = false;
                // updateMapMoves(moveAxis);
                return;
            }

            int count = ((pressedMove == MOVE::F || pressedMove == MOVE::U || pressedMove == MOVE::L) ||
                         (pressedMove == MOVE::RPRIME || pressedMove == MOVE::BPRIME || pressedMove == MOVE::DPRIME))
                            ? 1
                            : 3;

            for (int i = 0; i < count; i++)
                rotateMeshMat(layer, moveAxis);

            // if we are animating and the front move of the queue finish, then go to the next move
            if (isAnimating)
            {
                isMoving = true;
                waitList.pop();
                moveAxis = getAxis(waitList.front());
                if (waitList.empty())
                    resetValues();
            }
            return;
        }
        if (!isRotatingAxis)
            rotate(pressedMove, layer, moveAxis);
    }
}

void DisplayCube::rotateAxisCube(AXIS axis)
{
    isRotatingAxis = true;
    isMoving = true;
    updateMapMoves(rotatingAxis, 1);
    updateMapAxis(mapAxis, rotatingAxis, 1);
}

void DisplayCube::animateSequence(vector<MOVE> sequence, float animSpeed)
{
    speed = animSpeed;
    animationEnd = degree / speed;
    for (MOVE move : sequence)
    {
        bool doubleMove = (move == MOVE::B2 || move == MOVE::F2 || move == MOVE::L2 || move == MOVE::R2 || move == MOVE::U2 || move == MOVE::D2);
        bool primeMove = (move == MOVE::BPRIME || move == MOVE::FPRIME || move == MOVE::LPRIME || move == MOVE::RPRIME || move == MOVE::UPRIME || move == MOVE::DPRIME);

        if (move == MOVE::B2)
            move = mapMove[MOVE::B];
        else if (move == MOVE::F2)
            move = mapMove[MOVE::F];
        else if (move == MOVE::L2)
            move = mapMove[MOVE::L];
        else if (move == MOVE::R2)
            move = mapMove[MOVE::R];
        else if (move == MOVE::U2)
            move = mapMove[MOVE::U];
        else if (move == MOVE::D2)
            move = mapMove[MOVE::D];
        else
            move = (!primeMove) ? mapMove[move] : getInvertMove(mapMove[getInvertMove(move)]);

        if (doubleMove)
            waitList.push(move);
        waitList.push(move);
    }
    moveAxis = getAxis(waitList.front());
}

void DisplayCube::scramble(int count)
{
    scrambleMoves = aux.getScramble(count);

    for (auto move : scrambleMoves)
        appliedMoves.push_back(move);

    axisSeq = moveAxisBfs(mapAxis, defaultMapAxis);
    for (auto sq : axisSeq)
    {
        updateMapMoves(sq, 1);
        updateMapAxis(mapAxis, sq, 1);
    }

    animCount = 0;
    isMoving = true;
    isAnimating = true;
    animateSequence(scrambleMoves, degree);
}

void DisplayCube::solve()
{
    RubiksCube newCube;
    Solver S(newCube);

    for (auto &move : appliedMoves)
        newCube.move(move);

    if (newCube.isSolved())
        return;

    S.startNode.cube = newCube;
    S.solveCube();
    animCount = 0;
    isMoving = isAnimating = true;

    axisSeq = moveAxisBfs(mapAxis, defaultMapAxis);

    for (auto sq : axisSeq)
    {
        updateMapMoves(sq, 1);
        updateMapAxis(mapAxis, sq, 1);
    }
    vector<MOVE> solution = S.getSolutionSteps();

    animateSequence(solution, 9.0f);

    scrambleMoves.clear();
    appliedMoves.clear();
}

DisplayCube::AXIS DisplayCube::getAxis(MOVE move)
{
    if (move == MOVE::L || move == MOVE::R || move == MOVE::LPRIME || move == MOVE::RPRIME)
        return AXIS::X;
    if (move == MOVE::U || move == MOVE::D || move == MOVE::UPRIME || move == MOVE::DPRIME)
        return AXIS::Y;
    return AXIS::Z;
}
MOVE DisplayCube::getInvertMove(MOVE move)
{
    return RubiksCube::invert(move);
}

int DisplayCube::getLayer(MOVE move)
{
    int layer = 1;
    if (move == MOVE::L || move == MOVE::LPRIME ||
        move == MOVE::B || move == MOVE::BPRIME ||
        move == MOVE::D || move == MOVE::DPRIME)
        layer = 0;

    if (move == MOVE::R || move == MOVE::RPRIME ||
        move == MOVE::F || move == MOVE::FPRIME ||
        move == MOVE::U || move == MOVE::UPRIME)
        layer = 2;

    return layer;
}

void DisplayCube::resetValues()
{
    isMoving = isAnimating = false;
    speed = defaultSpeed;
    animationEnd = degree / speed;
    animCount = 0;

    for (int i = axisSeq.size() - 1; i >= 0; i--)
    {
        AXIS sq = axisSeq[i];
        updateMapMoves(sq, 3);
        updateMapAxis(mapAxis, sq, 3);
    }
    axisSeq.clear();
}

void DisplayCube::updateMapMoves(AXIS axis, int count)
{
    while (count--)
    {
        if (axis == AXIS::X || axis == AXIS::XMINUS)
        {
            swap(mapMove[MOVE::F], mapMove[MOVE::U]);
            swap(mapMove[MOVE::U], mapMove[MOVE::B]);
            swap(mapMove[MOVE::B], mapMove[MOVE::D]);
        }
        else if (axis == AXIS::Y || axis == AXIS::YMINUS)
        {
            swap(mapMove[MOVE::F], mapMove[MOVE::L]);
            swap(mapMove[MOVE::L], mapMove[MOVE::B]);
            swap(mapMove[MOVE::B], mapMove[MOVE::R]);
        }
        else // Z-axis
        {
            swap(mapMove[MOVE::U], mapMove[MOVE::R]);
            swap(mapMove[MOVE::R], mapMove[MOVE::D]);
            swap(mapMove[MOVE::D], mapMove[MOVE::L]);
        }
    }
}

void DisplayCube::updateMapAxis(map<AXIS, AXIS> &mAxis, AXIS axis, int count)
{
    while (count--)
    {
        if (axis == AXIS::X || axis == AXIS::XMINUS)
        {
            swap(mAxis[AXIS::Z], mAxis[AXIS::Y]);
            swap(mAxis[AXIS::Y], mAxis[AXIS::ZMINUS]);
            swap(mAxis[AXIS::ZMINUS], mAxis[AXIS::YMINUS]);
        }
        else if (axis == AXIS::Y || axis == AXIS::YMINUS)
        {
            swap(mAxis[AXIS::Z], mAxis[AXIS::XMINUS]);
            swap(mAxis[AXIS::XMINUS], mAxis[AXIS::ZMINUS]);
            swap(mAxis[AXIS::ZMINUS], mAxis[AXIS::X]);
        }
        else // Z-axis
        {
            swap(mAxis[AXIS::Y], mAxis[AXIS::X]);
            swap(mAxis[AXIS::X], mAxis[AXIS::YMINUS]);
            swap(mAxis[AXIS::YMINUS], mAxis[AXIS::XMINUS]);
        }
    }
}

vector<DisplayCube::AXIS> DisplayCube::moveAxisBfs(map<AXIS, AXIS> init, map<AXIS, AXIS> end)
{
    // they are on the same state
    if (init == end)
        return {};
    vector<AXIS> sol, moves{X, Y, Z};
    queue<map<AXIS, AXIS>> q;
    map<map<AXIS, AXIS>, AXIS> parent;
    parent[init] = AXIS::XMINUS; // this is like -1
    q.push(init);

    bool finish = false;
    while (!q.empty())
    {
        auto topMap = q.front();
        if (topMap == end)
        {
            finish = true;
            break;
        }
        q.pop();
        for (auto ax : moves)
        {
            updateMapAxis(topMap, ax, 1);
            if (parent.find(topMap) != parent.end())
                continue;
            parent[topMap] = ax;
            q.push(topMap);
            updateMapAxis(topMap, ax, 3);
        }
    }
    if (finish)
        while (end != init)
        {
            sol.insert(sol.begin(), parent[end]);
            updateMapAxis(end, parent[end], 3);
        }
    return sol;
}

void DisplayCube::rotate(MOVE move, int layer, AXIS axis)
{

    glm::vec3 axisVector;
    glm::vec3 axisX = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 axisY = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 axisZ = glm::vec3(0.0f, 0.0f, 1.0f);

    if (axis == AXIS::X || axis == AXIS::XMINUS)
        axisVector = axisX; // X-Axis
    else if (axis == AXIS::Y || axis == AXIS::YMINUS)
        axisVector = axisY; // Y-Axis
    else if (axis == AXIS::Z || axis == AXIS::ZMINUS)
        axisVector = axisZ; // Z-Axis

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int X = i, Y = j, Z = layer;
            if (axisVector == axisX)
                swap(Y, Z);
            else if (axisVector == axisZ)
                swap(X, Z);

            float nSpeed = speed;
            if (move == MOVE::F || move == MOVE::U || move == MOVE::R || move == MOVE::LPRIME || move == MOVE::BPRIME || move == MOVE::DPRIME)
                nSpeed = -nSpeed;
            for (auto &v : cubeMeshMat[X][Y][Z]->vertices)
            {
                v.position = glm::rotate(v.position, glm::radians(nSpeed), axisVector);
                v.normal = glm::rotate(v.normal, glm::radians(nSpeed), axisVector);
            }
            cubeMeshMat[X][Y][Z]->updateMesh();
        }
    }
}

void DisplayCube::rotateMeshMat(int layer, AXIS axis)
{
    if (axis == AXIS::XMINUS)
        axis = AXIS::X;
    else if (axis == AXIS::YMINUS)
        axis = AXIS::Y;
    else if (axis == AXIS::ZMINUS)
        axis = AXIS::Z;
    // [X] [Y] [Z]
    vector<Mesh *> copyCube[3][3];

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            copyCube[i][j] = cubeMeshMat[i][j];

    for (int i = 2; i >= 0; i--)
        for (int j = 0; j < 3; j++)
        {
            int X = i, Y = j, Z = layer;
            if (axis == AXIS::Z) // Eje Z
                cubeMeshMat[Z][X][2 - Y] = copyCube[Z][Y][X];
            else if (axis == AXIS::X) // Eje X
                cubeMeshMat[X][Z][2 - Y] = copyCube[Y][Z][X];
            else // axis == Axis::Y -> Eje Y default
                cubeMeshMat[Y][X][Z] = copyCube[2 - X][Y][Z];
        }
}
