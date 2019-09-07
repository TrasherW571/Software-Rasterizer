#include <iostream>
#include "Point.h"

Point::Point()
{

}

Point::Point(double x, double y, double z)
{
	xCoord = x;
	yCoord = y;
	zCoord = z;
	rVal = 0;
	bVal = 0;
	gVal = 0;
	aCoord = 0.0;
	bCoord = 0.0;
	cCoord = 0.0;
}


Point::~Point()
{
}