#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <limits>
#include "Image.h"
#include "Point.h"
#include "Triangle.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

struct Color
{
	unsigned char r, g, b;
};
//Calculate Scale factor
double CalculateScale(double Xmax, double Xmin, double Ymax, double Ymin, int width, int height)
{
	//Calculates the Scale, S
	double S;
	double part1 = Xmax - Xmin;
	double Sx = width / part1;
	part1 = Ymax - Ymin;
	double Sy = height / part1;
	if (Sx < Sy)
	{
		return Sx;
	}
	else
	{
		return Sy;
	}
}
//Calculate Translation factors
double CalculateTranslation(double Xmax, double Xmin, double Ymax, double Ymin, int Var, double S, int option)
{
	double part2 = 0.5 * (Xmax + Xmin);
	double part3 = 0.5 * (Ymax + Ymin);
	if (option == 1)
	{
		double Tx = (0.5 * Var) - (S * part2);
		return Tx;
	}
	else
	{
		double Ty = (0.5 * Var) - (S * part3);
		return Ty;
	}
}

//Transforms(Scales and Translates) the coordinates for each of the triangle's vertexes
void TransformVertexes(Triangle &T, double S, double Tx, double Ty)
{
	double tempX = (S * T.getAX()) + Tx;
	double tempY = (S * T.getAY()) + Ty;
	T.setAX(tempX);
	T.setAY(tempY);
	tempX = (S * T.getBX()) + Tx;
	tempY = (S * T.getBY()) + Ty;
	T.setBX(tempX);
	T.setBY(tempY);
	tempX = (S * T.getCX()) + Tx;
	tempY = (S * T.getCY()) + Ty;
	T.setCX(tempX);
	T.setCY(tempY);
	T.MinMax();
}
//Generates random RGB values for each of the three vertices of the triangle. Used for color mode 0
void GenerateColors(Triangle &T, string vertex)
{
	int tempR = rand() % 255;
	unsigned char tR = (unsigned char)tempR;
	int tempG = rand() % 255;
	unsigned char tG = (unsigned char)tempG;
	int tempB = rand() % 255;
	unsigned char tB = (unsigned char)tempB;

	if (vertex == "a")
	{
		T.setVAColor(tR, tG, tB);
	}
	else if (vertex == "b")
	{
		T.setVBColor(tR, tG, tB);
	}
	else if (vertex == "c")
	{
		T.setVCColor(tR, tG, tB);
	}
}

//Interpolates the pixels color using pixel's barycentric coordinates (3 random colors at each triangle vertex interpolating)
void ColorMode0(Triangle T, Point &P)
{
	double rVal = T.getVertexA().getRed();
	double gVal = T.getVertexA().getGreen();
	double bVal = T.getVertexA().getBlue();
	double Color1[3] = { rVal, gVal, bVal };
	rVal = T.getVertexB().getRed();
	gVal = T.getVertexB().getGreen();
	bVal = T.getVertexB().getBlue();
	double Color2[3] = { rVal, gVal, bVal };
	rVal = T.getVertexC().getRed();
	gVal = T.getVertexC().getGreen();
	bVal = T.getVertexC().getBlue();
	double Color3[3] = { rVal, gVal, bVal };

	double R = (Color1[0] * P.getA()) + (Color2[0] * P.getB()) + (Color3[0] * P.getC());
	double G = (Color1[1] * P.getA()) + (Color2[1] * P.getB()) + (Color3[1] * P.getC());
	double B = (Color1[2] * P.getA()) + (Color2[2] * P.getB()) + (Color3[2] * P.getC());
	
	P.setRed((unsigned char)R);
	P.setGreen((unsigned char)G);
	P.setBlue((unsigned char)B);
}
//uses the z value of each point to color the pixel, colors depths and shadow for the image
void ColorMode1(const vector<vector<double> > &Z, int x, int y, vector<vector<Color> > &F, double Zmin, double Zmax)
{
	double ZDiff = Zmax - Zmin;
	double BVal = static_cast<double>(F[y][x].b);
	//need to subtract Zmin from current value because some values are negative which then causes the RGB value to be negative which you can't have
	double Zb = Z[y][x] - Zmin;
	double ColorZ = (BVal * Zb) / ZDiff;

	F[y][x] = { 0, 0, (unsigned char)ColorZ };

}
//colors a linear gradient along the y axis from one color to another
void ColorMode2(Point &P, int y, int yMax, int yMin)
{
	double Color1[3] = { 255, 128, 0 };	//orange
	double Color2[3] = { 0, 255, 255 };	//cyan
	
	double numerator = yMax - y;
	double denominator = yMax - yMin;
	double percent = numerator / denominator;
	double R = Color1[0] + percent * (Color2[0] - Color1[0]);
	double G = Color1[1] + percent * (Color2[1] - Color1[1]);
	double B = Color1[2] + percent * (Color2[2] - Color1[2]);

	P.setRed((unsigned char)R);
	P.setGreen((unsigned char)G);
	P.setBlue((unsigned char)B);
}

//Calculates Z Coordinate for the Pixel
/* ******************************************************************************************
*Title: Triangle $z$-index interpolation between the vertices
*Author: Arundel (https://math.stackexchange.com/users/54148/arundel)
*Date: 01/26/2019
*Availability: https://math.stackexchange.com/questions/349444/triangle-z-index-interpolation-between-the-vertices
*
*************************************************************************************** */
void CalculateZ(Triangle T, Point &P)
{
	P.setZCoord((T.getAZ() * P.getA()) + (T.getBZ() * P.getB()) + (T.getCZ() * P.getC()));
}
//This does the Z buffer for our image
void ZBuffer(vector<vector<double> > &Z, vector<vector<Color> > &F, int x, int y, Point &P)
{
	if (Z[y][x] < P.getZCoord())
	{
		Z[y][x] = P.getZCoord();
		F[y][x] = { P.getRed(), P.getGreen(), P.getBlue() };
	}
}

int main(int argc, char **argv)
{
	srand(time(NULL));
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}
	string meshName(argv[1]);

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
	//sets width of the image
	int width = atoi(argv[3]);
	//sets height of the image
	int height = atoi(argv[4]);
	//sets outpute filename of the image
	string filename(argv[2]);
	//sets color mode of the image
	int colorMode = atoi(argv[5]);
	if ((colorMode != 0) && (colorMode != 1) && (colorMode != 2))
	{
		cout << "Please enter either 0, 1, or 2 for the color mode" << endl;
		return 0;
	}
	//creates the image
	auto image = make_shared<Image>(width+1, height+1);
	//Vector containing our triangles
	vector<Triangle> Tri;
	//Vector for containing Pixel coordinates
	vector<Point> Pixels;
	//adding triangles to our vector
	for (int i = 0; i < posBuf.size(); i++)
	{
		Tri.push_back(Triangle(Point(posBuf[i], posBuf[i + 1], posBuf[i + 2]), Point(posBuf[i + 3], posBuf[i + 4], posBuf[i + 5]), Point(posBuf[i+6], posBuf[i + 7], posBuf[i + 8])));
		i = i + 8;
	}
	//Calculating max/min of all vertexes for x and y
	double YMin = Tri[0].getYmin();
	double YMax = Tri[0].getYmax();
	double XMin = Tri[0].getXmin();
	double XMax = Tri[0].getXmax();
	for (int i = 1; i < Tri.size(); i++)
	{
		if (YMin > Tri[i].getYmin())
		{
			YMin = Tri[i].getYmin();
		}
		if (YMax < Tri[i].getYmax())
		{
			YMax = Tri[i].getYmax();
		}
		if (XMin > Tri[i].getXmin())
		{
			XMin = Tri[i].getXmin();
		}
		if (XMax < Tri[i].getXmax())
		{
			XMax = Tri[i].getXmax();
		}
	}
	//calculating scale and translation factors
	double Scale = CalculateScale(XMax, XMin, YMax, YMin, width, height);
	double Tx = CalculateTranslation(XMax, XMin, YMax, YMin, width, Scale, 1);
	double Ty = CalculateTranslation(XMax, XMin, YMax, YMin, height, Scale, 2);

	YMax = (Scale * YMax) + Ty;
	YMin = (Scale * YMin) + Ty;
	//Creates Z buffer and Frame buffer
	vector<vector<double> > ZBuff(floor(height));
	vector<vector<Color> > FBuff(floor(height));
	//Initializes Z buffer and Frame buffer
	for (int i = 0; i < ZBuff.size(); i++)
	{
		ZBuff[i] = vector<double>(width+1);
		FBuff[i] = vector<Color>(width+1);
		for (int j = 0; j < width; j++)
		{
			ZBuff[i][j] = -numeric_limits<double>::infinity();
			FBuff[i][j] = { (unsigned char)0.0, (unsigned char)0.0, (unsigned char)0.0 };
		}
	}

	double Zmin = numeric_limits<double>::infinity();
	double Zmax = -numeric_limits<double>::infinity();
	//Creates the bounding box for each triangle and colors the pixel if color mode 0 or 2 were selected
	for (int i = 0; i < Tri.size(); i++)
	{
		TransformVertexes(Tri[i], Scale, Tx, Ty);
		if (colorMode == 0)
		{
			GenerateColors(Tri[i], "a");
			GenerateColors(Tri[i], "b");
			GenerateColors(Tri[i], "c");
		}
		int tempYmin = ceil(Tri[i].getYmin());
		int tempYmax = floor(Tri[i].getYmax());
		for (int y = tempYmin; y <= tempYmax; ++y) {
			int tempXmin = ceil(Tri[i].getXmin());
			int tempXmax = floor(Tri[i].getXmax());
			for (int x = tempXmin; x <= tempXmax; ++x) {
				Pixels.push_back(Point(x, y, 0.0));
				Tri[i].BaryCentric(Tri[i].getVertexA(), Tri[i].getVertexB(), Tri[i].getVertexC(), Pixels.back());
				CalculateZ(Tri[i], Pixels.back());
				if (Tri[i].insideTriangle(Pixels.back()) == true)
				{
					if (colorMode == 0)
					{
						ColorMode0(Tri[i], Pixels.back());
						ZBuffer(ZBuff, FBuff, x, y, Pixels.back());
						image->setPixel(x, y, FBuff[y][x].r, FBuff[y][x].g, FBuff[y][x].b);
					}
					if (colorMode == 2)
					{
						ColorMode2(Pixels.back(), y, YMax, YMin);
						ZBuffer(ZBuff, FBuff, x, y, Pixels.back());
						image->setPixel(x, y, FBuff[y][x].r, FBuff[y][x].g, FBuff[y][x].b);
					}
					
					if (colorMode == 1)
					{
						Pixels.back().setBlue(255);
						ZBuffer(ZBuff, FBuff, x, y, Pixels.back());
					}
					
					if (Zmax < ZBuff[y][x])
					{
						Zmax = ZBuff[y][x];
					}
					if (Zmin > ZBuff[y][x])
					{
						Zmin = ZBuff[y][x];
					}
					
				}
				Pixels.pop_back();
			}
		}
	}
	//colors the pixels if color mode 1 was selected
	if (colorMode == 1)
	{
		for (int i = 0; i < FBuff.size(); i++)
		{
			for (int j = 0; j < FBuff[i].size(); j++)
			{
				ColorMode1(ZBuff, j, i, FBuff, Zmin, Zmax);
				image->setPixel(j, i, FBuff[i][j].r, FBuff[i][j].g, FBuff[i][j].b);
			}
		}
	}
	
	
	// Write image to file
	image->writeToFile(filename);
	return 0;
}