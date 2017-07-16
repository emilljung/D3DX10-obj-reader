#ifndef VERTEX_H
#define VERTEX_H

#include <D3D10.h>
#include <D3DX10.h>

#include <iostream>
using namespace std;

struct Vertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex;

	Vertex()
	{
		this->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		this->tex = D3DXVECTOR2(0.0f, 0.0f);
	}
};

#endif