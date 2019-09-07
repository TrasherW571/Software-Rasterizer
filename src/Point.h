#pragma once

class Point
{
public:
	Point();
	Point(double x, double y, double z);
	~Point();
	//Getter Functions
	double getXCoord() { return xCoord; }
	double getYCoord() { return yCoord; }
	double getZCoord() { return zCoord; }
	double getA() { return aCoord; }
	double getB() { return bCoord; }
	double getC() { return cCoord; }
	unsigned char getRed() { return rVal; }
	unsigned char getBlue() { return bVal; }
	unsigned char getGreen() { return gVal; }
	//Setter Functions
	void setXCoord(double x) { xCoord = x; }
	void setYCoord(double y) { yCoord = y; }
	void setZCoord(double z) { zCoord = z; }
	void setRed(unsigned char r) { rVal = r; }
	void setBlue(unsigned char b) { bVal = b; }
	void setGreen(unsigned char g) { gVal = g; }
	void setA(double a) { aCoord = a; }
	void setB(double b) { bCoord = b; }
	void setC(double c) { cCoord = c; }

private:
	double xCoord, yCoord, zCoord, aCoord, bCoord, cCoord;
	unsigned char rVal, bVal, gVal;
};
