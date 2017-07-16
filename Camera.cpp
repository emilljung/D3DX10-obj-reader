#include "Camera.h"

Camera::Camera()
{
	this->camPosition = D3DXVECTOR3(0.0f, 0.0f, -110.0f);
	this->screenWidth = 800;	//Has the same values as screenWidth &
	this->screenHeight = 600;	//screenHeight in D3DObject

	this->pl.pos = D3DXVECTOR3(0.0f, 0.0f, -75.0f);
	this->pl.radius = 125.0f;
	this->pl.ambient = D3DXCOLOR(0.07f, 0.07f, 0.07f, 1.0f);
	this->pl.diffuse = D3DXCOLOR(1.0, 1.0, 1.0, 1.0f);
	this->pl.specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

Camera::~Camera()
{}

D3DXMATRIX Camera::getWorldViewProjection()
{
	//Camera is pointing at (0, 0, 0)
	D3DXMatrixLookAtLH(&this->camView, &this->camPosition, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMatrixPerspectiveLH(&this->camProjection,(float)(D3DX_PI * 0.6f) , (float)(this->screenWidth/this->screenHeight), 1.0f, 1000.0f);
	this->worldViewProjection = this->camView * this->camProjection;

	return this->worldViewProjection;
}

D3DXVECTOR3 Camera::getCamPos()
{
	return this->camPosition;
}