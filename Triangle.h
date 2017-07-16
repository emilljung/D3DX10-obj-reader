#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <vector>
#include "Vertex.h"


struct Triangle
{
	Vertex triangle[3];

	Triangle()
	{
		for(int i = 0; i < 3; i++)
		{
			this->triangle[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			this->triangle[i].tex = D3DXVECTOR2(0.0f, 0.0f);
			this->triangle[i].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
};

#endif