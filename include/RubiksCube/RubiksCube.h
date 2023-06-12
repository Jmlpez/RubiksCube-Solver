#ifndef RUBIKS_CUBE_H
#define RUBIKS_CUBE_H

// #include <vector>
// #include <array>
/*
    The moves are defined as:
 U, D, L, R, F, B  --> clockwise
 U, D', L', R', F', B' --> counter_clockwise
 U2, D2, L2, R2, F2, B2 --> double_twist (count as one move)

*/

#include <iostream>

using namespace std;

class RubiksCube
{
public:
  // Attributes:
  enum COLOR : short
  { RED, BLUE, WHITE, GREEN, YELLOW, ORANGE};
  enum CORNER : short
  { ULF, URF, URB, ULB, DLF, DRF, DRB, DLB };
  enum EDGE : short
  { UF, UR, UB, UL, LF, RF, RB, LB, DF, DR, DB, DL};
  enum FACE : short
  { UP, LEFT, FRONT, RIGHT, BACK, DOWN };
  enum MOVE : short
  { L, LPRIME, L2, 
    R, RPRIME, R2, 
    U, UPRIME, U2, 
    D, DPRIME, D2, 
    F, FPRIME, F2, 
    B, BPRIME, B2, NONE
  };
  struct cubieCol{
    RubiksCube::COLOR X;
    RubiksCube::COLOR Y;
    RubiksCube::COLOR Z;
  };

  struct Cubie
  {
    Cubie(){};
    // 0-11 for edges, 0-7 for corners
    short index;
    // 0,1 for edges, 0-2 for corners
    short orientation;
    // X, Y for edges, X,Y,Z for corners
    cubieCol color;
  };

  RubiksCube();
  RubiksCube(const RubiksCube& cube);
  RubiksCube& operator=(const RubiksCube& cube);

  bool isSolved();

  void updateEdgeOrientation(EDGE ind);
  void updateCornerOrienation(CORNER ind, int cant);

  void setEdgesColor(EDGE ind);
  void setCornersColor(CORNER ind);

  cubieCol getEdgeColor(EDGE ind);
  cubieCol getCornerColor(CORNER ind);

  short getEdgeOrientation(EDGE ind);
  short getCornerOrientation(CORNER ind);
  
  short getEdgeIndex(EDGE ind);
  short getCornerIndex(CORNER ind);

  COLOR getFaceColor(FACE F, int row, int col);

  // Moves
  static MOVE invert(MOVE idx);
  void move(MOVE idx);
  void rotU(short cant = 1);
  void rotF(short cant = 1);
  void rotL(short cant = 1);
  void rotD(short cant = 1);
  void rotB(short cant = 1);
  void rotR(short cant = 1);


  Cubie edges[12];
  Cubie corners[8];
  COLOR centers[6];
  // string path;
};

#endif