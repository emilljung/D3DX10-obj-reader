#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include <D3D10.h>
#include <D3DX10.h>

#include <iostream>
using namespace std;

struct PointLight
{
	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXVECTOR3 pos;
	float radius;
};

#endif