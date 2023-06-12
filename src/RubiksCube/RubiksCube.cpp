#include "../../include/RubiksCube/RubiksCube.h"
#include <iostream>

RubiksCube::RubiksCube()
{
    for (int i = 0; i < 12; i++)
    {
        edges[i].index = i;
        edges[i].orientation = 0;
        setEdgesColor((EDGE)i);
    }
    for (int i = 0; i < 8; i++)
    {
        corners[i].index = i;
        corners[i].orientation = 0;
        setCornersColor((CORNER)i);
    }

    centers[FACE::UP] = RED;
    centers[FACE::LEFT] = BLUE;
    centers[FACE::FRONT] = WHITE;
    centers[FACE::RIGHT] = GREEN;
    centers[FACE::BACK] = YELLOW;
    centers[FACE::DOWN] = ORANGE;
    // path = "";
}

// Copy constructor...
RubiksCube::RubiksCube(const RubiksCube &cube)
{
    for (int i = 0; i < 12; i++)
    {
        edges[i].index = cube.edges[i].index;
        edges[i].orientation = cube.edges[i].orientation;
        edges[i].color = cube.edges[i].color;
    }
    for (int i = 0; i < 8; i++)
    {
        corners[i].index = cube.corners[i].index;
        corners[i].orientation = cube.corners[i].orientation;
        corners[i].color = cube.corners[i].color;
    }
    for (int i = 0; i < 6; i++)
        centers[i] = cube.centers[i];
    // // path = cube.path;
    // return *this;
}

RubiksCube &RubiksCube::operator=(const RubiksCube &cube)
{
    for (int i = 0; i < 12; i++)
    {
        edges[i].index = cube.edges[i].index;
        edges[i].orientation = cube.edges[i].orientation;
        edges[i].color = cube.edges[i].color;
    }
    for (int i = 0; i < 8; i++)
    {
        corners[i].index = cube.corners[i].index;
        corners[i].orientation = cube.corners[i].orientation;
        corners[i].color = cube.corners[i].color;
    }
    for (int i = 0; i < 6; i++)
        centers[i] = cube.centers[i];
    // path = cube.path;
    return *this;
}

bool RubiksCube::isSolved()
{
    for (int i = 0; i < 12; i++)
        if (edges[i].index != i || edges[i].orientation != 0)
            return false;
    for (int i = 0; i < 8; i++)
        if (corners[i].index != i || corners[i].orientation != 0)
            return false;

    // it's solved
    return true;
}

void RubiksCube::setEdgesColor(EDGE ind)
{
    Cubie &edge = edges[(short)ind];

    switch (edge.index)
    {
    case EDGE::UF:
        edge.color.Y = COLOR::RED;
        edge.color.Z = COLOR::WHITE;
        break;
    case EDGE::UR:
        edge.color.Y = COLOR::RED;
        edge.color.X = COLOR::GREEN;
    case EDGE::UB:
        edge.color.Y = COLOR::RED;
        edge.color.Z = COLOR::YELLOW;
        break;
    case EDGE::UL:
        edge.color.Y = COLOR::RED;
        edge.color.X = COLOR::BLUE;
        break;
    case EDGE::LF:
        edge.color.X = COLOR::BLUE;
        edge.color.Z = COLOR::WHITE;
        break;
    case EDGE::RF:
        edge.color.X = COLOR::GREEN;
        edge.color.Z = COLOR::WHITE;
        break;
    case EDGE::LB:
        edge.color.X = COLOR::BLUE;
        edge.color.Z = COLOR::YELLOW;
        break;
    case EDGE::RB:
        edge.color.X = COLOR::GREEN;
        edge.color.Z = COLOR::YELLOW;
        break;
    case EDGE::DF:
        edge.color.Y = COLOR::ORANGE;
        edge.color.Z = COLOR::WHITE;
        // cout << edge.color.Y << "*\n";
        break;
    case EDGE::DR:
        edge.color.Y = COLOR::ORANGE;
        edge.color.X = COLOR::GREEN;
        break;
    case EDGE::DB:
        edge.color.Y = COLOR::ORANGE;
        edge.color.Z = COLOR::YELLOW;
        break;
    case EDGE::DL:
        edge.color.Y = COLOR::ORANGE;
        edge.color.X = COLOR::BLUE;
        break;
    default:
        // Imposibble
        break;
    }
}

void RubiksCube::setCornersColor(CORNER ind)
{
    Cubie &corner = corners[(short)ind];
    switch (corner.index)
    {
    case CORNER::ULF:
        corner.color.Y = COLOR::RED;
        corner.color.X = COLOR::BLUE;
        corner.color.Z = COLOR::WHITE;
        break;
    case CORNER::URF:
        corner.color.Y = COLOR::RED;
        corner.color.X = COLOR::GREEN;
        corner.color.Z = COLOR::WHITE;
        break;
    case CORNER::URB:
        corner.color.Y = COLOR::RED;
        corner.color.X = COLOR::GREEN;
        corner.color.Z = COLOR::YELLOW;
        break;
    case CORNER::ULB:
        corner.color.Y = COLOR::RED;
        corner.color.X = COLOR::BLUE;
        corner.color.Z = COLOR::YELLOW;
        break;
    case CORNER::DLF:
        corner.color.Y = COLOR::ORANGE;
        corner.color.X = COLOR::BLUE;
        corner.color.Z = COLOR::WHITE;
        break;
    case CORNER::DRF:
        corner.color.Y = COLOR::ORANGE;
        corner.color.X = COLOR::GREEN;
        corner.color.Z = COLOR::WHITE;
        break;
    case CORNER::DRB:
        corner.color.Y = COLOR::ORANGE;
        corner.color.X = COLOR::GREEN;
        corner.color.Z = COLOR::YELLOW;
        break;
    case CORNER::DLB:
        corner.color.Y = COLOR::ORANGE;
        corner.color.X = COLOR::BLUE;
        corner.color.Z = COLOR::YELLOW;
        break;

    default:
        // Impossible
        break;
    }
}

RubiksCube::cubieCol RubiksCube::getEdgeColor(EDGE ind)
{
    return edges[ind].color;
}

RubiksCube::cubieCol RubiksCube::getCornerColor(CORNER ind)
{
    return corners[ind].color;
}

short RubiksCube::getEdgeIndex(EDGE ind)
{
    return edges[ind].index;
}
short RubiksCube::getCornerIndex(CORNER ind)
{
    return corners[ind].index;
}

short RubiksCube::getEdgeOrientation(EDGE ind)
{
    return edges[ind].orientation;
}
short RubiksCube::getCornerOrientation(CORNER ind)
{
    return corners[ind].orientation;
}

/*
 Get the color at position (row,col) on Face F.
 The row, col initial is placed liked if the Face were on Front.
*/
RubiksCube::COLOR RubiksCube::getFaceColor(RubiksCube::FACE F, int row, int col)
{
    if (row == 1 && col == 1)
    {
        return centers[(short)F];
    }
    // TODO
    if (F == FACE::UP)
    {
        if (row == 0)
        {
            if (col == 0)
                return getCornerColor(CORNER::ULB).Y;
            if (col == 1)
                return getEdgeColor(EDGE::UB).Y;
            if (col == 2)
                return getCornerColor(CORNER::URB).Y;
        }
        if (row == 1)
        {
            if (col == 0)
                return getEdgeColor(EDGE::UL).Y;
            if (col == 2)
                return getEdgeColor(EDGE::UR).Y;
        }
        if (row == 2)
        {
            if (col == 0)
                return getCornerColor(CORNER::ULF).Y;
            if (col == 1)
                return getEdgeColor(EDGE::UF).Y;
            if (col == 2)
                return getCornerColor(CORNER::URF).Y;
        }
    }
    if (F == FACE::FRONT)
    {
        if (row == 0)
        {
            if (col == 0)
                return getCornerColor(CORNER::ULF).Z;
            if (col == 1)
                return getEdgeColor(EDGE::UF).Z;
            if (col == 2)
                return getCornerColor(CORNER::URF).Z;
        }
        if (row == 1)
        {
            if (col == 0)
                return getEdgeColor(EDGE::LF).Z;
            if (col == 2)
                return getEdgeColor(EDGE::RF).Z;
        }
        if (row == 2)
        {
            if (col == 0)
                return getCornerColor(CORNER::DLF).Z;
            if (col == 1)
                return getEdgeColor(EDGE::DF).Z;
            if (col == 2)
                return getCornerColor(CORNER::DRF).Z;
        }
    }
    if (F == FACE::RIGHT)
    {
        if (row == 0)
        {
            if (col == 0)
                return getCornerColor(CORNER::URF).X;
            if (col == 1)
                return getEdgeColor(EDGE::UR).X;
            if (col == 2)
                return getCornerColor(CORNER::URB).X;
        }
        if (row == 1)
        {
            if (col == 0)
                return getEdgeColor(EDGE::RF).X;
            if (col == 2)
                return getEdgeColor(EDGE::RB).X;
        }
        if (row == 2)
        {
            if (col == 0)
                return getCornerColor(CORNER::DRF).X;
            if (col == 1)
                return getEdgeColor(EDGE::DR).X;
            if (col == 2)
                return getCornerColor(CORNER::DRB).X;
        }
    }
    if (F == FACE::DOWN)
    {
        if (row == 0)
        {
            if (col == 0)
                return getCornerColor(CORNER::DLF).Y;
            if (col == 1)
                return getEdgeColor(EDGE::DF).Y;
            if (col == 2)
                return getCornerColor(CORNER::DRF).Y;
        }
        if (row == 1)
        {
            if (col == 0)
                return getEdgeColor(EDGE::DL).Y;
            if (col == 2)
                return getEdgeColor(EDGE::DR).Y;
        }
        if (row == 2)
        {
            if (col == 0)
                return getCornerColor(CORNER::DLB).Y;
            if (col == 1)
                return getEdgeColor(EDGE::DB).Y;
            if (col == 2)
                return getCornerColor(CORNER::DRB).Y;
        }
    }
    if (F == FACE::BACK)
    {
        if (row == 0)
        {
            if (col == 0)
                return getCornerColor(CORNER::URB).Z;
            if (col == 1)
                return getEdgeColor(EDGE::UB).Z;
            if (col == 2)
                return getCornerColor(CORNER::ULB).Z;
        }
        if (row == 1)
        {
            if (col == 0)
                return getEdgeColor(EDGE::RB).Z;
            if (col == 2)
                return getEdgeColor(EDGE::LB).Z;
        }
        if (row == 2)
        {
            if (col == 0)
                return getCornerColor(CORNER::DRB).Z;
            if (col == 1)
                return getEdgeColor(EDGE::DB).Z;
            if (col == 2)
                return getCornerColor(CORNER::DLB).Z;
        }
    }
    if (F == FACE::LEFT)
    {
        if (row == 0)
        {
            if (col == 0)
                return getCornerColor(CORNER::ULB).X;
            if (col == 1)
                return getEdgeColor(EDGE::UL).X;
            if (col == 2)
                return getCornerColor(CORNER::ULF).X;
        }
        if (row == 1)
        {
            if (col == 0)
                return getEdgeColor(EDGE::LB).X;
            if (col == 2)
                return getEdgeColor(EDGE::LF).X;
        }
        if (row == 2)
        {
            if (col == 0)
                return getCornerColor(CORNER::DLB).X;
            if (col == 1)
                return getEdgeColor(EDGE::DL).X;
            if (col == 2)
                return getCornerColor(CORNER::DLF).X;
        }
    }
    return COLOR::RED; // a default value for the warning
}

void RubiksCube::updateEdgeOrientation(EDGE ind)
{
    edges[ind].orientation = 1 - edges[ind].orientation;
}

void RubiksCube::updateCornerOrienation(CORNER ind, int cant)
{
    corners[ind].orientation += cant;
    corners[ind].orientation %= 3;
}

void RubiksCube::move(MOVE idx)
{
    switch (idx)
    {
    case MOVE::U:
        rotU(1);
        break;
    case MOVE::U2:
        rotU(2);
        break;
    case MOVE::UPRIME:
        rotU(3);
        break;
    case MOVE::D:
        rotD(1);
        break;
    case MOVE::D2:
        rotD(2);
        break;
    case MOVE::DPRIME:
        rotD(3);
        break;
    case MOVE::R:
        rotR(1);
        break;
    case MOVE::R2:
        rotR(2);
        break;
    case MOVE::RPRIME:
        rotR(3);
        break;
    case MOVE::L:
        rotL(1);
        break;
    case MOVE::L2:
        rotL(2);
        break;
    case MOVE::LPRIME:
        rotL(3);
        break;
    case MOVE::F:
        rotF(1);
        break;
    case MOVE::F2:
        rotF(2);
        break;
    case MOVE::FPRIME:
        rotF(3);
        break;
    case MOVE::B:
        rotB(1);
        break;
    case MOVE::B2:
        rotB(2);
        break;
    case MOVE::BPRIME:
        rotB(3);
        break;

    default:
        break;
    }
}

RubiksCube::MOVE RubiksCube::invert(MOVE idx)
{
    switch (idx)
    {
    case MOVE::U:
        return MOVE::UPRIME;
        break;
    case MOVE::UPRIME:
        return MOVE::U;
        break;
    case MOVE::D:
        return MOVE::DPRIME;
        break;
    case MOVE::DPRIME:
        return MOVE::D;
        break;
    case MOVE::R:
        return MOVE::RPRIME;
        break;
    case MOVE::RPRIME:
        return MOVE::R;
        break;
    case MOVE::L:
        return MOVE::LPRIME;
        break;
    case MOVE::LPRIME:
        return MOVE::L;
        break;
    case MOVE::F:
        return MOVE::FPRIME;
        break;
    case MOVE::FPRIME:
        return MOVE::F;
        break;
    case MOVE::B:
        return MOVE::BPRIME;
        break;
    case MOVE::BPRIME:
        return MOVE::B;
        break;
    }
    return idx; // U2, B2, F2, L2, D2, R2 are there own inverse move
}

void RubiksCube::rotU(short cant)
{
    for (int i = 0; i < cant; i++)
    {
        // Update Edges Indexes
        Cubie tmp = edges[EDGE::UF];
        edges[EDGE::UF] = edges[EDGE::UR];
        edges[EDGE::UR] = edges[EDGE::UB];
        edges[EDGE::UB] = edges[EDGE::UL];
        edges[EDGE::UL] = tmp;

        // Update Corner Indexes
        tmp = corners[CORNER::URF];
        corners[CORNER::URF] = corners[CORNER::URB];
        corners[CORNER::URB] = corners[CORNER::ULB];
        corners[CORNER::ULB] = corners[CORNER::ULF];
        corners[CORNER::ULF] = tmp;

        // Update Edges Colors
        swap(edges[EDGE::UF].color.X, edges[EDGE::UF].color.Z);
        swap(edges[EDGE::UR].color.X, edges[EDGE::UR].color.Z);
        swap(edges[EDGE::UB].color.X, edges[EDGE::UB].color.Z);
        swap(edges[EDGE::UL].color.X, edges[EDGE::UL].color.Z);

        // Update Corner Colors
        swap(corners[CORNER::URF].color.X, corners[CORNER::URF].color.Z);
        swap(corners[CORNER::URB].color.X, corners[CORNER::URB].color.Z);
        swap(corners[CORNER::ULB].color.X, corners[CORNER::ULB].color.Z);
        swap(corners[CORNER::ULF].color.X, corners[CORNER::ULF].color.Z);

        // Edges/Corners Orientation does not change when U/D moves are performed
    }
}

void RubiksCube::rotF(short cant)
{
    for (int i = 0; i < cant; i++)
    {
        // Update Edges Indexes
        Cubie tmp = edges[EDGE::UF];
        edges[EDGE::UF] = edges[EDGE::LF];
        edges[EDGE::LF] = edges[EDGE::DF];
        edges[EDGE::DF] = edges[EDGE::RF];
        edges[EDGE::RF] = tmp;

        // Update Corner Indexes
        tmp = corners[CORNER::URF];
        corners[CORNER::URF] = corners[CORNER::ULF];
        corners[CORNER::ULF] = corners[CORNER::DLF];
        corners[CORNER::DLF] = corners[CORNER::DRF];
        corners[CORNER::DRF] = tmp;

        // Update Edges Colors
        swap(edges[EDGE::UF].color.X, edges[EDGE::UF].color.Y);
        swap(edges[EDGE::LF].color.X, edges[EDGE::LF].color.Y);
        swap(edges[EDGE::DF].color.X, edges[EDGE::DF].color.Y);
        swap(edges[EDGE::RF].color.X, edges[EDGE::RF].color.Y);

        // Update Corner Colors
        swap(corners[CORNER::URF].color.X, corners[CORNER::URF].color.Y);
        swap(corners[CORNER::ULF].color.X, corners[CORNER::ULF].color.Y);
        swap(corners[CORNER::DLF].color.X, corners[CORNER::DLF].color.Y);
        swap(corners[CORNER::DRF].color.X, corners[CORNER::DRF].color.Y);

        // Update Edges Orientation
        updateEdgeOrientation(EDGE::UF);
        updateEdgeOrientation(EDGE::LF);
        updateEdgeOrientation(EDGE::DF);
        updateEdgeOrientation(EDGE::RF);

        // Update Corners Orientation
        updateCornerOrienation(CORNER::URF, 1);
        updateCornerOrienation(CORNER::ULF, 2);
        updateCornerOrienation(CORNER::DLF, 1);
        updateCornerOrienation(CORNER::DRF, 2);
    }
}

void RubiksCube::rotL(short cant)
{
    for (int i = 0; i < cant; i++)
    {
        // Update Edges Indexes
        Cubie tmp = edges[EDGE::LF];
        edges[EDGE::LF] = edges[EDGE::UL];
        edges[EDGE::UL] = edges[EDGE::LB];
        edges[EDGE::LB] = edges[EDGE::DL];
        edges[EDGE::DL] = tmp;

        // Update Corner Indexes
        tmp = corners[CORNER::ULF];
        corners[CORNER::ULF] = corners[CORNER::ULB];
        corners[CORNER::ULB] = corners[CORNER::DLB];
        corners[CORNER::DLB] = corners[CORNER::DLF];
        corners[CORNER::DLF] = tmp;

        // Update Edges Colors
        swap(edges[EDGE::LF].color.Y, edges[EDGE::LF].color.Z);
        swap(edges[EDGE::UL].color.Y, edges[EDGE::UL].color.Z);
        swap(edges[EDGE::LB].color.Y, edges[EDGE::LB].color.Z);
        swap(edges[EDGE::DL].color.Y, edges[EDGE::DL].color.Z);

        // Update Corners Colors
        swap(corners[CORNER::ULF].color.Y, corners[CORNER::ULF].color.Z);
        swap(corners[CORNER::ULB].color.Y, corners[CORNER::ULB].color.Z);
        swap(corners[CORNER::DLB].color.Y, corners[CORNER::DLB].color.Z);
        swap(corners[CORNER::DLF].color.Y, corners[CORNER::DLF].color.Z);

        // Update Corners Orientations
        updateCornerOrienation(CORNER::ULF, 1);
        updateCornerOrienation(CORNER::ULB, 2);
        updateCornerOrienation(CORNER::DLB, 1);
        updateCornerOrienation(CORNER::DLF, 2);

        // Edges orientations only changes when F/B moves are performed
    }
}

void RubiksCube::rotD(short cant)
{
    for (int i = 0; i < cant; i++)
    {
        // Update Edges Indexes
        Cubie tmp = edges[EDGE::DF];
        edges[EDGE::DF] = edges[EDGE::DL];
        edges[EDGE::DL] = edges[EDGE::DB];
        edges[EDGE::DB] = edges[EDGE::DR];
        edges[EDGE::DR] = tmp;

        // Update Corner Indexes
        tmp = corners[CORNER::DRF];
        corners[CORNER::DRF] = corners[CORNER::DLF];
        corners[CORNER::DLF] = corners[CORNER::DLB];
        corners[CORNER::DLB] = corners[CORNER::DRB];
        corners[CORNER::DRB] = tmp;

        // Update Edges Colors
        swap(edges[EDGE::DF].color.X, edges[EDGE::DF].color.Z);
        swap(edges[EDGE::DR].color.X, edges[EDGE::DR].color.Z);
        swap(edges[EDGE::DB].color.X, edges[EDGE::DB].color.Z);
        swap(edges[EDGE::DL].color.X, edges[EDGE::DL].color.Z);

        // Update Corner Colors
        swap(corners[CORNER::DRF].color.X, corners[CORNER::DRF].color.Z);
        swap(corners[CORNER::DRB].color.X, corners[CORNER::DRB].color.Z);
        swap(corners[CORNER::DLB].color.X, corners[CORNER::DLB].color.Z);
        swap(corners[CORNER::DLF].color.X, corners[CORNER::DLF].color.Z);

        // Corners Orientation does not change when U/D moves are performed
        // Edges orientations only changes when F/B moves are performed
    }
}

void RubiksCube::rotB(short cant)
{
    for (int i = 0; i < cant; i++)
    {
        // Update Edges Indexes
        Cubie tmp = edges[EDGE::UB];
        edges[EDGE::UB] = edges[EDGE::RB];
        edges[EDGE::RB] = edges[EDGE::DB];
        edges[EDGE::DB] = edges[EDGE::LB];
        edges[EDGE::LB] = tmp;

        // Update Corner Indexes
        tmp = corners[CORNER::URB];
        corners[CORNER::URB] = corners[CORNER::DRB];
        corners[CORNER::DRB] = corners[CORNER::DLB];
        corners[CORNER::DLB] = corners[CORNER::ULB];
        corners[CORNER::ULB] = tmp;

        // Update Edges Colors
        swap(edges[EDGE::UB].color.X, edges[EDGE::UB].color.Y);
        swap(edges[EDGE::RB].color.X, edges[EDGE::RB].color.Y);
        swap(edges[EDGE::DB].color.X, edges[EDGE::DB].color.Y);
        swap(edges[EDGE::LB].color.X, edges[EDGE::LB].color.Y);

        // Update Corner Colors
        swap(corners[CORNER::URB].color.X, corners[CORNER::URB].color.Y);
        swap(corners[CORNER::DRB].color.X, corners[CORNER::DRB].color.Y);
        swap(corners[CORNER::DLB].color.X, corners[CORNER::DLB].color.Y);
        swap(corners[CORNER::ULB].color.X, corners[CORNER::ULB].color.Y);

        // Update Edges Orientation
        updateEdgeOrientation(EDGE::UB);
        updateEdgeOrientation(EDGE::RB);
        updateEdgeOrientation(EDGE::DB);
        updateEdgeOrientation(EDGE::LB);

        // Update Corners Orientation
        updateCornerOrienation(CORNER::URB, 2);
        updateCornerOrienation(CORNER::DRB, 1);
        updateCornerOrienation(CORNER::DLB, 2);
        updateCornerOrienation(CORNER::ULB, 1);
    }
}

void RubiksCube::rotR(short cant)
{
    for (int i = 0; i < cant; i++)
    {
        // Update Edges Indexes
        Cubie tmp = edges[EDGE::RF];
        edges[EDGE::RF] = edges[EDGE::DR];
        edges[EDGE::DR] = edges[EDGE::RB];
        edges[EDGE::RB] = edges[EDGE::UR];
        edges[EDGE::UR] = tmp;

        // Update Corner Indexes
        tmp = corners[CORNER::URF];
        corners[CORNER::URF] = corners[CORNER::DRF];
        corners[CORNER::DRF] = corners[CORNER::DRB];
        corners[CORNER::DRB] = corners[CORNER::URB];
        corners[CORNER::URB] = tmp;

        // Update Edges Colors
        swap(edges[EDGE::RF].color.Y, edges[EDGE::RF].color.Z);
        swap(edges[EDGE::DR].color.Y, edges[EDGE::DR].color.Z);
        swap(edges[EDGE::RB].color.Y, edges[EDGE::RB].color.Z);
        swap(edges[EDGE::UR].color.Y, edges[EDGE::UR].color.Z);

        // Update Corners Colors
        swap(corners[CORNER::URF].color.Y, corners[CORNER::URF].color.Z);
        swap(corners[CORNER::DRF].color.Y, corners[CORNER::DRF].color.Z);
        swap(corners[CORNER::DRB].color.Y, corners[CORNER::DRB].color.Z);
        swap(corners[CORNER::URB].color.Y, corners[CORNER::URB].color.Z);

        // Update Corners Orientations
        updateCornerOrienation(CORNER::URF, 2);
        updateCornerOrienation(CORNER::DRF, 1);
        updateCornerOrienation(CORNER::DRB, 2);
        updateCornerOrienation(CORNER::URB, 1);

        // Edges orientations only changes when F/B moves are performed
    }
}
