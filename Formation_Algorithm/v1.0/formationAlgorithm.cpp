#include "Arduino.h"
#include "formationAlgorithm.h"



int compareVal(const void *ptr1, const void *ptr2){
	
  Index *p1 = (Index*)ptr1;
  Index *p2 = (Index*)ptr2;
  if (p1->val < p2->val) return -1;
  if (p1->val == p2->val)return 0;
  if (p1->val > p2->val)return 1;
}

int comparedistToTarget(const void *ptr1, const void *ptr2){
  Robot *p1 = (Robot*)ptr1;
  Robot *p2 = (Robot*)ptr2;
  if (p1->distToTarget < p2->distToTarget) return -1;
  if (p1->distToTarget == p2->distToTarget)return 0;
  if (p1->distToTarget > p2->distToTarget)return 1;
}

int Formation::dist(Coordinate a, Coordinate b){
  float tempX = (a.x - b.x)*(a.x - b.x);
  float tempY = (a.y - b.y)*(a.y - b.y);
  float test = tempX + tempY;
  int temp = sqrt(test);
  return temp;
}

int Formation::orient(Coordinate a, Coordinate b, Coordinate c){
  /*int q = (a.y-b.y)/(a.x-b.x);
  int p = (b.y-c.y)/(b.x-c.x);*/
  int temp = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
  if (temp == 0) {
    return 0;//collinear
  }
  else if (temp<0) {
    return 1;//counterclockwise
  }
  else {
    return 2;//clockwise
  }
}

Coordinate Formation::hullCentroidCalc(Coordinate points[]){
   int tempRes[4];
  for (int i = 0; i<4; i++) {
    tempRes[i] = -1;
  }
  int leftPoint = 0;
  for (int i = 1; i<4; i++) {
    if (points[i].x<points[leftPoint].x)
      leftPoint = i;
  }

  int a = leftPoint;
  int b = 0;
  int ct = 1;
  while (b != leftPoint || ct == 1)//changed to the ct method instead of the do-while loop
  {
    b = (a + 1) % 4;
    for (int w = 0; w<4; w++) {
      if (orient(points[a], points[w], points[b]) == 1) {
        b = w;
      }
    }
    tempRes[a] = b;
    a = b;
    ct++;
  }
  int hullNum = 0;
  memset(centArr,0,4);
  for (int z = 0; z<4; z++) {
    if (tempRes[z] != -1) {
      Serial.println(points[z].x);
      Serial.println(points[z].y);
      centArr[z].x = points[z].x;
      centArr[z].y = points[z].y;
      hullNum++;
    }
  }
  if (hullNum == 4) { //shouldnt need to decrement by 1
    centroid.x = (centArr[0].x + centArr[1].x + centArr[2].x + centArr[3].x) / 4;
    centroid.y = (centArr[0].y + centArr[1].y + centArr[2].y + centArr[3].y) / 4;
  }
  else {
    centroid.x = (centArr[0].x + centArr[1].x + centArr[2].x + centArr[3].x) / 3; //divide by hullNum
    centroid.y = (centArr[0].y + centArr[1].y + centArr[2].y + centArr[3].y) / 3;
  }
  Serial.println(hullNum);
  Serial.println("Centroid:");
  Serial.println(centroid.x);
  Serial.println(centroid.y);
  return centroid; 
}

void Formation::squareCalc(Coordinate centroid, int squareSize, Coordinate Square[4]){
  int apothem = squareSize / 2; //change var name-done
  Square[0].x = centroid.x + apothem;
  Square[0].y = centroid.y + apothem;
  Square[1].x = centroid.x - apothem;
  Square[1].y = centroid.y - apothem;
  Square[2].x = centroid.x + apothem;
  Square[2].y = centroid.y - apothem;
  Square[3].x = centroid.x - apothem;
  Square[3].y = centroid.y + apothem;
}

void Formation::getCoordinates(){
}

void Formation::pointAssignment(){
  for (int i = 0; i < 4; i++) {
    robots[i].targetPt = -1;
  }
  for (int m = 0; m <= 15; m++) {//init the distIndex Squareay
    distIndex[m].robot = (m % 4);
    distIndex[m].point = (m / 4);
    distIndex[m].pointUsed = false;
    distIndex[m].val = dist(robots[distIndex[m].point].robotPoint, Square[m % 4]);
  }
  qsort(distIndex, 16, sizeof(Index), compareVal);
  for (int i = 0; i <= 15; i++) {//point assignment
    int temp = distIndex[i].point;
    if (robots[distIndex[i].robot].havePoint == false && distIndex[i].pointUsed == false) {
      robots[distIndex[i].robot].targetPt = temp;
      robots[distIndex[i].robot].havePoint = true;
      for (int m = 0; m <= 15; m++) {
        if (temp == distIndex[m].point) {
          distIndex[m].pointUsed = true;
        }
      }
    }
  }
}

void Formation::pointAssignment1(){
  for (int i = 0; i < 4; i++) {
    robots[i].targetPt = -1;
  }
  for (int m = 0; m <= 15; m++) {//init the distIndex Squareay
    distIndex[m].robot = (m % 4);
    distIndex[m].point = (m / 4);
    distIndex[m].pointUsed = false;
    distIndex[m].val = dist(robots[distIndex[m].point].robotPoint, tempSquare[m % 4]);
	Serial.println("tempSquare");
	//Serial.println(tempSquare[m%4]);
  }
  qsort(distIndex, 16, sizeof(Index), compareVal);
  for (int i = 0; i <= 15; i++) {//point assignment
    int temp = distIndex[i].point;
    if (robots[distIndex[i].robot].havePoint == false && distIndex[i].pointUsed == false) {
      robots[distIndex[i].robot].targetPt = temp;
      robots[distIndex[i].robot].havePoint = true;
      for (int m = 0; m <= 15; m++) {
        if (temp == distIndex[m].point) {
          distIndex[m].pointUsed = true;
        }
      }
    }
  }
}

void Formation::squareOptimization(){
  int currDistWanted = 250;
  for (int i = 0; i < 4; i++) {//figure out the preferred distances for each robot
    for (int m = 0; m < 4; m++) {
      int temp = dist(robots[i].robotPoint, Square[m]);
      if (temp < currDistWanted) {
        currDistWanted = temp;
      }
    }
    robots[i].distToPtWanted= currDistWanted;
  }
  for (int i = 0; i < 4; i++) {
    Serial.println("These are the coordinates of the initial square:");
    Serial.println(Square[robots[i].targetPt].x);
    Serial.println(Square[robots[i].targetPt].y);
    int temp1 = dist(Square[robots[i].targetPt], robots[i].robotPoint);
    Serial.println(temp1);
    robots[i].distToTarget = dist(Square[robots[i].targetPt], robots[i].robotPoint);
    int temp = abs((robots[0].distToPtWanted - robots[0].distToTarget));
    int error = (temp + abs((robots[1].distToTarget - robots[1].distToPtWanted))
      + abs((robots[2].distToTarget - robots[2].distToPtWanted)) + abs((robots[3].distToTarget - robots[3].distToPtWanted)))/4;
    Serial.println("Error:");
    Serial.println(error);
  }
  Serial.println("TempSquare");
  for(int i=0;i<4;i++){
	  Serial.println(tempSquare[i].x);
	  Serial.println(tempSquare[i].y);
  }
  Coordinate buffer[20];
  int errors[25];
  currDistWanted = 250;
  int ct = 0;
  int sum = 0;
  Coordinate tempCentroid;
  tempCentroid = centroid;
  for (int i = 0; i < 4; i++) {
    tempSquare[i] = Square[i];
  }
  Serial.println("TempSquare");
  for(int i=0;i<4;i++){
	  Serial.println(tempSquare[i].x);
	  Serial.println(tempSquare[i].y);
  }
  Serial.println("End TempSquare");
  for (int m = 0; m < 4; m++) {
      robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
    }
  qsort(robots, 4, sizeof(Robot), comparedistToTarget);
  int initialLongestDistance = robots[3].distToTarget;
  int currLongestDistance = initialLongestDistance;
  Serial.print("Initial Longest Distance:");
  Serial.println(initialLongestDistance);
  for (int i = 0; i < initialLongestDistance; i++) {
    tempCentroid.x++;
    tempCentroid.y++;
    squareCalc(tempCentroid, m_squareSize, tempSquare);
    for (int m = 0; m < 4; m++) {
      robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
    }
    qsort(robots, 4, sizeof(Robot), comparedistToTarget);
    if (robots[3].distToTarget < currLongestDistance) {
		currLongestDistance = robots[3].distToTarget;
	Serial.println("Fresh off the boat");
	  for(int i=0;i<4;i++){
		  m_point[i][0] = tempSquare[robots[i].targetPt].x;
		  m_point[i][1] = tempSquare[robots[i].targetPt].y;
		  Serial.println(m_point[i][0]);
		  Serial.println(m_point[i][1]);
	  }
    }
  }
  /*for (int i = 0; i < currLongestDistance; i++) {
    tempCentroid.x++;
    squareCalc(tempCentroid, m_squareSize, tempSquare);
    for (int m = 0; m < 4; m++) {
      robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
    }
    qsort(robots, 4, sizeof(Robot), comparedistToTarget);
    if (robots[3].distToTarget < currLongestDistance) {
      currLongestDistance = robots[3].distToTarget;
	  for(int i=0;i<4;i++){
		  m_point[i][0] = tempSquare[robots[i].targetPt].x;
		  m_point[i][1] = tempSquare[robots[i].targetPt].y;
	  }
    }
  }
  for (int i = 0; i < currLongestDistance; i++) {
    tempCentroid.y++;
    squareCalc(tempCentroid, m_squareSize, tempSquare);
    for (int m = 0; m < 4; m++) {
      robots[m].distToTarget = dist(tempSquare[robots[m].targetPt], robots[m].robotPoint);
    }
    qsort(robots, 4, sizeof(Robot), comparedistToTarget);
    if (robots[3].distToTarget < currLongestDistance) {
      currLongestDistance = robots[3].distToTarget;
	  for(int i=0;i<4;i++){
		  m_point[i][0] = tempSquare[robots[i].targetPt].x;
		  m_point[i][1] = tempSquare[robots[i].targetPt].y;
	  }
    }
  }*/
  Serial.println("Best Optimization Distance:");
  Serial.println(currLongestDistance);
 }

void Formation::getSquare(){
  robots[0].robotPoint.x = m_point[0][0];
  robots[0].robotPoint.y = m_point[0][1];
  robots[1].robotPoint.x = m_point[1][0];
  robots[1].robotPoint.y = m_point[1][1];
  robots[2].robotPoint.x = m_point[2][0];
  robots[2].robotPoint.y = m_point[2][1];
  robots[3].robotPoint.x = m_point[3][0];
  robots[3].robotPoint.y = m_point[3][1];
  Coordinate robotPoints[4];
  int theta = 0;//declarations for the square optimization part of the code
  for (int i = 0; i < 4; i++) {
    robotPoints[i].x = robots[i].robotPoint.x;
    robotPoints[i].y = robots[i].robotPoint.y;
  }
  //struct Coordinate points[] = { { 34, 26 }, { 4, 7 }, { 3, 8 }, { 8, 24 } };
  centroid = hullCentroidCalc(robotPoints);
  squareCalc(centroid, m_squareSize, Square);
  Serial.println("THIS IS THE INITIAL SQUARE");
  for(int i=0;i<4;i++){
	  Serial.println(Square[i].x);
	  Serial.println(Square[i].y);
  }
  for (int i = 0; i<4; i++) {
    Serial.println(Square[i].x);
    Serial.println(Square[i].y);
  }
  pointAssignment();
  squareOptimization();
  Serial.println("Coordinates Being Sent Over:");
  Serial.println(m_point[0][0]);
  Serial.println(m_point[0][1]);
  Serial.println(m_point[1][0]);
  Serial.println(m_point[1][1]);
  Serial.println(m_point[2][0]);
  Serial.println(m_point[2][1]);
  Serial.println(m_point[3][0]);
  Serial.println(m_point[3][1]);
}


