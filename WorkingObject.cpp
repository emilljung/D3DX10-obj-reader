#include "WorkingObject.h"

// Written by Tommy Petersson

WorkingObject::WorkingObject ( HINSTANCE hInstance )
	: D3DObject ( hInstance )
{
}

WorkingObject::~WorkingObject ()
{
}

bool WorkingObject::initiateAndRun()
{
	if ( !initiate() ) return false;

	this->bth.Init(mD3Device);

	run();
	return true;
}

void WorkingObject::update( float dt )
{
	// Calculate here
}

void WorkingObject::render()
{
	// Clear Previous Frame
	// -------------------------------------------------------------------
	static float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	mD3Device->ClearRenderTargetView( mRenderTargetView, clearColor );
	mD3Device->ClearDepthStencilView( mDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0 );
	// -------------------------------------------------------------------

	// Render scene here
	this->bth.Render(mD3Device, mRenderTargetView, mDepthStencilView);

	// Swap Buffers
	// -------------------------------------------------------------------
	if ( FAILED(mSwapChain->Present( 0, 0 )) )
	{
		MessageBox( 0, "Failed To Present Scene", 0, 0 );
	}
	// -------------------------------------------------------------------
}