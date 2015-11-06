#include "Arduino.h"
#include "formationAlgorithmV3.h"



int compareVal(const void *ptr1, const void *ptr2){//for the qsort function
	
  Index *p1 = (Index*)ptr1;
  Index *p2 = (Index*)ptr2;
  if (p1->val < p2->val) return -1;
  if (p1->val == p2->val)return 0;
  if (p1->val > p2->val)return 1;
}

int comparedistToTarget(const void *ptr1, const void *ptr2){//for the distToTarget part of the robots array for qsort
  Robot *p1 = (Robot*)ptr1;
  Robot *p2 = (Robot*)ptr2;
  if (p1->distToTarget < p2->distToTarget) return -1;
  if (p1->distToTarget == p2->distToTarget)return 0;
  if (p1->distToTarget > p2->distToTarget)return 1;
}

int Formation::dist(Coordinate a, Coordinate b){//distance function
  float tempX = (a.x - b.x)*(a.x - b.x);
  float tempY = (a.y - b.y)*(a.y - b.y);
  float test = tempX + tempY;
  int temp = sqrt(test);
  return temp;
}

int Formation::orient(Coordinate a, Coordinate b, Coordinate c){//orient function
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

Coordinate Formation::hullCentroidCalc(Coordinate points[], int numRobots){//centroid calculator
  int hullNum = 0;
  if(numRobots==3){
	  Serial.println("Neelay you're an idiot");
	  centroid.x = (points[0].x + points[1].x + points[2].x)/3;
	  centroid.y = (points[0].y + points[1].y + points[2].y)/3;
	  return centroid;
  }
  else
  {
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
    centroid.x = (centArr[0].x + centArr[1].x + centArr[2].x + centArr[3].x) / 3;
    centroid.y = (centArr[0].y + centArr[1].y + centArr[2].y + centArr[3].y) / 3;
  }
  Serial.println(hullNum);
  
  Serial.println("Centroid:");
  Serial.println(centroid.x);
  Serial.println(centroid.y);
  return centroid; 
}


void Formation::shapeCalc(int numRobots){
	if(numRobots==3){
		int apothem = m_shapeSize/2;
		float triApothem = sqrt(3)/6*m_shapeSize;
		shape[0].x = centroid.x-apothem;
		shape[0].y = centroid.y-triApothem;
		shape[1].x = centroid.x+apothem;
		shape[1].y = centroid.y-triApothem;
		shape[2].x = centroid.x;
		shape[2].y = centroid.y+2*triApothem;
		shape[3] = {0,0};
	}
	else{
		int apothem = m_shapeSize / 2; 
		shape[0].x = centroid.x + apothem;
		shape[0].y = centroid.y + apothem;
	    shape[1].x = centroid.x - apothem;
		shape[1].y = centroid.y - apothem;
		shape[2].x = centroid.x + apothem;
		shape[2].y = centroid.y - apothem;
		shape[3].x = centroid.x - apothem;
		shape[3].y = centroid.y + apothem;	
	}
}

void Formation::pointAssignment(){//assigning robots to points on the shape
  for (int i = 0; i < 4; i++) {
    robots[i].targetPt = -1;
  }
  for (int m = 0; m <= 15; m++) {//init the distIndex 
    distIndex[m].robot = (m % 4);
    distIndex[m].point = (m / 4);
    distIndex[m].pointUsed = false;
    distIndex[m].val = dist(robots[distIndex[m].point].robotPoint, shape[m % 4]);
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

void Formation::triPointAssignment(){//assigning robots to points on the shape
  for (int i = 0; i < 4; i++) {
    robots[i].targetPt = -1;
  }
  for (int m = 0; m <= 11; m++) {//init the tridistIndex 
    tridistIndex[m].robot = (m % 4);
    tridistIndex[m].point = (m / 3);
    tridistIndex[m].pointUsed = false;
    tridistIndex[m].val = dist(robots[tridistIndex[m].point].robotPoint, shape[m % 3]);
  }
  qsort(tridistIndex, 12, sizeof(Index), compareVal);
  for (int i = 0; i <= 11; i++) {//point assignment
    int temp = tridistIndex[i].point;
    if (robots[tridistIndex[i].robot].havePoint == false && tridistIndex[i].pointUsed == false) {
      robots[tridistIndex[i].robot].targetPt = temp;
      robots[tridistIndex[i].robot].havePoint = true;
      for (int m = 0; m <= 11; m++) {
        if (temp == tridistIndex[m].point) {
          tridistIndex[m].pointUsed = true;
        }
      }
    }
  }
}
void Formation::getShape(int numRobots){
  robots[0].robotPoint.x = m_point[0][0];
  robots[0].robotPoint.y = m_point[0][1];
  robots[1].robotPoint.x = m_point[1][0];
  robots[1].robotPoint.y = m_point[1][1];
  robots[2].robotPoint.x = m_point[2][0];
  robots[2].robotPoint.y = m_point[2][1];
  robots[3].robotPoint.x = m_point[3][0];
  robots[3].robotPoint.y = m_point[3][1];
  Coordinate robotPoints[4];//array for calculating the convex hull and centroid function
  for (int i = 0; i < 4; i++) {//transferring vals from robots[] to robotPoints[]
    robotPoints[i].x = robots[i].robotPoint.x;
    robotPoints[i].y = robots[i].robotPoint.y;
  }
  centroid = hullCentroidCalc(robotPoints,numRobots);//calculate the centroid
  shapeCalc(numRobots);
  Serial.println("THIS IS THE INITIAL SHAPE");
  for(int i=0;i<4;i++){
	  Serial.println(shape[i].x);
	  Serial.println(shape[i].y);
  }
  if(numRobots==3){
	  triPointAssignment();
  }
  else{
	  pointAssignment();
  }
  m_point[0][0] = shape[robots[0].targetPt].x;
  m_point[0][1]= shape[robots[0].targetPt].y;
  m_point[1][0]= shape[robots[1].targetPt].x;
  m_point[1][1]= shape[robots[1].targetPt].y;
  m_point[2][0]= shape[robots[2].targetPt].x;
  m_point[2][1]= shape[robots[2].targetPt].y;
  m_point[3][0]= shape[robots[3].targetPt].x;
  m_point[3][1]= shape[robots[3].targetPt].y;
  for(int i=0;i<3;i++){
	  for(int m=0;m<2;m++){
		  if(m_point[i][m]==0 && m_point[i][m+1]==0){
			  m_point[i][m] = m_point[i+1][m];
			  m_point[i][m+1] = m_point[i+1][m+1];
			  m_point[i+1][m] = 0;
			  m_point[i+1][m+1] = 0;
		  }
	  }
  }
  Serial.println("Coordinates Being Sent Over:");
  for(int i=0;i<4;i++){
	  for(int m=0;m<2;m++){
		  Serial.println(m_point[i][m]);
	  }
  }
  Serial.println("Robots and Their Assigned Points");
  for(int i=0;i<4;i++){
	  Serial.println("Robot");
	  Serial.println(i);
	  Serial.println(robots[i].targetPt);
  }
}
