#ifndef formationAlgorithm_h
#define formationAlgorithm_h


#include "Arduino.h"

struct Coordinate//a basic struct to store coordinates
    {
      int x;
      int y;
    };
struct Index {//for point assignment
      int robot;
      int point;
      int val;
      bool pointUsed;
    };
struct Robot {//struct with char. of all robots
      Coordinate robotPoint;
      int targetPt;
      bool havePoint;
      int distToTarget;
      int distToPtWanted;
    };	
int compareVal(const void *ptr1, const void *ptr2);
int comparedistToTarget(const void *ptr1, const void *ptr2);


class Formation
{
  public:
    void getSquare();
	float m_point[4][2];
	Formation(int squareSize):m_squareSize(squareSize){};
  private:
	
    int m_squareSize;
	Coordinate tempSquare[4];
    Coordinate centroid;
    Coordinate Square[4];
    Coordinate centArr[4];
    Index distIndex[16];
    Robot robots[4] =
    {
      { { 109, 46 }, 2, false, 0, 0 },
      { { 105, 154 }, 2, false, 0, 0 },
      { { 97, 44 }, 2, false, 0 , 0},
      { { 65, 31 }, 2, false, 0, 0 }
    };

    int orient(Coordinate a, Coordinate b, Coordinate c);
    Coordinate hullCentroidCalc(Coordinate points[]);
    void squareCalc(Coordinate centroid, int squareSize, Coordinate arr[4]);
    int dist(Coordinate a, Coordinate b);
 

    void getCoordinates();
    void pointAssignment();
	void pointAssignment1();
    void squareOptimization();
};

#endif

