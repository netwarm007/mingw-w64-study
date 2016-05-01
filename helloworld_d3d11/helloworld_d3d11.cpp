// include the basic windows header file
#include <windows.h>
#include <windowsx.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dx9.h>
#include <d3dcompiler.h>

#include <tchar.h>

#define SCREEN_WIDTH	800
#define	SCREEN_HEIGHT	600

// global declarations
IDXGISwapChain *g_pSwapchain;             // the pointer to the swap chain interface
ID3D11Device *g_pDev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *g_pDevcon;           // the pointer to our Direct3D device context

ID3D11RenderTargetView *g_pRTView;

ID3D11InputLayout	*g_pLayout;			// the pointer to the input layout
ID3D11VertexShader	*g_pVS;				// the pointer to the vertex shader
ID3D11PixelShader	*g_pPS;				// the pointer to the pixel shader

ID3D11Buffer		*g_pVBuffer;

struct VERTEX {
	FLOAT X, Y, Z;		// position
	D3DXCOLOR Color;	// color
};

// function prototypes
HRESULT InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void CreateRenderTarget();
void SetViewPort();
void RenderFrame();
void InitPipeline();
void InitGraphics();
void CleanD3D();         // closes Direct3D and releases memory

LRESULT CALLBACK WindowProc(HWND hWnd,
                         UINT message,
                         WPARAM wParam,
                         LPARAM lParam);

// the entry point for any Windows program
int WINAPI _tWinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine,
                   int nCmdShow)
{
	HRESULT hr;
    // the handle for the window, filled by a function
    HWND hWnd;
    // this struct holds information for the window class
    WNDCLASSEX wc;

    // clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // fill in the struct with the needed information
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("WindowClass1");

    // register the window class
    RegisterClassEx(&wc);

    // create the window and use the result as the handle
    hWnd = CreateWindowEx(0,
                          _T("WindowClass1"),    // name of the window class
                          _T("Our First Windowed Program"),   // title of the window
                          WS_OVERLAPPEDWINDOW,    // window style
                          300,    // x-position of the window
                          300,    // y-position of the window
                          SCREEN_WIDTH,    // width of the window
                          SCREEN_HEIGHT,    // height of the window
                          NULL,    // we have no parent window, NULL
                          NULL,    // we aren't using menus, NULL
                          hInstance,    // application handle
                          NULL);    // used with multiple windows, NULL

	// Init D3D
	hr = InitD3D(hWnd);

    // display the window on the screen
    ShowWindow(hWnd, nCmdShow);

    // enter the main loop:

    // this struct holds Windows event messages
    MSG msg;

    while(TRUE)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if(msg.message == WM_QUIT)
                break;
        }

        RenderFrame();
    }

	// Clean D3D
	CleanD3D();

    // return this part of the WM_QUIT message to Windows
    return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // sort through and find what code to run for the message given
    switch(message)
    {
        // this message is read when the window is closed
        case WM_DESTROY:
            {
                // close the application entirely
                PostQuitMessage(0);
                return 0;
            } break;
    }

    // Handle any messages the switch statement didn't
    return DefWindowProc (hWnd, message, wParam, lParam);
}

// this function initializes and prepares Direct3D for use
HRESULT InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// allow full-screen switching

	const D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_1,
												D3D_FEATURE_LEVEL_11_0,
												D3D_FEATURE_LEVEL_10_1,
												D3D_FEATURE_LEVEL_10_0,
												D3D_FEATURE_LEVEL_9_3,
												D3D_FEATURE_LEVEL_9_2,
												D3D_FEATURE_LEVEL_9_1};
	D3D_FEATURE_LEVEL FeatureLevelSupported;

	HRESULT hr = S_OK;

    // create a device, device context and swap chain using the information in the scd struct
    hr = D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  0,
                                  FeatureLevels,
                                  _countof(FeatureLevels),
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &g_pSwapchain,
                                  &g_pDev,
                                  &FeatureLevelSupported,
                                  &g_pDevcon);

	if (hr == E_INVALIDARG) {
	    hr = D3D11CreateDeviceAndSwapChain(NULL,
	                                  D3D_DRIVER_TYPE_HARDWARE,
	                                  NULL,
	                                  0,
	                                  &FeatureLevelSupported,
	                                  1,
	                                  D3D11_SDK_VERSION,
	                                  &scd,
	                                  &g_pSwapchain,
	                                  &g_pDev,
	                                  NULL,
	                                  &g_pDevcon);
	}

	if (hr == S_OK) {
		CreateRenderTarget();
		SetViewPort();
		InitPipeline();
		InitGraphics();
	}

	return hr;
}

void CreateRenderTarget() {
	HRESULT hr;
	ID3D11Texture2D *pBackBuffer;

	// Get a pointer to the back buffer
	g_pSwapchain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), 
	                             ( LPVOID* )&pBackBuffer );
	
	// Create a render-target view
	g_pDev->CreateRenderTargetView( pBackBuffer, NULL,
	                                      &g_pRTView );
	pBackBuffer->Release();
	
	// Bind the view
	g_pDevcon->OMSetRenderTargets( 1, &g_pRTView, NULL );
}

void SetViewPort() {
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	g_pDevcon->RSSetViewports(1, &viewport);
}

// this is the function used to render a single frame
void RenderFrame()
{
    // clear the back buffer to a deep blue
    g_pDevcon->ClearRenderTargetView(g_pRTView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    // do 3D rendering on the back buffer here
	{
        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        g_pDevcon->IASetVertexBuffers(0, 1, &g_pVBuffer, &stride, &offset);

        // select which primtive type we are using
        g_pDevcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // draw the vertex buffer to the back buffer
        g_pDevcon->Draw(3, 0);
	}


    // switch the back buffer and the front buffer
    g_pSwapchain->Present(0, 0);
}

// this is the function that loads and prepares the shaders
void InitPipeline() {
	// load and compile the two shaders
	ID3DBlob *VS, *PS;
#if 0 // deprecated, not work at mingw64
	D3DX11CompileFromFile(_T("shaders.shader"), 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(_T("shaders.shader"), 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);
#else
	D3DReadFileToBlob(L"vs.fxo", &VS);
	D3DReadFileToBlob(L"ps.fxo", &PS);
#endif

	// encapsulate both shaders into shader objects
	g_pDev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &g_pVS);
	g_pDev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &g_pPS);

	// set the shader objects
	g_pDevcon->VSSetShader(g_pVS, 0, 0);
	g_pDevcon->PSSetShader(g_pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    g_pDev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &g_pLayout);
    g_pDevcon->IASetInputLayout(g_pLayout);

	VS->Release();
	PS->Release();
}

// this is the function that creates the shape to render
void InitGraphics() {
    // create a triangle using the VERTEX struct
    VERTEX OurVertices[] =
    {
        {0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
        {0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
        {-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
    };


    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    g_pDev->CreateBuffer(&bd, NULL, &g_pVBuffer);       // create the buffer

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    g_pDevcon->Map(g_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    g_pDevcon->Unmap(g_pVBuffer, NULL);                                      // unmap the buffer
}

// this is the function that cleans up Direct3D and COM
void CleanD3D()
{
	g_pSwapchain->SetFullscreenState(FALSE, NULL);	// switch to windowed mode

    // close and release all existing COM objects
	g_pLayout->Release();
	g_pVS->Release();
	g_pPS->Release();
	g_pVBuffer->Release();
	g_pSwapchain->Release();
	g_pRTView->Release();
    g_pDev->Release();
    g_pDevcon->Release();
}

