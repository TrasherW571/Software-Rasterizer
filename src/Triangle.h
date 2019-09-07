#pragma once
#include "Point.h"
#include "Image.h"
#include <vector>

using namespace std;

class Triangle {
private:
	Point vertexA, vertexB, vertexC;
	double area;
	double xMax, xMin, yMax, yMin, zMax, zMin;
	double S, Tx, Ty;
	unsigned char Color;
public:
	Triangle();
	Triangle(Point a, Point b, Point c);
	virtual ~Triangle();
	//Calculates the Min and Max for X, Y, Z coordinates of the three vertices
	void MinMax();
	//Calculates the weights for the pixel
	void BaryCentric(Point A, Point B, Point C, Point &P);
	//Calculates area of the triangle
	double CalculateArea(Point A, Point B, Point C);
	//Checks to see if pixel is in triangle or not
	bool insideTriangle(Point P);
	//getter functions
	double getAX() { return vertexA.getXCoord(); }
	double getBX() { return vertexB.getXCoord(); }
	double getCX() { return vertexC.getXCoord(); }
	double getAY() { return vertexA.getYCoord(); }
	double getBY() { return vertexB.getYCoord(); }
	double getCY() { return vertexC.getYCoord(); }
	double getAZ() { return vertexA.getZCoord(); }
	double getBZ() { return vertexB.getZCoord(); }
	double getCZ() { return vertexC.getZCoord(); }
	double getXmin() { return xMin; }
	double getXmax() { return xMax; }
	double getYmin() { return yMin; }
	double getYmax() { return yMax; }
	double getZmin() { return zMin; }
	double getZmax() { return zMax; }
	double getScaleX() { return S; }
	double getTX() { return Tx; }
	double getTY() { return Ty; }
	Point getVertexA() { return vertexA; }
	Point getVertexB() { return vertexB; }
	Point getVertexC() { return vertexC; }
	//setter functions
	void setAX(double x) { vertexA.setXCoord(x); }
	void setAY(double y) { vertexA.setYCoord(y); }
	void setAZ(double z) { vertexA.setZCoord(z); }
	void setBX(double x) { vertexB.setXCoord(x); }
	void setBY(double y) { vertexB.setYCoord(y); }
	void setBZ(double z) { vertexB.setZCoord(z); }
	void setCX(double x) { vertexC.setXCoord(x); }
	void setCY(double y) { vertexC.setYCoord(y); }
	void setCZ(double z) { vertexC.setZCoord(z); }
	void setXmin(double x) { xMin = x; }
	void setXmax(double x) { xMax = x; }
	void setYmin(double y) { yMin = y; }
	void setYmax(double y) { yMax = y; }
	void setZmin(double z) { zMin = z; }
	void setZmax(double z) { zMax = z; }
	void setScale(double s) { S = s; }
	void setTX(double x) { Tx = x; }
	void setTY(double y) { Ty = y; }
	void setVAColor(unsigned char r, unsigned char g, unsigned char b);
	void setVBColor(unsigned char r, unsigned char g, unsigned char b);
	void setVCColor(unsigned char r, unsigned char g, unsigned char b);
};