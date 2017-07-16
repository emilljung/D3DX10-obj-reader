#ifndef CAMERA_H
#define CAMERA_H

#include <D3D10.h>
#include <D3DX10.h>

#include "PointLight.h"
#include <iostream>

struct Camera
{
	D3DXVECTOR3 camPosition;
	
	D3DXMATRIX camView;
	D3DXMATRIX camProjection;
	D3DXMATRIX worldViewProjection;
	D3DXMATRIX worldMatrix;

	PointLight pl;
	int screenWidth, screenHeight;

	Camera();
	~Camera();

	D3DXMATRIX getWorldViewProjection();
	D3DXVECTOR3 getCamPos();
	
};

#endif