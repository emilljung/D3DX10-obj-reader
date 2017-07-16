#ifndef BTH_H
#define BTH_H

#include "readOBJ.h"
#include "Camera.h"

struct BTH
{
	readOBJ rObj;
	Camera cam;
	char* fileName;

	ID3D10Effect				*g_BTH_Effect;
	ID3D10EffectTechnique		*g_BTH_Tech;
	ID3D10ShaderResourceView	*g_BTH_Texture;
	ID3D10InputLayout			*g_BTH_VertexLayout;
	ID3D10Buffer				*g_BTH_Buffer;

	BTH();
	~BTH();

	void Init(ID3D10Device *device);
	void Map(void **b);

	HRESULT Render(ID3D10Device *device, ID3D10RenderTargetView *renderTargetView, ID3D10DepthStencilView *depthStencilView);

};

#endif