#include "readOBJ.h"

readOBJ::readOBJ()
{
	this->num = 0.0f;

	this->line = "";
}

readOBJ::~readOBJ()
{}

void readOBJ::readFile(char* fileName)
{
	ifstream file(fileName);

	if(file.is_open())
	{
		while(!file.eof())
		{
			file >> this->line;
			if(this->line == "v")
			{
				this->putStrInArr(3, file);
				this->vertex.pos = D3DXVECTOR3(	this->stringToFloat(this->toVector[0]),
												this->stringToFloat(this->toVector[1]),
												this->stringToFloat(this->toVector[2]));
				this->p.push_back(this->vertex.pos);
			}
			else if(this->line == "vt")
			{
				this->putStrInArr(2, file);
				this->vertex.tex = D3DXVECTOR2(	this->stringToFloat(this->toVector[0]),
												this->stringToFloat(this->toVector[1]));
				this->t.push_back(this->vertex.tex);
			}
			else if(this->line == "vn")
			{
				this->putStrInArr(3, file);
				this->vertex.normal = D3DXVECTOR3(	this->stringToFloat(this->toVector[0]),
													this->stringToFloat(this->toVector[1]),
													this->stringToFloat(this->toVector[2]));
				this->n.push_back(this->vertex.normal);
			}
			else if(this->line == "f") //Kommer ha tillg�ng till alla vektorer n�r denna blir true
			{
				this->putStrInArr(3, file);
				
				this->newNum = 0;

				for(uint i = 0; i < 3; i++)
				{					
					this->en_vertex = toVector[i];
					
					for(uint j = 0; j < 3; j++)
					{
						this->found_delimiter = this->en_vertex.find('/', this->newNum);
						this->ett_index = this->en_vertex.substr(this->newNum, this->found_delimiter);
						this->vectorID[j] = atoi(this->ett_index.c_str());
						this->newNum = this->found_delimiter+1;
					}

					this->triangle.triangle[i].pos = p[this->vectorID[0]-1];
					this->triangle.triangle[i].tex = t[this->vectorID[1]-1];
					this->triangle.triangle[i].normal = n[this->vectorID[2]-1];
				}

				this->triList.push_back(this->triangle);
			}
		}
		file.close();
	}
	/*int size = this->triList.size(), bytes = 0, nrOfTriangles = 0, sizeOfTri = sizeof(triangle);

	for(int i = 0; i < size; i++)
	{
		bytes = sizeof(this->triList[i].triangle);
		nrOfTriangles += bytes/sizeOfTri;
	}

	cout << "Size of triList is: " << (float)nrOfTriangles*(float)sizeOfTri << " bytes\n";
	cout << "There are " << (float)nrOfTriangles << " triangles in triList\n";*/
}

float readOBJ::stringToFloat(string line)
{
	return this->num = (float)atof(line.c_str());
}

void readOBJ::putStrInArr(int i, ifstream &file)
{
	for(int j = 0; j < i; j++)
	{
		file >> this->line;
		this->toVector[j] = this->line;
	}
}