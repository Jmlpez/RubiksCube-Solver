#include "../../include/RubiksCube/CubeHelper.h"

typedef RubiksCube::MOVE MOVE;
typedef RubiksCube::FACE FACE;
typedef RubiksCube::COLOR COLOR;

CubeHelper::CubeHelper() {}

void CubeHelper::scramble(RubiksCube &cube, int count)
{
    MOVE moves[18] = {
        MOVE::L,
        MOVE::LPRIME,
        MOVE::L2,
        MOVE::R,
        MOVE::RPRIME,
        MOVE::R2,
        MOVE::U,
        MOVE::UPRIME,
        MOVE::U2,
        MOVE::D,
        MOVE::DPRIME,
        MOVE::D2,
        MOVE::F,
        MOVE::FPRIME,
        MOVE::F2,
        MOVE::B,
        MOVE::BPRIME,
        MOVE::B2,
    };

    srand(time(0));
    cout << "Scramble Cube:\n";
    for (int i = 0; i < count; i++)
    {
        int moveIdx = rand() % 18;
        cube.move(moves[moveIdx]);
        cout << convertMove(moves[moveIdx]) << " ";
        if (i % 10 == 0 && i != 0)
            cout << "\n";
    }
    cout << "\n";
}

vector<MOVE> CubeHelper::getScramble(int count)
{
    MOVE moves[18] = {
        MOVE::L,
        MOVE::LPRIME,
        MOVE::L2,
        MOVE::R,
        MOVE::RPRIME,
        MOVE::R2,
        MOVE::U,
        MOVE::UPRIME,
        MOVE::U2,
        MOVE::D,
        MOVE::DPRIME,
        MOVE::D2,
        MOVE::F,
        MOVE::FPRIME,
        MOVE::F2,
        MOVE::B,
        MOVE::BPRIME,
        MOVE::B2,
    };
    vector<MOVE> sol;
    srand(time(0));
    for (int i = 0; i < count; i++)
    {
        int moveIdx = rand() % 18;
        sol.push_back((moves[moveIdx]));
    }
    return sol;
}

void CubeHelper::moveSequence(RubiksCube &cube, vector<RubiksCube::MOVE> sequence)
{
    for (MOVE move : sequence)
        cube.move(move);
}

string CubeHelper::convertMove(MOVE idx)
{
    string move;
    switch (idx)
    {
    case MOVE::U:
        move = "U";
        break;
    case MOVE::U2:
        move = "U2";
        break;
    case MOVE::UPRIME:
        move = "U'";
        break;
    case MOVE::D:
        move = "D";
        break;
    case MOVE::D2:
        move = "D2";
        break;
    case MOVE::DPRIME:
        move = "D'";
        break;
    case MOVE::R:
        move = "R";
        break;
    case MOVE::R2:
        move = "R2";
        break;
    case MOVE::RPRIME:
        move = "R'";
        break;
    case MOVE::L:
        move = "L";
        break;
    case MOVE::L2:
        move = "L2";
        break;
    case MOVE::LPRIME:
        move = "L'";
        break;
    case MOVE::F:
        move = "F";
        break;
    case MOVE::F2:
        move = "F2";
        break;
    case MOVE::FPRIME:
        move = "F'";
        break;
    case MOVE::B:
        move = "B";
        break;
    case MOVE::B2:
        move = "B2";
        break;
    case MOVE::BPRIME:
        move = "B'";
        break;
    default:
        move = "";
    }
    return move;
}

char CubeHelper::colorHelper(COLOR color)
{
    typedef COLOR COLOR;

    char col;
    switch (color)
    {
    case COLOR::BLUE:
        col = 'B';
        break;
    case COLOR::GREEN:
        col = 'G';
        break;
    case COLOR::RED:
        col = 'R';
        break;
    case COLOR::ORANGE:
        col = 'O';
        break;
    case COLOR::WHITE:
        col = 'W';
        break;
    case COLOR::YELLOW:
        col = 'Y';
        break;
    }
    return col;
}

/*
    Helper to show the cube in console

    Output example:

   *          R R R
   *          R R R
   *          R R R
   *
   *  B B B   W W W   G G G   Y Y Y
   *  B B B   W W W   G G G   Y Y Y
   *  B B B   W W W   G G G   Y Y Y
   *
   *          O O O
   *          O O O
   *          O O O

*/
void CubeHelper::showCubeConsole(RubiksCube &cube)
{
    string up, middle, bottom;
    // RED
    for (int i = 0; i < 3; i++)
    {
        up.append(11, ' ');
        for (int j = 0; j < 3; j++)
        {
            COLOR c = cube.getFaceColor(FACE::UP, i, j);
            char color = colorHelper(c);
            up += color;
            up += ' ';
        }
        up += "\n";
    }

    // BLUE WHITE GREEN YELLOW
    FACE faces[4] = {FACE::LEFT, FACE::FRONT, FACE::RIGHT, FACE::BACK};
    for (int k = 0; k < 3; k++)
    {
        for (int i = 0; i < 4; i++)
        {
            middle.append(3, ' ');
            for (int j = 0; j < 3; j++)
            {
                COLOR c = cube.getFaceColor(faces[i], k, j);
                char color = colorHelper(c);
                middle += color;
                middle += (j < 2) ? " " : "";
            }
        }
        middle += "\n";
    }

    // ORANGE
    for (int i = 0; i < 3; i++)
    {
        bottom.append(11, ' ');
        for (int j = 0; j < 3; j++)
        {
            COLOR c = cube.getFaceColor(FACE::DOWN, i, j);
            char color = colorHelper(c);
            bottom += color;
            bottom += ' ';
        }
        bottom += "\n";
    }
    cout << "\n"
         << up << "\n"
         << middle << "\n"
         << bottom << "\n";
}