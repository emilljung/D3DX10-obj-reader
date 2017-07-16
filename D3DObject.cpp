#include "D3DObject.h"

// Written by Tommy Petersson

HWND D3DObject::mMainHwnd = NULL;
D3DObject* D3DObject::D3DInstance = NULL;

D3DObject::D3DObject ( HINSTANCE hInstance )
{
	mInstance = hInstance;
	D3DInstance = this;
}

D3DObject::~D3DObject ()
{
	mD3Device->ClearState();
	if ( mD3Device ) { mD3Device->Release(); }
	if ( mSwapChain ) { mSwapChain->Release(); }
	if ( mRenderTargetView ) { mRenderTargetView->Release(); }
	if ( mDepthStencilView ) { mDepthStencilView->Release(); }
}

bool D3DObject::initiate ( int screenWidth, int screenHeight )
{
	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	if ( !initiateWindow () )
	{
		return false;
	}
	if ( !initiateD3D () )
	{
		return false;
	}
	return true;
}

bool D3DObject::initiateWindow ()
{
	// Fill in the Window class structure
	WNDCLASS winClass;	
	winClass.style = CS_HREDRAW | CS_VREDRAW;
	winClass.lpfnWndProc = winProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = mInstance;
	winClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	winClass.hCursor = LoadCursor(0, IDC_ARROW);
	winClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	winClass.lpszMenuName = 0;
	winClass.lpszClassName = "MainWindow";

	// Try to register window class
	if ( !RegisterClass( &winClass ) )
	{
		MessageBox(0, "Failed to Register Window Class", 0, MB_ICONERROR );
		return false;
	}

	// Adjust Window to get Client area of right size
	RECT screenRect = { 0, 0, mScreenWidth, mScreenHeight };
	AdjustWindowRect(&screenRect, WS_OVERLAPPEDWINDOW, false );
	long windowWidth = screenRect.right - screenRect.left;
	long windowHeight = screenRect.bottom - screenRect.top;

	// Create the Window
	mMainHwnd = CreateWindow( "MainWindow", "Direct3D", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, 0, 0, mInstance, 0 );
	if ( !mMainHwnd )
	{
		MessageBox(0, "Failed to Create Window", 0, MB_ICONERROR );
		return false;
	}

	ShowWindow(mMainHwnd, SW_SHOW);
	UpdateWindow(mMainHwnd);

	return true;
}

bool D3DObject::initiateD3D()
{
	// Set Driver types
	// ----------------------------------------------------------------------
	D3D10_DRIVER_TYPE driverTypes [] =
	{
		D3D10_DRIVER_TYPE_HARDWARE,
		D3D10_DRIVER_TYPE_REFERENCE
	};
	int nrOfDriverTypes = sizeof(driverTypes)/sizeof(driverTypes[0]);
	// ----------------------------------------------------------------------

	// Fill in Swap Chain Description Structure
	// ----------------------------------------------------------------------
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof(swapChainDesc) );
	swapChainDesc.BufferDesc.Width = mScreenWidth;
	swapChainDesc.BufferDesc.Height = mScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	// No multisampling
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = mMainHwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// ----------------------------------------------------------------------

	// Check if Debug Mode
	// ----------------------------------------------------------------------
	int createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
	#endif
	// ----------------------------------------------------------------------

	// Create Device and Swap Chain
	// ----------------------------------------------------------------------
	HRESULT hr;
	for ( int i = 0; i < nrOfDriverTypes; i++ )
	{
		hr = D3D10CreateDeviceAndSwapChain( 0, driverTypes[i], 0, createDeviceFlags, D3D10_SDK_VERSION, &swapChainDesc, &mSwapChain, &mD3Device );
		if ( SUCCEEDED(hr) )
		{
			break;
		}
	}
	if ( FAILED(hr) )
	{
		MessageBox( 0, "Failed to Create Swap Chain or Device", 0, MB_ICONERROR );
		return false;
	}
	// ----------------------------------------------------------------------

	// Create Back Buffer and Render Target View
	// ----------------------------------------------------------------------
	ID3D10Texture2D* backBuffer;
	hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), (void**)&backBuffer );
	if ( FAILED(hr) )
	{
		MessageBox( 0, "Failed to Create Back Buffer", 0, MB_ICONERROR );
		return false;
	}
	hr = mD3Device->CreateRenderTargetView( backBuffer, 0, &mRenderTargetView );
	backBuffer->Release();
	if ( FAILED(hr) )
	{
		MessageBox( 0, "Failed to Create Render Target View", 0, MB_ICONERROR );
		return false;
	}
	// ----------------------------------------------------------------------

	// Create Depth/Stencil Texture
	// ----------------------------------------------------------------------
	D3D10_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = mScreenWidth;
	depthDesc.Height = mScreenHeight;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;

	// No multisampling, must match Back Buffer
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;

	depthDesc.Usage = D3D10_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	hr = mD3Device->CreateTexture2D( &depthDesc, 0, &mDepthStencilBuffer );
	if ( FAILED(hr) )
	{
		MessageBox( 0, "Failed to Create Depth Stencil Texture", 0, MB_ICONERROR );
		return false;
	}
	// ----------------------------------------------------------------------
	
	// Create Depth/Stencil View
	// ----------------------------------------------------------------------
	D3D10_DEPTH_STENCIL_VIEW_DESC depthViewDesc;
	depthViewDesc.Format = depthDesc.Format;
	depthViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	depthViewDesc.Texture2D.MipSlice = 0;

	hr = mD3Device->CreateDepthStencilView( mDepthStencilBuffer, &depthViewDesc, &mDepthStencilView );
	if ( FAILED(hr) )
	{
		MessageBox( 0, "Failed to Create Depth Stencil Texture", 0, MB_ICONERROR );
		return false;
	}
	// ----------------------------------------------------------------------

	// Set the Render Targets in the Output merger
	mD3Device->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

	// Setup the viewport
	// ----------------------------------------------------------------------
	D3D10_VIEWPORT viewPort;
	viewPort.Width = mScreenWidth;
	viewPort.Height = mScreenHeight;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	mD3Device->RSSetViewports( 1, &viewPort );
	// ----------------------------------------------------------------------

	return true;
}

int D3DObject::run ()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(msg));

	// Get Current Timestamp
	__int64 currentTime = 0;
	__int64 previousTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&previousTime);

	// Get Count Frequency
	__int64 countsPerSecond = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
	float secondsPerCount = 1 / (float)countsPerSecond;

	// To hold Elapsed time per frame value
	float deltaTime = 0;

	while(msg.message != WM_QUIT)
	{
		// Check for events
		if ( PeekMessage(&msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// Run Game Loop
		else
		{
			// Get current Timestamp
			QueryPerformanceCounter ((LARGE_INTEGER*)&currentTime);
			deltaTime = (currentTime-previousTime)*secondsPerCount;

			// Update Frame
			update ( deltaTime );

			// Render Frame
			render ();

			// Ready Time for next Frame
			previousTime = currentTime;
		}
	}
	return (int)msg.wParam;
}


long CALLBACK D3DObject::winProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch(msg)
	{
		case WM_KEYDOWN:
			if ( wParam == VK_ESCAPE )
			{
				DestroyWindow(mMainHwnd);
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			// Default Messages
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}