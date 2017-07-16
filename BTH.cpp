#include "BTH.h"

BTH::BTH()
{
	this->fileName = "bth.obj";
	this->rObj.readFile(fileName);
}

BTH::~BTH()
{}

void BTH::Init(ID3D10Device *device)
{
	//Create vertex buffer
	D3D10_BUFFER_DESC bd;
	bd.Usage = D3D10_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( Triangle ) * this->rObj.triList.size() ;
	bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;

	if( FAILED( device->CreateBuffer( &bd, 0, &this->g_BTH_Buffer ) ) )
		MessageBox( 0, "Unable to create Vertex Buffer", "VB Error", 0 );


	//Create shader
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY;
	ID3D10Blob* error = NULL;
	if(FAILED(D3DX10CreateEffectFromFile("BTH.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 
								0, device, NULL, NULL, &this->g_BTH_Effect, &error, NULL)))
	{
		MessageBox(0, "Error compiling shader!", "Shader error!", 0);
		if ( error )
		{
			MessageBox(0,((char*)error->GetBufferPointer()),0,0);
		}
	}

	//Get technique
	this->g_BTH_Tech = this->g_BTH_Effect->GetTechniqueByName("DrawBTH");

	//Start to create input layout by defining vertex data layout-----------------------------
	const D3D10_INPUT_ELEMENT_DESC lineVertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX" , 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D10_PASS_DESC PassDesc;
	this->g_BTH_Tech->GetPassByIndex(0)->GetDesc(&PassDesc); //Will go for 'pass p0' in BTH.fx

	device->CreateInputLayout(lineVertexLayout,
		sizeof(lineVertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC),
		PassDesc.pIAInputSignature,
		PassDesc.IAInputSignatureSize,
		&this->g_BTH_VertexLayout );
	//Input layout is now created-------------------------------------------------------------

	//read texturefile
	D3DX10CreateShaderResourceViewFromFile(device, "bthcolor.dds", NULL, NULL, &this->g_BTH_Texture, NULL);
	
	//send variable to shader (picture is now on GPU)
	this->g_BTH_Effect->GetVariableByName("BTHtexture")->AsShaderResource()->SetResource(this->g_BTH_Texture);

	//Set Pointlight-defaults in GPU
	this->g_BTH_Effect->GetVariableByName("light")->SetRawValue(&cam.pl, 0, sizeof(PointLight));

	//Map
	Vertex* p_VB = NULL;
	this->Map( (void**)&p_VB );

	memcpy( &p_VB[0], &this->rObj.triList[0], sizeof( Triangle ) * this->rObj.triList.size() );

	this->g_BTH_Buffer->Unmap();
}

void BTH::Map(void **b)
{
	this->g_BTH_Buffer->Map( D3D10_MAP_WRITE_DISCARD, 0, reinterpret_cast< void** >(b) );
}

HRESULT BTH::Render(ID3D10Device *device, ID3D10RenderTargetView *renderTargetView, ID3D10DepthStencilView *depthStencilView)
{
	//Camera-values
	D3DXVECTOR3 camPos = cam.getCamPos();
	D3DXMATRIX worldViewProj = cam.getWorldViewProjection();

	static float ko;
	ko++;
	D3DXMATRIX rotation;
	D3DXMatrixRotationY(&rotation, 0.0005f *ko);

	worldViewProj = rotation * worldViewProj;

	this->g_BTH_Effect->GetVariableByName( "g_mWorldViewProjection" )->AsMatrix()->SetMatrix( (float*)&worldViewProj );
	this->g_BTH_Effect->GetVariableByName( "g_WorldMatrix" )->AsMatrix()->SetMatrix( (float*)&rotation );
	this->g_BTH_Effect->GetVariableByName( "camPos" )->AsVector()->SetFloatVector( (float*)&D3DXVECTOR4(camPos,1.0f) );

	// Set Input Assembler params
	uint stride = sizeof(Vertex);
	uint offset = 0;
	device->IASetInputLayout( this->g_BTH_VertexLayout ); //g_pVertexLayout
	device->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	device->IASetVertexBuffers( 0, 1, &this->g_BTH_Buffer, &stride, &offset );

	static float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	
	//clear render target
	device->ClearRenderTargetView( renderTargetView, ClearColor );

	//clear depth info
	device->ClearDepthStencilView( depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0 );

	// Render line using the technique g_pRenderTextured
	D3D10_TECHNIQUE_DESC techDesc;
	this->g_BTH_Tech->GetDesc( &techDesc );
	for( uint p = 0; p < techDesc.Passes; p++ )
	{
		this->g_BTH_Tech->GetPassByIndex( p )->Apply(0);
		device->Draw(this->rObj.triList.size() * 3, 0);
	}

	return S_OK;

}