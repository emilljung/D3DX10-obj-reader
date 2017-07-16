#ifndef _D3DOBJECT_H__
#define _D3DOBJECT_H__
#include <D3D10.h>
#include <D3DX10.h>

// Written by Tommy Petersson

class D3DObject
{
	public:
		// Constructor
		D3DObject ( HINSTANCE hInstance );
		// Destructor
		virtual ~D3DObject ();

		// Game Loop
		// ------------------------------------------------
		// Updates the Game logic
		virtual void update ( float dt ) = 0;

		// Render scene to screen
		virtual void render () = 0;
		// ------------------------------------------------

	protected:
		// Initiate a Window with Direct3D
		bool initiate ( int screenWidth = 800, int screenHeight = 600 );

		// Starts the Game loop
		int run ();

		// Direct3D propeties
		// ------------------------------------------------
		static D3DObject *D3DInstance;
		ID3D10Device *mD3Device;
		IDXGISwapChain *mSwapChain;
		ID3D10RenderTargetView *mRenderTargetView;
		ID3D10Texture2D *mDepthStencilBuffer;
		ID3D10DepthStencilView *mDepthStencilView;
		// ------------------------------------------------

		int mScreenWidth;
		int mScreenHeight;

	private:
		// Helper funtions to initiate each api
		// ------------------------------------------------
		bool initiateWindow();
		bool initiateD3D ();
		// ------------------------------------------------

		// Window propeties
		// ------------------------------------------------
		HINSTANCE mInstance;
		static HWND mMainHwnd;
		static long CALLBACK winProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
		// ------------------------------------------------
};

#endif // _D3DOBJECT_H__