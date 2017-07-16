#ifndef READOBJ_H
#define READOBJ_H

#include "Vertex.h"
#include "Triangle.h"

#include <fstream>
#include <sstream>
#include <string>

#define uint unsigned int

//http://cboard.cprogramming.com/game-programming/89759-how-load-obj-files.html

//5/2/1 is a point, "5/2/1 6/3/2 1/1/3" is a triangle.

//f 5/2/1 6/3/2 1/1/3 i obj-fil is the same as...
//f pos[4]/tex[1]/nor[0] pos[5]/tex[2]/nor[1] pos[0]/tex[0]/nor[2].

class readOBJ
{
private:
	Vertex vertex;
	Triangle triangle;

	string line, en_vertex, ett_index, toVector[3];
	int vectorID[3];
	int newNum;
	float num;

	uint found_delimiter;

	vector<D3DXVECTOR3> p, n;
	vector<D3DXVECTOR2> t;

	 
public:
	vector<Triangle> triList;

	readOBJ();
	~readOBJ();

	void readFile(char* fileName);

	float stringToFloat(string line);
	void putStrInArr(int i, ifstream &file);
};

#endif