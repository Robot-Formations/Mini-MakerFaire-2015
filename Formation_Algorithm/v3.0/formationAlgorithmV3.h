#ifndef formationAlgorithmV3_h
#define formationAlgoritmV3_h

#include "Arduino.h"

struct Coordinate
	{
		int x;
		int y;
	};
struct Index
	{
		int robot;
		int point;
		int val;
		bool pointUsed;
	};
struct Robot
	{
		Coordinate robotPoint;
		int targetPt;
		bool havePoint;
		int distToTarget;
		int disttoPtWanted;
	};
int compareVal(const void *ptr1, const void *ptr2);
int compareDistToTarget(const void *ptr1, const void *ptr2);

class Formation{
	public:
		void getShape(int numRobots);
		float m_point[4][2];
		Formation(int shapeSize):m_shapeSize(shapeSize){};
	private:
		int m_shapeSize;
		Coordinate tempShape[4];
		Coordinate centroid;
		Coordinate shape[4];
		Coordinate centArr[4];
		Index distIndex[16];
		Index tridistIndex[12];
		Robot robots[4] =
		{
		  { { 109, 46 }, 2, false, 0, 0 },
		  { { 105, 154 }, 2, false, 0, 0 },
		  { { 97, 44 }, 2, false, 0 , 0},
		  { { 65, 31 }, 2, false, 0, 0 }
		};
		int orient(Coordinate a, Coordinate b, Coordinate c);
		Coordinate hullCentroidCalc(Coordinate points[], int numPoints);
		void shapeCalc(int numRobots);
		int dist(Coordinate a, Coordinate b);
		void pointAssignment();
		void triPointAssignment();
};

#endif