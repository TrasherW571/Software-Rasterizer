#include "Triangle.h"
#include "Point.h"
#include "Image.h"
#include <vector>
#include <ctime>

using namespace std;

Triangle::Triangle()
{

}

Triangle::Triangle(Point a, Point b, Point c)
{
	vertexA = a;
	vertexB = b;
	vertexC = c;
	area = CalculateArea(vertexA, vertexB, vertexC);
	MinMax();
	srand(time(0));
	int tempColor = rand() % 256;
	Color = (unsigned char)tempColor;
	S = 0.0;
	Tx = 0.0;
	Ty = 0.0;
}

double Triangle::CalculateArea(Point A, Point B, Point C)
{
	double part1 = B.getXCoord() - A.getXCoord();
	double part2 = C.getYCoord() - A.getYCoord();
	double part3 = C.getXCoord() - A.getXCoord();
	double part4 = B.getYCoord() - A.getYCoord();
	double area = 0.5 * ((part1 * part2) - (part3 * part4));
	return area;
}

void Triangle::BaryCentric(Point A, Point B, Point C, Point &P)
{
	area = CalculateArea(A, B, C);
	double AreaofA = CalculateArea(P, B, C);
	double AreaofB = CalculateArea(P, C, A);
	double AreaofC = CalculateArea(P, A, B);
	P.setA(AreaofA / area);
	P.setB(AreaofB / area);
	P.setC(AreaofC / area);
}

bool Triangle::insideTriangle(Point P)
{
	double a = P.getA();
	double b = P.getB();
	double c = P.getC();

	if ((a < 0) || (a > 1))
	{
		return false;
	}
	if ((b < 0) || (b > 1))
	{
		return false;
	}
	if ((c < 0) || (c > 1))
	{
		return false;
	}
	

	return true;
	
}

void Triangle::MinMax()
{
	xMin = vertexA.getXCoord();
	xMax = xMin;
	yMin = vertexA.getYCoord();
	yMax = yMin;
	zMin = vertexA.getZCoord();
	zMax = zMin;
	
	if (xMin > vertexB.getXCoord())
	{
		xMin = vertexB.getXCoord();
	}
	if (xMin > vertexC.getXCoord())
	{
		xMin = vertexC.getXCoord();
	}
	if (xMax < vertexB.getXCoord())
	{
		xMax = vertexB.getXCoord();
	}
	if (xMax < vertexC.getXCoord())
	{
		xMax = vertexC.getXCoord();
	}

	if (yMin > vertexB.getYCoord())
	{
		yMin = vertexB.getYCoord();
	}
	if (yMin > vertexC.getYCoord())
	{
		yMin = vertexC.getYCoord();
	}
	if (yMax < vertexB.getYCoord())
	{
		yMax = vertexB.getYCoord();
	}
	if (yMax < vertexC.getYCoord())
	{
		yMax = vertexC.getYCoord();
	}

	if (zMin > vertexB.getZCoord())
	{
		zMin = vertexB.getZCoord();
	}
	if (zMin > vertexC.getZCoord())
	{
		zMin = vertexC.getZCoord();
	}
	if (zMax < vertexB.getZCoord())
	{
		zMax = vertexB.getZCoord();
	}
	if (zMax < vertexC.getZCoord())
	{
		zMax = vertexC.getZCoord();
	}
}

void Triangle::setVAColor(unsigned char r, unsigned char g, unsigned char b)
{
	vertexA.setRed(r);
	vertexA.setGreen(g);
	vertexA.setBlue(b);
}

void Triangle::setVBColor(unsigned char r, unsigned char g, unsigned char b)
{
	vertexB.setRed(r);
	vertexB.setGreen(g);
	vertexB.setBlue(b);
}

void Triangle::setVCColor(unsigned char r, unsigned char g, unsigned char b)
{
	vertexC.setRed(r);
	vertexC.setGreen(g);
	vertexC.setBlue(b);
}

Triangle::~Triangle()
{

}