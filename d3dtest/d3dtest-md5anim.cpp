#include "stdafx.h"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#pragma comment(lib, "D3D10_1.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "dwrite.lib")
///////////////**************new**************////////////////////
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
///////////////**************new**************////////////////////

#include <windows.h>
#include "Resource.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <D3D10_1.h>
#include <DXGI.h>
#include <D2D1.h>
#include <sstream>
#include <dwrite.h>
///////////////**************new**************////////////////////
#include <dinput.h>
///////////////**************new**************////////////////////
///////////////**************new**************////////////////////
#include <vector>
#include <fstream>
#include <istream>
///////////////**************new**************////////////////////
//全局描述符
IDXGISwapChain* SwapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DevCon;
ID3D11RenderTargetView* renderTargetView;

//索引缓冲
//ID3D11Buffer* squareIndexBuffer;

//深度值-20170927
ID3D11DepthStencilView* depthStencilView;
ID3D11Texture2D* depthStencilBuffer;

//着色器
//ID3D11Buffer* squareVertBuffer;
ID3D11VertexShader* VS;
ID3D11PixelShader* PS;
ID3D11PixelShader* D2D_PS;
ID3D10Blob* D2D_PS_Buffer;
ID3D10Blob* VS_Buffer;
ID3D10Blob* PS_Buffer;
ID3D11InputLayout* vertLayout;

///
ID3D11Buffer* cbPerObjectBuffer;
ID3D11BlendState* d2dTransparency;
ID3D11RasterizerState* CCWcullMode;
ID3D11RasterizerState* CWcullMode;
//ID3D11ShaderResourceView* CubesTexture;
ID3D11SamplerState* CubesTexSamplerState;

ID3D11Buffer* cbPerFrameBuffer;

ID3D10Device1 *d3d101Device;
IDXGIKeyedMutex *keyedMutex11;
IDXGIKeyedMutex *keyedMutex10;
ID2D1RenderTarget *D2DRenderTarget;
ID2D1SolidColorBrush *Brush;
ID3D11Texture2D *BackBuffer11;
ID3D11Texture2D *sharedTex11;
ID3D11Buffer *d2dVertBuffer;
ID3D11Buffer *d2dIndexBuffer;
ID3D11ShaderResourceView *d2dTexture;
IDWriteFactory *DWriteFactory;
IDWriteTextFormat *TextFormat;
///////////////**************new**************////////////////////
IDirectInputDevice8* DIKeyboard;
IDirectInputDevice8* DIMouse;
///////////////**************new**************////////////////////
ID3D11Buffer* sphereIndexBuffer;
ID3D11Buffer* sphereVertBuffer;

ID3D11VertexShader* SKYMAP_VS;
ID3D11PixelShader* SKYMAP_PS;
ID3D10Blob* SKYMAP_VS_Buffer;
ID3D10Blob* SKYMAP_PS_Buffer;

ID3D11ShaderResourceView* smrv;

ID3D11DepthStencilState* DSLessEqual;
ID3D11RasterizerState* RSCullNone;
///////////////**************new**************////////////////////
ID3D11BlendState* Transparency;
//网格变量，每个被加载的网格需要它自己的集
ID3D11Buffer* meshVertBuff;
ID3D11Buffer* meshIndexBuff;
XMMATRIX meshWorld;
int meshSubsets = 0;
std::vector<int> meshSubsetIndexStart;
std::vector<int> meshSubsetTexture;


//纹理和材质变量，用于所有的网格的加载
std::vector<ID3D11ShaderResourceView*> meshSRV;
std::vector<std::wstring> textureNameArray;
///////////////**************new**************////////////////////
std::wstring printText;
/////
LPCTSTR WndClassName = L"firstwindow";
HWND hwnd = NULL;
HRESULT hr;

const int Width = 1920; //设置宽
const int Height = 1200; // 设置高

///////////////**************new**************////////////////////
DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

float rotx = 0;
float rotz = 0;
float scaleX = 1.0f;
float scaleY = 1.0f;

XMMATRIX Rotationx;
//XMMATRIX Rotationy;
XMMATRIX Rotationz;
XMMATRIX Rotationy;
///////////////**************new**************////////////////////
///四个空间以及相机属性
XMMATRIX WVP;
//立方体
//XMMATRIX cube1World;
//XMMATRIX cube2World;
//
//XMMATRIX World;
XMMATRIX camView;
XMMATRIX camProjection;

XMMATRIX d2dWorld;
XMVECTOR camPosition;
XMVECTOR camTarget;
XMVECTOR camUp;
///////////////**************new**************////////////////////
XMVECTOR DefaultForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
XMVECTOR DefaultRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);
XMVECTOR camForward = XMVectorSet(0.0f,0.0f,1.0f, 0.0f);
XMVECTOR camRight = XMVectorSet(1.0f,0.0f,0.0f, 0.0f);

XMMATRIX camRotationMatrix;
//XMMATRIX groundWorld;

float moveLeftRight = 0.0f;
float moveBackForward = 0.0f;

float camYaw = 0.0f;
float camPitch = 0.0f;
///////////////**************new**************////////////////////
int NumSphereVertices;
int NumSphereFaces;

XMMATRIX sphereWorld;
///////////////**************new**************////////////////////
XMMATRIX Rotation;
XMMATRIX Scale;
XMMATRIX Translation;

float rot = 0.01f;

///////////////**************new**************////////////////////
double countsPerSecond = 0.0;
__int64 CounterStart = 0;

int frameCount = 0;
int fps = 0;

__int64 frameTimeOld = 0;
double frameTime;
///////////////**************new**************////////////////////
//Function Prototypes//
bool InitializeDirect3d11App(HINSTANCE hInstance);
void CleanUp();
bool InitScene();
void DrawScene();
bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter);
void InitD2DScreenTexture();
///////////////**************new**************////////////////////
void UpdateScene(double time);
///////////////**************new**************////////////////////
void UpdateCamera();

void RenderText(std::wstring text, int inInt);
//void RenderText(std::wstring text);

void StartTimer();
double GetTime();
double GetFrameTime();


// 初始化窗口
bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed);

//初始化消息循环函数
int messageloop();
//初始化窗口回调过程。Windows API是事件驱动型的编程模型。在该函数中捕获Windows消息，比如一个按键按下（也叫事件）以及程序操作流程。
///////////////**************new**************////////////////////
bool InitDirectInput(HINSTANCE hInstance);
void DetectInput(double time);
///////////////**************new**************////////////////////
void CreateSphere(int LatLines, int LongLines);

LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

///new
//创建效果常量缓冲的结构体
struct cbPerObject
{
	XMMATRIX WVP;
	XMMATRIX World;

	///////////////**************new**************////////////////////
	//用于像素着色器
	XMFLOAT4 difColor;
	BOOL hasTexture;
	///////////////**************new**************////////////////////
	//Because of HLSL structure packing, we will use windows BOOL
	//instead of bool because HLSL packs things into 4 bytes, and
	//bool is only one byte, where BOOL is 4 bytes
	BOOL hasNormMap;
	///////////////**************new**************////////////////////
};

cbPerObject cbPerObj;

///////////////**************new**************////////////////////
//创建材质结构体
struct SurfaceMaterial
{
	std::wstring matName;
	XMFLOAT4 difColor;
	int texArrayIndex;
	///////////////**************new**************////////////////////
	int normMapTexArrayIndex;
	bool hasNormMap;
	///////////////**************new**************////////////////////
	bool hasTexture;
	bool transparent;
};

std::vector<SurfaceMaterial> material;

//自创建surfaceMaterial结构体后，定义函数LoadObjModel
bool LoadObjModel(std::wstring filename,			//.obj filename
	ID3D11Buffer** vertBuff,					//mesh vertex buffer
	ID3D11Buffer** indexBuff,					//mesh index buffer
	std::vector<int>& subsetIndexStart,			//start index of each subset
	std::vector<int>& subsetMaterialArray,		//index value of material for each subset
	std::vector<SurfaceMaterial>& material,		//vector of material structures
	int& subsetCount,							//Number of subsets in mesh
	bool isRHCoordSys,							//true if model was created in right hand coord system
    bool computeNormals);                        //true to compute the normals, false to use the files normals

///////////////**************new**************////////////////////
struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 dir;
	float cone;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;

};
Light light;

struct cbPerFrame
{
	Light light;
};

cbPerFrame constbuffPerFrame;


//顶点结构体以及顶点布局（输入布局）

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z,
		float u, float v,
		float nx, float ny, float nz,
		float tx, float ty, float tz)
		: pos(x,y,z), texCoord(u, v), normal(nx, ny, nz),
		tangent(tx, ty, tz){}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	///////////////**************new**************////////////////////
	XMFLOAT3 tangent;
	XMFLOAT3 biTangent;
///////////////**************new**************////////////////////
    // 不会传入着色器中
    int StartWeight;
    int WeightCount;
///////////////**************new**************////////////////////
};

D3D11_INPUT_ELEMENT_DESC layout[] =
{
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",	 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0},
	///////////////**************new**************////////////////////
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0}
	///////////////**************new**************////////////////////
};
UINT numElements = ARRAYSIZE(layout);

///////////////**************new**************////////////////////
struct Joint
{
    std::wstring name;
    int parentID;

    XMFLOAT3 pos;
    XMFLOAT4 orientation;
};

///////////////**************new**************////////////////////
struct BoundingBox
{
    XMFLOAT3 min;
    XMFLOAT3 max;
};

struct FrameData
{
    int frameID;
    std::vector<float> frameData;
};
struct AnimJointInfo
{
    std::wstring name;
    int parentID;

    int flags;
    int startIndex;
};

struct ModelAnimation
{
    int numFrames;
    int numJoints;
    int frameRate;
    int numAnimatedComponents;

    float frameTime;
    float totalAnimTime;
    float currAnimTime;

    std::vector<AnimJointInfo> jointInfo;
    std::vector<BoundingBox> frameBounds;
    std::vector<Joint>    baseFrameJoints;
    std::vector<FrameData>    frameData;
    std::vector<std::vector<Joint>> frameSkeleton;
};
///////////////**************new**************////////////////////

struct Weight
{
    int jointID;
    float bias;
    XMFLOAT3 pos;
    ///////////////**************new**************////////////////////
    XMFLOAT3 normal;
    ///////////////**************new**************////////////////////
};

struct ModelSubset
{
    int texArrayIndex;
    int numTriangles;

    std::vector<Vertex> vertices;
    std::vector<XMFLOAT3> jointSpaceNormals;
    std::vector<DWORD> indices;
    std::vector<Weight> weights;

    std::vector<XMFLOAT3> positions;

    ID3D11Buffer* vertBuff; 
    ID3D11Buffer* indexBuff;
};

struct Model3D
{
    int numSubsets;
    int numJoints;

    std::vector<Joint> joints;
    std::vector<ModelSubset> subsets;
    ///////////////**************new**************////////////////////
    std::vector<ModelAnimation> animations;
    ///////////////**************new**************////////////////////
};

XMMATRIX smilesWorld;
Model3D NewMD5Model;

//LoadMD5Model() function prototype
bool LoadMD5Model(std::wstring filename,
    Model3D& MD5Model,
    std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
    std::vector<std::wstring> texFileNameArray);
///////////////**************new**************////////////////////
bool LoadMD5Anim(std::wstring filename,    Model3D& MD5Model);

void UpdateMD5Model(Model3D& MD5Model, float deltaTime, int animation);
//主函数，传入应用程序句柄hInstance,前一个应用程序句柄hPrevInstance，传给函数处理的命令行lpCmdLine以及窗口显示方式的nShowCmd
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{
	//创建并注册窗口
	if (!InitializeWindow(hInstance, nShowCmd, Width, Height, true))
	{
		MessageBox(0, L"Window Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	/////new
	if (!InitializeDirect3d11App(hInstance)) // 初始化D3D
	{
		MessageBox(0, L"Direct3D Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	if(!InitScene())	//Initialize our scene
	{
		MessageBox(0, L"Scene Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}

	///////////////**************new**************////////////////////
	if(!InitDirectInput(hInstance))
	{
		MessageBox(0, L"Direct Input Initialization - Failed",
			L"Error", MB_OK);
		return 0;
	}
	///////////////**************new**************////////////////////

	messageloop();
	CleanUp();
	//ReleaseObjects();

	return 0;
}
// windowed 若为true则为窗口模式显示，若为false则为全屏模式显示
bool InitializeWindow(HINSTANCE hInstance,
	int ShowWnd,
	int width, int height,
	bool windowed)
{
    typedef struct _WNDCLASS {
        UINT cbSize;
        UINT style;
        WNDPROC lpfnWndProc;
        int cbClsExtra;
        int cbWndExtra;
        HANDLE hInstance;
        HICON hIcon;
        HCURSOR hCursor;
        HBRUSH hbrBackground;
        LPCTSTR lpszMenuName;
        LPCTSTR lpszClassName;
    } WNDCLASS;

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX); //window类的大小
	/********windows类风格
	*CS_CLASSDC 一个使用该类创建的在所有窗口间共享的设备上下文
	*CS_DBLCLKS 在窗口上使能双击功能
	*CS_HREDRAW 若窗口的宽度有改变或者窗口水平地移动，窗口将会刷新
	*CS_NOCLOSE 窗口菜单上禁止关闭选项
	*CS_OWNDC   为每个窗口创建自己的设备上下文。正好与CS_CLASSDC相反
	*CS_PARENTDC 这会设置创建的子窗口的剪裁四边形到父窗口，这允许子窗口能够在父窗口上绘画
	*CS_VERDRAW 若在窗口的高度或窗口在垂直方向有移动窗口会重绘
	**/
	wc.style = CS_HREDRAW | CS_VREDRAW;
	//lpfnWndProc是一个指向处理窗口消息函数的指针，设置窗口处理函数的函数名WndProc
	wc.lpfnWndProc = WndProc;
	//cbClsExtra是WNDCLASSEX之后额外申请的字节数
	wc.cbClsExtra = NULL;
	//cbWndExtra指定窗口实例之后所申请的字节数
	wc.cbWndExtra = NULL;
	//当前窗口应用程序的句柄，通过给函数GetModuleHandle()函数第一个参数传入NULL可获取当前窗口应用程序。
	wc.hInstance = hInstance;

	//hIcon用来指定窗口标题栏左上角的图标。以下是一些标准图标：
	/*
	*IDI_APPLICATION 默认应用程序图标
	*IDI_HAND 手形状的图标
	*IDI_EXCLAMATION 感叹号图标
	*IDI_INFORMATION 星号图标
	*IDI_QUESTION 问号图标
	*IDI_WINLOGO 若使用的是XP则是默认应用程序图标，否则是窗口logo
	*/
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);

	/*定义光标图标
	*IDC_APPSTARTING 标准箭头以及小型沙漏光标
	*IDC_ARROW 标准箭头光标
	*IDC_CROSS 十字线光标
	*IDC_HAND 手型光标
	*IDC_NO 斜线圈光标
	*IDC_WAIT 沙漏光标
	*/
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//hbrBackground是一个刷子的句柄，可使得背景黑色。
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//附加到窗口的菜单名字，不需要的话设置为NULL
	wc.lpszMenuName = NULL;
	//对类进行命名
	wc.lpszClassName = WndClassName;
	//指定任务栏的图标，使用上面的IDI＿图标
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	//注册类。若失败则会获得一个错误，若成功，则继续创建窗口
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
			L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}
	//创建窗口
	hwnd = CreateWindowEx(
		NULL, 
		WndClassName, 
		L"model md5anim",
		WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
		NULL,
		NULL,
		hInstance,
		NULL
		);

	if (!hwnd)
    {
        MessageBox(NULL, L"Error creating window",
            L"Error", MB_OK | MB_ICONERROR);
        return 1;
	}

	//BOOL ShowWindow(HWND hWnd, int nCmdShow);
	//BOOL UpdateWindow(HWND hWnd);

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);// 发送WM_PAINT消息到窗口过程，若窗口客户区没有任何东西要显示，则不发送消息。返回true，继续运行到mainloop中去。

	return true;
}

bool InitializeDirect3d11App(HINSTANCE hInstance)
{
	//声明缓冲
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = Width;
	bufferDesc.Height = Height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//声明交换链
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = hwnd;
	///////////////**************new**************////////////////////
	swapChainDesc.Windowed = true; 
	///////////////**************new**************////////////////////
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//创建DXGI factory来枚举显卡
	IDXGIFactory1 *DXGIFactory;
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void **)&DXGIFactory);

	//使用第一个显卡
	IDXGIAdapter1 *Adapter;
	hr = DXGIFactory->EnumAdapters1(0, &Adapter);
	DXGIFactory->Release();

	//创建D3D11设备和交换链
	//hr = D3D11C

	//创建交换链
	D3D11CreateDeviceAndSwapChain(Adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, D3D11_CREATE_DEVICE_BGRA_SUPPORT, 
	NULL, NULL,	D3D11_SDK_VERSION, &swapChainDesc, &SwapChain, &d3d11Device, NULL, &d3d11DevCon);

	//初始化D2D D3D10.1和DirectWrite
	InitD2D_D3D101_DWrite(Adapter);

	//释放Adapter接口
	Adapter->Release();

	//创建后缓冲
	ID3D11Texture2D* BackBuffer;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	//创建渲染目标
	d3d11Device->CreateRenderTargetView(BackBuffer, NULL, &renderTargetView);
	BackBuffer->Release();

	//创建深度模板缓冲
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = Width;
	depthStencilDesc.Height = Height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL; //绑定到OM
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//创建深度模板视图
	d3d11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	d3d11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);


	return true;
}

bool InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter)
{
	//创建D3D101设备
	hr = D3D10CreateDevice1(Adapter, D3D10_DRIVER_TYPE_HARDWARE, NULL, D3D10_CREATE_DEVICE_BGRA_SUPPORT,
		D3D10_FEATURE_LEVEL_9_3, D3D10_1_SDK_VERSION, &d3d101Device);

	//创建共享纹理，D3D101将会渲染它
	D3D11_TEXTURE2D_DESC sharedTexDesc;
	ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	sharedTexDesc.Width = Width;
	sharedTexDesc.Height = Height;
	sharedTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;// DXGI_FORMAT_R8G8B8A8_UNORM;// DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels = 1;
	sharedTexDesc.ArraySize = 1;
	sharedTexDesc.SampleDesc.Count = 1;
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;

	hr = d3d11Device->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11);
	
	//为共享纹理获取key互斥量（为D3D11）
	hr = sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void **)&keyedMutex11);

	//获取共享句柄需要在D3D10.1中打开共享纹理
	IDXGIResource *sharedResource10;
	HANDLE sharedHandle10;

	hr = sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void **)&sharedResource10);
	hr = sharedResource10->GetSharedHandle(&sharedHandle10);
	sharedResource10->Release();

	//在D3D10.1中为共享纹理打开界面
	IDXGISurface1 *sharedSurface10;
	hr = d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void **)(&sharedSurface10));
	hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void **)&keyedMutex10);

	//创建D2D factory
	ID2D1Factory *D2DFactory;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void **)&D2DFactory);
	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget);

	sharedSurface10->Release();
	D2DFactory->Release();

	//创建立体彩色画笔绘制一些东西
	hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f), &Brush);

	//DirectWrite
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), 
	reinterpret_cast<IUnknown **>(&DWriteFactory));
	hr = DWriteFactory->CreateTextFormat(
		L"Script",
		NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		24.0f,
		L"en-us",
        &TextFormat
		);
    hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);    
	return true;
}

///////////////**************new**************////////////////////
bool InitDirectInput(HINSTANCE hInstance)
{
    hr = DirectInput8Create(hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)&DirectInput,
        NULL); 

    hr = DirectInput->CreateDevice(GUID_SysKeyboard,
        &DIKeyboard,
        NULL);

    hr = DirectInput->CreateDevice(GUID_SysMouse,
        &DIMouse,
        NULL);

    hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
    hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

    hr = DIMouse->SetDataFormat(&c_dfDIMouse);
    hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

    return true;
}

void UpdateCamera()
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget = XMVector3TransformCoord(DefaultForward, camRotationMatrix );
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

    // 步行
    //camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
    //camUp = XMVector3TransformCoord(camUp, RotateYTempMatrix);
    //camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

    // 自由相机
    camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
    camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
    camUp = XMVector3Cross(camForward, camRight);
	camPosition += moveLeftRight*camRight;
	camPosition += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;	

	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );
}
void DetectInput(double time)
{
    DIMOUSESTATE mouseCurrState;

    BYTE keyboardState[256];

    DIKeyboard->Acquire();
    DIMouse->Acquire();

    DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

    DIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

    if(keyboardState[DIK_ESCAPE] & 0x80)
        PostMessage(hwnd, WM_DESTROY, 0, 0);

	float speed = 10.0f * time;

	if(keyboardState[DIK_A] & 0x80)
	{
		moveLeftRight -= speed;
	}
	if(keyboardState[DIK_D] & 0x80)
	{
		moveLeftRight += speed;
	}
	if(keyboardState[DIK_W] & 0x80)
	{
		moveBackForward += speed;
	}
	if(keyboardState[DIK_S] & 0x80)
	{
		moveBackForward -= speed;
	}
    ///////////////**************new**************////////////////////
    if(keyboardState[DIK_R] & 0X80)
    {
        float timeFactor = 1.0f;    // 可通过改变该值来加速或减速时间
        UpdateMD5Model(NewMD5Model, time*timeFactor, 0);
    }
    ///////////////**************new**************////////////////////
	if((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		camYaw += mouseLastState.lX * 0.001f;

		camPitch += mouseCurrState.lY * 0.001f;

		mouseLastState = mouseCurrState;
	}

	UpdateCamera();


    return;
}


void CleanUp()
{
	///////////////**************new**************////////////////////
	SwapChain->SetFullscreenState(false, NULL);
	PostMessage(hwnd, WM_DESTROY, 0, 0);
	///////////////**************new**************////////////////////
	SwapChain->Release();
	d3d11Device->Release();
	d3d11DevCon->Release();
	renderTargetView->Release();

	//squareVertBuffer->Release();
	//squareIndexBuffer->Release();

	//triangleVertBuffer->Release();
	VS->Release();
	PS->Release();
	VS_Buffer->Release();
	PS_Buffer->Release();
	vertLayout->Release();

	depthStencilView->Release();
	depthStencilBuffer->Release();

	//
	cbPerObjectBuffer->Release();

	//释放不裁剪对象
//	noCull->Release();
	//释放混合对象
//#if 1
	Transparency->Release();
	CCWcullMode->Release();
	CWcullMode->Release();
//#endif	
	//释放线框
	//WireFrame->Release();

	d3d101Device->Release();
	keyedMutex11->Release();
	keyedMutex10->Release();
	D2DRenderTarget->Release();
	Brush->Release();
//	BackBuffer11->Release();
	sharedTex11->Release();
	DWriteFactory->Release();
    TextFormat->Release();
	d2dTexture->Release();

	/// new
	cbPerFrameBuffer->Release();
    ///////////////**************new**************////////////////////
    DIKeyboard->Unacquire();
    DIMouse->Unacquire();
    DirectInput->Release();
	sphereIndexBuffer->Release();
	sphereVertBuffer->Release();

	SKYMAP_VS->Release();
	SKYMAP_PS->Release();
	SKYMAP_VS_Buffer->Release();
	SKYMAP_PS_Buffer->Release();

	smrv->Release();

	DSLessEqual->Release();
	RSCullNone->Release();

	///////////////**************new**************////////////////////
	meshVertBuff->Release();
	meshIndexBuff->Release();
    ///////////////**************new**************////////////////////
    for(int i = 0; i < NewMD5Model.numSubsets; i++)
    {
        NewMD5Model.subsets[i].indexBuff->Release();
        NewMD5Model.subsets[i].vertBuff->Release();
    }
}

///////////////**************new**************////////////////////
bool LoadMD5Anim(std::wstring filename,    Model3D& MD5Model)
{    
    ModelAnimation tempAnim;                        // Temp animation to later store in our model's animation array

    std::wifstream fileIn (filename.c_str());        // Open file

    std::wstring checkString;                        // Stores the next string from our file

    if(fileIn)                                        // Check if the file was opened
    {
        while(fileIn)                                // Loop until the end of the file is reached
        {    
            fileIn >> checkString;                    // Get next string from file

            if ( checkString == L"MD5Version" )        // Get MD5 version (this function supports version 10)
            {
                fileIn >> checkString;
                /*MessageBox(0, checkString.c_str(),    //display message
                L"MD5Version", MB_OK);*/
            }
            else if ( checkString == L"commandline" )
            {
                std::getline(fileIn, checkString);    // Ignore the rest of this line
            }
            else if ( checkString == L"numFrames" )
            {
                fileIn >> tempAnim.numFrames;                // Store number of frames in this animation
            }
            else if ( checkString == L"numJoints" )
            {
                fileIn >> tempAnim.numJoints;                // Store number of joints (must match .md5mesh)
            }
            else if ( checkString == L"frameRate" )
            {
                fileIn >> tempAnim.frameRate;                // Store animation's frame rate (frames per second)
            }
            else if ( checkString == L"numAnimatedComponents" )
            {
                fileIn >> tempAnim.numAnimatedComponents;    // Number of components in each frame section
            }
            else if ( checkString == L"hierarchy" )
            {
                fileIn >> checkString;                // Skip opening bracket "{"

                for(int i = 0; i < tempAnim.numJoints; i++)    // Load in each joint
                {
                    AnimJointInfo tempJoint;

                    fileIn >> tempJoint.name;        // Get joints name
                    // Sometimes the names might contain spaces. If that is the case, we need to continue
                    // to read the name until we get to the closing " (quotation marks)
                    if(tempJoint.name[tempJoint.name.size()-1] != '"')
                    {
                        wchar_t checkChar;
                        bool jointNameFound = false;
                        while(!jointNameFound)
                        {
                            checkChar = fileIn.get();

                            if(checkChar == '"')
                                jointNameFound = true;        

                            tempJoint.name += checkChar;                                                            
                        }
                    }

                    // Remove the quotation marks from joints name
                    tempJoint.name.erase(0, 1);
                    tempJoint.name.erase(tempJoint.name.size()-1, 1);

                    fileIn >> tempJoint.parentID;            // Get joints parent ID
                    fileIn >> tempJoint.flags;                // Get flags
                    fileIn >> tempJoint.startIndex;            // Get joints start index

                    // Make sure the joint exists in the model, and the parent ID's match up
                    // because the bind pose (md5mesh) joint hierarchy and the animations (md5anim)
                    // joint hierarchy must match up
                    bool jointMatchFound = false;
                    for(int k = 0; k < MD5Model.numJoints; k++)
                    {
                        if(MD5Model.joints[k].name == tempJoint.name)
                        {
                            if(MD5Model.joints[k].parentID == tempJoint.parentID)
                            {
                                jointMatchFound = true;
                                tempAnim.jointInfo.push_back(tempJoint);
                            }
                        }
                    }
                    if(!jointMatchFound)                    // If the skeleton system does not match up, return false
                        return false;                        // You might want to add an error message here

                    std::getline(fileIn, checkString);        // Skip rest of this line
                }
            }
            else if ( checkString == L"bounds" )            // Load in the AABB for each animation
            {
                fileIn >> checkString;                        // Skip opening bracket "{"

                for(int i = 0; i < tempAnim.numFrames; i++)
                {
                    BoundingBox tempBB;

                    fileIn >> checkString;                    // Skip "("
                    fileIn >> tempBB.min.x >> tempBB.min.z >> tempBB.min.y;
                    fileIn >> checkString >> checkString;    // Skip ") ("
                    fileIn >> tempBB.max.x >> tempBB.max.z >> tempBB.max.y;
                    fileIn >> checkString;                    // Skip ")"

                    tempAnim.frameBounds.push_back(tempBB);
                }
            }            
            else if ( checkString == L"baseframe" )            // This is the default position for the animation
            {                                                // All frames will build their skeletons off this
                fileIn >> checkString;                        // Skip opening bracket "{"

                for(int i = 0; i < tempAnim.numJoints; i++)
                {
                    Joint tempBFJ;

                    fileIn >> checkString;                        // Skip "("
                    fileIn >> tempBFJ.pos.x >> tempBFJ.pos.z >> tempBFJ.pos.y;
                    fileIn >> checkString >> checkString;        // Skip ") ("
                    fileIn >> tempBFJ.orientation.x >> tempBFJ.orientation.z >> tempBFJ.orientation.y;
                    fileIn >> checkString;                        // Skip ")"

                    tempAnim.baseFrameJoints.push_back(tempBFJ);
                }
            }
            else if ( checkString == L"frame" )        // Load in each frames skeleton (the parts of each joint that changed from the base frame)
            {
                FrameData tempFrame;

                fileIn >> tempFrame.frameID;        // Get the frame ID

                fileIn >> checkString;                // Skip opening bracket "{"

                for(int i = 0; i < tempAnim.numAnimatedComponents; i++)
                {
                    float tempData;
                    fileIn >> tempData;                // Get the data

                    tempFrame.frameData.push_back(tempData);
                }

                tempAnim.frameData.push_back(tempFrame);

                ///*** build the frame skeleton ***///
                std::vector<Joint> tempSkeleton;

                for(int i = 0; i < tempAnim.jointInfo.size(); i++)
                {
                    int k = 0;                        // Keep track of position in frameData array

                    // Start the frames joint with the base frame's joint
                    Joint tempFrameJoint = tempAnim.baseFrameJoints[i];

                    tempFrameJoint.parentID = tempAnim.jointInfo[i].parentID;

                    // Notice how I have been flipping y and z. this is because some modeling programs such as
                    // 3ds max (which is what I use) use a right handed coordinate system. Because of this, we
                    // need to flip the y and z axes. If your having problems loading some models, it's possible
                    // the model was created in a left hand coordinate system. in that case, just reflip all the
                    // y and z axes in our md5 mesh and anim loader.
                    if(tempAnim.jointInfo[i].flags & 1)        // pos.x    ( 000001 )
                        tempFrameJoint.pos.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

                    if(tempAnim.jointInfo[i].flags & 2)        // pos.y    ( 000010 )
                        tempFrameJoint.pos.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

                    if(tempAnim.jointInfo[i].flags & 4)        // pos.z    ( 000100 )
                        tempFrameJoint.pos.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

                    if(tempAnim.jointInfo[i].flags & 8)        // orientation.x    ( 001000 )
                        tempFrameJoint.orientation.x = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

                    if(tempAnim.jointInfo[i].flags & 16)    // orientation.y    ( 010000 )
                        tempFrameJoint.orientation.z = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];

                    if(tempAnim.jointInfo[i].flags & 32)    // orientation.z    ( 100000 )
                        tempFrameJoint.orientation.y = tempFrame.frameData[tempAnim.jointInfo[i].startIndex + k++];


                    // Compute the quaternions w
                    float t = 1.0f - ( tempFrameJoint.orientation.x * tempFrameJoint.orientation.x )
                        - ( tempFrameJoint.orientation.y * tempFrameJoint.orientation.y )
                        - ( tempFrameJoint.orientation.z * tempFrameJoint.orientation.z );
                    if ( t < 0.0f )
                    {
                        tempFrameJoint.orientation.w = 0.0f;
                    }
                    else
                    {
                        tempFrameJoint.orientation.w = -sqrtf(t);
                    }

                    // Now, if the upper arm of your skeleton moves, you need to also move the lower part of your arm, and then the hands, and then finally the fingers (possibly weapon or tool too)
                    // This is where joint hierarchy comes in. We start at the top of the hierarchy, and move down to each joints child, rotating and translating them based on their parents rotation
                    // and translation. We can assume that by the time we get to the child, the parent has already been rotated and transformed based of it's parent. We can assume this because
                    // the child should never come before the parent in the files we loaded in.
                    if(tempFrameJoint.parentID >= 0)
                    {
                        Joint parentJoint = tempSkeleton[tempFrameJoint.parentID];

                        // Turn the XMFLOAT3 and 4's into vectors for easier computation
                        XMVECTOR parentJointOrientation = XMVectorSet(parentJoint.orientation.x, parentJoint.orientation.y, parentJoint.orientation.z, parentJoint.orientation.w);
                        XMVECTOR tempJointPos = XMVectorSet(tempFrameJoint.pos.x, tempFrameJoint.pos.y, tempFrameJoint.pos.z, 0.0f);
                        XMVECTOR parentOrientationConjugate = XMVectorSet(-parentJoint.orientation.x, -parentJoint.orientation.y, -parentJoint.orientation.z, parentJoint.orientation.w);

                        // Calculate current joints position relative to its parents position
                        XMFLOAT3 rotatedPos;
                        XMStoreFloat3(&rotatedPos, XMQuaternionMultiply(XMQuaternionMultiply(parentJointOrientation, tempJointPos), parentOrientationConjugate));

                        // Translate the joint to model space by adding the parent joint's pos to it
                        tempFrameJoint.pos.x = rotatedPos.x + parentJoint.pos.x;
                        tempFrameJoint.pos.y = rotatedPos.y + parentJoint.pos.y;
                        tempFrameJoint.pos.z = rotatedPos.z + parentJoint.pos.z;

                        // Currently the joint is oriented in its parent joints space, we now need to orient it in
                        // model space by multiplying the two orientations together (parentOrientation * childOrientation) <- In that order
                        XMVECTOR tempJointOrient = XMVectorSet(tempFrameJoint.orientation.x, tempFrameJoint.orientation.y, tempFrameJoint.orientation.z, tempFrameJoint.orientation.w);
                        tempJointOrient = XMQuaternionMultiply(parentJointOrientation, tempJointOrient);

                        // Normalize the orienation quaternion
                        tempJointOrient = XMQuaternionNormalize(tempJointOrient);

                        XMStoreFloat4(&tempFrameJoint.orientation, tempJointOrient);
                    }

                    // Store the joint into our temporary frame skeleton
                    tempSkeleton.push_back(tempFrameJoint);
                }

                // Push back our newly created frame skeleton into the animation's frameSkeleton array
                tempAnim.frameSkeleton.push_back(tempSkeleton);

                fileIn >> checkString;                // Skip closing bracket "}"
            }
        }

        // Calculate and store some usefull animation data
        tempAnim.frameTime = 1.0f / tempAnim.frameRate;                        // Set the time per frame
        tempAnim.totalAnimTime = tempAnim.numFrames * tempAnim.frameTime;    // Set the total time the animation takes
        tempAnim.currAnimTime = 0.0f;                                        // Set the current time to zero

        MD5Model.animations.push_back(tempAnim);                            // Push back the animation into our model object
    }
    else    // If the file was not loaded
    {
        SwapChain->SetFullscreenState(false, NULL);    // Make sure we are out of fullscreen

        // create message
        std::wstring message = L"Could not open: ";
        message += filename;

        MessageBox(0, message.c_str(),                // display message
            L"Error", MB_OK);

        return false;
    }
    return true;
}

void UpdateMD5Model(Model3D& MD5Model, float deltaTime, int animation)
{
    MD5Model.animations[animation].currAnimTime += deltaTime;            // 更新当前动画时间

    if(MD5Model.animations[animation].currAnimTime > MD5Model.animations[animation].totalAnimTime)
        MD5Model.animations[animation].currAnimTime = 0.0f;

    // 当前所在帧
    float currentFrame = MD5Model.animations[animation].currAnimTime * MD5Model.animations[animation].frameRate;    
    int frame0 = floorf( currentFrame );
    int frame1 = frame0 + 1;

    // 确保不会超过帧的数量
    if(frame0 == MD5Model.animations[animation].numFrames-1)
        frame1 = 0;

    float interpolation = currentFrame - frame0;    // 获取frame0与frame1之间的余数作为插值因子

    std::vector<Joint> interpolatedSkeleton;        // 创建一个帧骨骼来存储插值的骨骼

    // 计算插值骨骼
    for( int i = 0; i < MD5Model.animations[animation].numJoints; i++)
    {
        Joint tempJoint;
        Joint joint0 = MD5Model.animations[animation].frameSkeleton[frame0][i];        // 获取frame0的骨骼的第i个关节
        Joint joint1 = MD5Model.animations[animation].frameSkeleton[frame1][i];        // 获取frame1的骨骼的第i个关节

        tempJoint.parentID = joint0.parentID;                                            // 设置tempJoint的父节点id

        // 将两个四元素转换进XMVECTOR
        XMVECTOR joint0Orient = XMVectorSet(joint0.orientation.x, joint0.orientation.y, joint0.orientation.z, joint0.orientation.w);
        XMVECTOR joint1Orient = XMVectorSet(joint1.orientation.x, joint1.orientation.y, joint1.orientation.z, joint1.orientation.w);

        // 插值位置
        tempJoint.pos.x = joint0.pos.x + (interpolation * (joint1.pos.x - joint0.pos.x));
        tempJoint.pos.y = joint0.pos.y + (interpolation * (joint1.pos.y - joint0.pos.y));
        tempJoint.pos.z = joint0.pos.z + (interpolation * (joint1.pos.z - joint0.pos.z));

        // 使用球面线性插值(Slerp)插值方向
        XMStoreFloat4(&tempJoint.orientation, XMQuaternionSlerp(joint0Orient, joint1Orient, interpolation));

        interpolatedSkeleton.push_back(tempJoint);        // 将关节push进插值的骨骼
    }

    for ( int k = 0; k < MD5Model.numSubsets; k++)
    {
        for ( int i = 0; i < MD5Model.subsets[k].vertices.size(); ++i )
        {
            Vertex tempVert = MD5Model.subsets[k].vertices[i];
            tempVert.pos = XMFLOAT3(0, 0, 0);    // 首先确保顶点的位置被清除
            tempVert.normal = XMFLOAT3(0,0,0);    // 清除顶点法线

            // 总结关节和权重信息以获得顶点的位置和法线
            for ( int j = 0; j < tempVert.WeightCount; ++j )
            {
                Weight tempWeight = MD5Model.subsets[k].weights[tempVert.StartWeight + j];
                Joint tempJoint = interpolatedSkeleton[tempWeight.jointID];

                // 将关节方向和权重位置转换到向量
                XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
                XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

                // 对关节方向四元数求反
                XMVECTOR tempJointOrientationConjugate = XMQuaternionInverse(tempJointOrientation);

                // 使用关节方向四元数和它的四元素的反数为该权重计算顶点位置(在关节空间，比如，绕着原点旋转)
                // 可使用方程rotatedPoint = quaternion * point * quaternionConjugate来旋转一个点
                XMFLOAT3 rotatedPoint;
                XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

                //将顶点位置从关节空间（0,0,0）位置移动到世界空间的关节位置,要考虑权重偏差
                tempVert.pos.x += ( tempJoint.pos.x + rotatedPoint.x ) * tempWeight.bias;
                tempVert.pos.y += ( tempJoint.pos.y + rotatedPoint.y ) * tempWeight.bias;
                tempVert.pos.z += ( tempJoint.pos.z + rotatedPoint.z ) * tempWeight.bias;

                // 根据前面的权重法线为该帧骨骼计算法线
                // 可使用计算顶点位置一样的方法来计算法线，只是不必转换它们(仅仅旋转即可)
                XMVECTOR tempWeightNormal = XMVectorSet(tempWeight.normal.x, tempWeight.normal.y, tempWeight.normal.z, 0.0f);

                // 旋转法线
                XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightNormal), tempJointOrientationConjugate));

                // 添加到顶点法线并考虑权重偏差
                tempVert.normal.x -= rotatedPoint.x * tempWeight.bias;
                tempVert.normal.y -= rotatedPoint.y * tempWeight.bias;
                tempVert.normal.z -= rotatedPoint.z * tempWeight.bias;
            }

            MD5Model.subsets[k].positions[i] = tempVert.pos;                // 将顶点位置存储进位置向量而不是直接存储到顶点向量
            MD5Model.subsets[k].vertices[i].normal = tempVert.normal;        // 存储顶点法线
            XMStoreFloat3(&MD5Model.subsets[k].vertices[i].normal, XMVector3Normalize(XMLoadFloat3(&MD5Model.subsets[k].vertices[i].normal)));
        }

        // 将位置放入该子集的顶点中去
        for(int i = 0; i < MD5Model.subsets[k].vertices.size(); i++)
        {
            MD5Model.subsets[k].vertices[i].pos = MD5Model.subsets[k].positions[i];
        }

        // 更新子集顶点缓冲
        // 先锁定缓冲
        D3D11_MAPPED_SUBRESOURCE mappedVertBuff;
        hr = d3d11DevCon->Map(MD5Model.subsets[k].vertBuff, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedVertBuff);

        // 将数据拷贝到顶点缓冲中去
        memcpy(mappedVertBuff.pData, &MD5Model.subsets[k].vertices[0], (sizeof(Vertex) * MD5Model.subsets[k].vertices.size()));

        d3d11DevCon->Unmap(MD5Model.subsets[k].vertBuff, 0);

        // 下面这行时另外一种更新缓冲的方式.当想要更新一个每帧少一次的缓冲时会使用该方法，
        // 由于GPU读取会更快(由DEFAULT创建的缓冲而不是DYNAMIC创建的
        // ),但是CPU写入会更慢，可尝试两种都使用找出哪一个更快
        // 若要使用下面这行,则必须创建一个带D3D11_USAGE_DEFAULT的缓冲而不是D3D11_USAGE_DYNAMIC的缓冲
        //d3d11DevCon->UpdateSubresource( MD5Model.subsets[k].vertBuff, 0, NULL, &MD5Model.subsets[k].vertices[0], 0, 0 );
    }
}
///////////////**************new**************////////////////////
bool LoadMD5Model(std::wstring filename,
    Model3D& MD5Model,
    std::vector<ID3D11ShaderResourceView*>& shaderResourceViewArray,
    std::vector<std::wstring> texFileNameArray)
{
    std::wifstream fileIn (filename.c_str());        // 打开文件

    std::wstring checkString;                        // 保存来自文件的字符

    if(fileIn)                                        // 检测文件是否打开
    {
        while(fileIn)                                // 循环到文件结尾
        {    
            fileIn >> checkString;                    // 获取文件下一个字符

            if(checkString == L"MD5Version")        // 获得MD5版本，这里为版本10
            {
                /*fileIn >> checkString;
                MessageBox(0, checkString.c_str(),    //显示信息
                L"MD5Version", MB_OK);*/
            }
            else if ( checkString == L"commandline" )
            {
                std::getline(fileIn, checkString);    // 忽略剩下的行
            }
            else if ( checkString == L"numJoints" )
            {
                fileIn >> MD5Model.numJoints;        // 保存关节数
            }
            else if ( checkString == L"numMeshes" )
            {
                fileIn >> MD5Model.numSubsets;        // 保存要调用的蒙皮数
            }
            else if ( checkString == L"joints" )
            {
                Joint tempJoint;

                fileIn >> checkString;                // 跳过 "{"

                for(int i = 0; i < MD5Model.numJoints; i++)
                {
                    fileIn >> tempJoint.name;        // 保存关节名
                    // 有时名字可能包含空格.若存在这种情况，则继续读取直到
                    // 闭引号"为止
                    if(tempJoint.name[tempJoint.name.size()-1] != '"')
                    {
                        wchar_t checkChar;
                        bool jointNameFound = false;
                        while(!jointNameFound)
                        {
                            checkChar = fileIn.get();

                            if(checkChar == '"')
                                jointNameFound = true;        

                            tempJoint.name += checkChar;                                                            
                        }
                    }

                    fileIn >> tempJoint.parentID;    // 保存父关节

                    fileIn >> checkString;            // 跳过 "("

                    // 保存关节位置（若模型在右手坐标系中，则调换y和z轴）
                    fileIn >> tempJoint.pos.x >> tempJoint.pos.z >> tempJoint.pos.y;

                    fileIn >> checkString >> checkString;    // 跳过 ")" 和 "("

                    // 保存关节的方位
                    fileIn >> tempJoint.orientation.x >> tempJoint.orientation.z >> tempJoint.orientation.y;

                    // 删除关节名的双引号
                    tempJoint.name.erase(0, 1);
                    tempJoint.name.erase(tempJoint.name.size()-1, 1);

                    // 计算四元素的w轴（MD5模型使用3D向量来表示骨头面对的方向。但是需要将它转为四元素，同时
                    // 四元素工作的方式为xyz值表示旋转轴，w值表示旋转角度，介于0和1之间
                    float t = 1.0f - ( tempJoint.orientation.x * tempJoint.orientation.x )
                        - ( tempJoint.orientation.y * tempJoint.orientation.y )
                        - ( tempJoint.orientation.z * tempJoint.orientation.z );
                    if ( t < 0.0f )
                    {
                        tempJoint.orientation.w = 0.0f;
                    }
                    else
                    {
                        tempJoint.orientation.w = -sqrtf(t);
                    }

                    std::getline(fileIn, checkString);        // 跳过该行剩余部分

                    MD5Model.joints.push_back(tempJoint);    // 将关节保存进模型的关节向量中
                }

                fileIn >> checkString;                    // 跳过"}"
            }
            else if ( checkString == L"mesh")
            {
                ModelSubset subset;
                int numVerts, numTris, numWeights;

                fileIn >> checkString;                    // 跳过"{"

                fileIn >> checkString;
                while ( checkString != L"}" )            // 一直读取到 '}' 为止
                {
                    // 本章中为简单起见，假设这里已经给定纹理名.
                    // 尽管通常给定的是材质名 (保存在材质库中，在加载.obj文件时可知，材质库是保存在.mtl文件中的）
                    if(checkString == L"shader")        // 加载纹理或材质
                    {                        
                        std::wstring fileNamePath;
                        fileIn >> fileNamePath;            // 获取纹理名

                        // 若文件名或材质名含有空格时需要做的
                        if(fileNamePath[fileNamePath.size()-1] != '"')
                        {
                            wchar_t checkChar;
                            bool fileNameFound = false;
                            while(!fileNameFound)
                            {
                                checkChar = fileIn.get();

                                if(checkChar == '"')
                                    fileNameFound = true;

                                fileNamePath += checkChar;                                                                    
                            }
                        }

                        // 删除纹理路径的引号
                        fileNamePath.erase(0, 1);
                        fileNamePath.erase(fileNamePath.size()-1, 1);

                        //检测纹理是否已经加载
                        bool alreadyLoaded = false;
                        for(int i = 0; i < texFileNameArray.size(); ++i)
                        {
                            if(fileNamePath == texFileNameArray[i])
                            {
                                alreadyLoaded = true;
                                subset.texArrayIndex = i;
                            }
                        }

                        //纹理还没有加载，现在加载
                        if(!alreadyLoaded)
                        {
                            ID3D11ShaderResourceView* tempMeshSRV;
                            hr = D3DX11CreateShaderResourceViewFromFile( d3d11Device, fileNamePath.c_str(),
                                NULL, NULL, &tempMeshSRV, NULL );
                            if(SUCCEEDED(hr))
                            {
                                texFileNameArray.push_back(fileNamePath.c_str());
                                subset.texArrayIndex = shaderResourceViewArray.size();
                                shaderResourceViewArray.push_back(tempMeshSRV);
                            }
                            else
                            {
                                MessageBox(0, fileNamePath.c_str(),        //显示消息
                                    L"Could Not Open:", MB_OK);
                                return false;
                            }
                        }    

                        std::getline(fileIn, checkString);                // 跳过本行剩余部分
                    }
                    else if ( checkString == L"numverts")
                    {
                        fileIn >> numVerts;                                // 保存顶点数

                        std::getline(fileIn, checkString);                // 跳过本行剩余部分

                        for(int i = 0; i < numVerts; i++)
                        {
                            Vertex tempVert;

                            fileIn >> checkString                        // 跳过 "vert # ("
                                >> checkString
                                >> checkString;

                            fileIn >> tempVert.texCoord.x                // 保存tex坐标
                                >> tempVert.texCoord.y;    

                            fileIn >> checkString;                        // 跳过 ")"

                            fileIn >> tempVert.StartWeight;                // 本顶点要加权的权重索引

                            fileIn >> tempVert.WeightCount;                // 本顶点的权重数

                            std::getline(fileIn, checkString);            // 跳过本行剩余部分

                            subset.vertices.push_back(tempVert);        // 将顶点push回蒙皮子集顶点向量中
                        }
                    }
                    else if ( checkString == L"numtris")
                    {
                        fileIn >> numTris;
                        subset.numTriangles = numTris;

                        std::getline(fileIn, checkString);                // 跳过本行剩余部分

                        for(int i = 0; i < numTris; i++)                // 轮询每个三角形
                        {
                            DWORD tempIndex;
                            fileIn >> checkString;                        // 跳过 "tri"
                            fileIn >> checkString;                        // 跳过 tri计数

                            for(int k = 0; k < 3; k++)                    // 保存3个索引
                            {
                                fileIn >> tempIndex;
                                subset.indices.push_back(tempIndex);
                            }

                            std::getline(fileIn, checkString);            // 跳过本行剩余部分
                        }
                    }
                    else if ( checkString == L"numweights")
                    {
                        fileIn >> numWeights;

                        std::getline(fileIn, checkString);                // 跳过本行剩余部分

                        for(int i = 0; i < numWeights; i++)
                        {
                            Weight tempWeight;
                            fileIn >> checkString >> checkString;        // 跳过 "weight #"

                            fileIn >> tempWeight.jointID;                // 保存 weight的关节ID

                            fileIn >> tempWeight.bias;                    // 保存施加在顶点上的weight的影响

                            fileIn >> checkString;                        // 跳过 "("

                            fileIn >> tempWeight.pos.x                    // 保存关节的模型空间的权重的位置
                                >> tempWeight.pos.z
                                >> tempWeight.pos.y;

                            std::getline(fileIn, checkString);            // 跳过本行剩余部分

                            subset.weights.push_back(tempWeight);        // 将临时关节push回蒙皮子集权重数组中
                        }

                    }
                    else
                        std::getline(fileIn, checkString);                // 跳过本行剩余部分

                    fileIn >> checkString;                                // 跳过 "}"
                }

                //*** 使用关节和权重得到每个顶点位置信息 ***//
                for ( int i = 0; i < subset.vertices.size(); ++i )
                {
                    Vertex tempVert = subset.vertices[i];
                    tempVert.pos = XMFLOAT3(0, 0, 0);    // 确保顶点位置首先被清除

                    // 将关节和权重信息相加得到顶点的位置信息
                    for ( int j = 0; j < tempVert.WeightCount; ++j )
                    {
                        Weight tempWeight = subset.weights[tempVert.StartWeight + j];
                        Joint tempJoint = MD5Model.joints[tempWeight.jointID];

                        // 为更容易计算将关节方向和权重位置转换到向量
                        // 当将3d向量转换到四元数时，要将w设为0，并且
                        // 当将四元素转为3d向量时，只需要忽略w即可
                        XMVECTOR tempJointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);
                        XMVECTOR tempWeightPos = XMVectorSet(tempWeight.pos.x, tempWeight.pos.y, tempWeight.pos.z, 0.0f);

                        // 需要使用关节方位四元素的共轭
                        // 对x,y,z求逆即可得到四元素的共轭
                        XMVECTOR tempJointOrientationConjugate = XMVectorSet(-tempJoint.orientation.x, -tempJoint.orientation.y, -tempJoint.orientation.z, tempJoint.orientation.w);

                        // 计算顶点位置（在关节空间，比如，针对该权重，通过关节方位四元素以及它的共轭来绕着原点(0,0,0)旋转该点）
                        // 通过方程式rotatedPoint = quaternion * point * quaternionConjugate以及它的四元素来旋转点
                        XMFLOAT3 rotatedPoint;
                        XMStoreFloat3(&rotatedPoint, XMQuaternionMultiply(XMQuaternionMultiply(tempJointOrientation, tempWeightPos), tempJointOrientationConjugate));

                        // 现在将关节空间的顶点位置转换到世界空间的关节位置，考虑权重偏差
                        // 考虑权重偏差是因为多权重可能会对顶点的最终位置有影响。每个权重关联到一个关节。
                        tempVert.pos.x += ( tempJoint.pos.x + rotatedPoint.x ) * tempWeight.bias;
                        tempVert.pos.y += ( tempJoint.pos.y + rotatedPoint.y ) * tempWeight.bias;
                        tempVert.pos.z += ( tempJoint.pos.z + rotatedPoint.z ) * tempWeight.bias;

                        // 上面的意思是，让权重位置和关节位置联系起来
                        // 随后通过使用表示关节旋转的四元素来旋转权重位置（以便权重能够在世界空间绕着（0,0,0）旋转）
                        // 在rotatedPoint变量中保存该旋转点，随后添加到世界空间的关节位置上（由于在世界空间中绕着（0,0,0）旋转权重的位置,
						//现在要对它进行转换,以便能够看起来像是绕着关节位置旋转）
                        // 最后将结果和权重偏差或最终顶点位置受权重影响的大小量相乘。所有影响单个顶点位置的权重偏差必须加到1
                        
                    }

                    subset.positions.push_back(tempVert.pos);            // 将顶点位置保存进位置向量而不是直接保存进顶点向量
                    // 由于可能还会需要在碰撞或拾取中使用到位置向量
                    // 而避免使用到整个顶点结构
                }

                // 将位置保存进蒙皮的向量中
                for(int i = 0; i < subset.vertices.size(); i++)
                {
                    subset.vertices[i].pos = subset.positions[i];
                }

                //*** 使用法线平均法计算顶点法线 ***///
                std::vector<XMFLOAT3> tempNormal;

                //规范和不规范的法线
                XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

                //用来从verts位置获取向量
                float vecX, vecY, vecZ;

                //三角形的两个边
                XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
                XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

                //计算面法线
                for(int i = 0; i < subset.numTriangles; ++i)
                {
                    //获取表示一个三角形的边的向量(edge 0,1)
                    vecX = subset.vertices[subset.indices[(i*3)]].pos.x - subset.vertices[subset.indices[(i*3)+2]].pos.x;
                    vecY = subset.vertices[subset.indices[(i*3)]].pos.y - subset.vertices[subset.indices[(i*3)+2]].pos.y;
                    vecZ = subset.vertices[subset.indices[(i*3)]].pos.z - subset.vertices[subset.indices[(i*3)+2]].pos.z;        
                    edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //创建第一个边

                    //获取表示另一个三角形的边的向量(edge 2,1)
                    vecX = subset.vertices[subset.indices[(i*3)+2]].pos.x - subset.vertices[subset.indices[(i*3)+1]].pos.x;
                    vecY = subset.vertices[subset.indices[(i*3)+2]].pos.y - subset.vertices[subset.indices[(i*3)+1]].pos.y;
                    vecZ = subset.vertices[subset.indices[(i*3)+2]].pos.z - subset.vertices[subset.indices[(i*3)+1]].pos.z;        
                    edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);    //创建第二个边

                    //叉乘两个边向量以获取非规范化面法线
                    XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

                    tempNormal.push_back(unnormalized);
                }

                //计算顶点法线（法线平均化）
                XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
                int facesUsing = 0;
                float tX, tY, tZ;    //临时轴变量

                //遍历每个顶点
                for(int i = 0; i < subset.vertices.size(); ++i)
                {
                    //检测哪个三角形使用该顶点
                    for(int j = 0; j < subset.numTriangles; ++j)
                    {
                        if(subset.indices[j*3] == i ||
                            subset.indices[(j*3)+1] == i ||
                            subset.indices[(j*3)+2] == i)
                        {
                            tX = XMVectorGetX(normalSum) + tempNormal[j].x;
                            tY = XMVectorGetY(normalSum) + tempNormal[j].y;
                            tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

                            normalSum = XMVectorSet(tX, tY, tZ, 0.0f);    //若一个面使用顶点，将非规范化面法线添加到normalSum

                            facesUsing++;
                        }
                    }

                    //通过用共享同一个顶点的面数除以normalSum得到真实法线
                    normalSum = normalSum / facesUsing;

                    //规范化normalSum向量
                    normalSum = XMVector3Normalize(normalSum);

                    //将法线和切线保存进当前顶点
                    subset.vertices[i].normal.x = -XMVectorGetX(normalSum);
                    subset.vertices[i].normal.y = -XMVectorGetY(normalSum);
                    subset.vertices[i].normal.z = -XMVectorGetZ(normalSum);

 ///////////////**************new**************////////////////////
                    // 为了更容易计算法线在动画中创建关节空间法线
                    Vertex tempVert = subset.vertices[i];                        // 获取当前顶点
                    subset.jointSpaceNormals.push_back(XMFLOAT3(0,0,0));        // push回一个空白法线
                    XMVECTOR normal = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);        // 清除法线

                    for ( int k = 0; k < tempVert.WeightCount; k++)                // 遍历每一个顶点权重
                    {
                        Joint tempJoint = MD5Model.joints[subset.weights[tempVert.StartWeight + k].jointID];    // 获取关节方向
                        XMVECTOR jointOrientation = XMVectorSet(tempJoint.orientation.x, tempJoint.orientation.y, tempJoint.orientation.z, tempJoint.orientation.w);

                        // 基于关节方向计算法线(转换进关节空间)
                        normal = XMQuaternionMultiply(XMQuaternionMultiply(XMQuaternionInverse(jointOrientation), normalSum), jointOrientation);        

                        XMStoreFloat3(&subset.weights[tempVert.StartWeight + k].normal, XMVector3Normalize(normal));            // 将规范四元素存储进权重法线
                    }                
                    ///////////////**************new**************////////////////////
                    //为下一个向量清除normalSum,faceUsing
                    normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
                    facesUsing = 0;
                }

                // 创建索引缓冲
                D3D11_BUFFER_DESC indexBufferDesc;
                ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

                indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
                indexBufferDesc.ByteWidth = sizeof(DWORD) * subset.numTriangles * 3;
                indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
                indexBufferDesc.CPUAccessFlags = 0;
                indexBufferDesc.MiscFlags = 0;

                D3D11_SUBRESOURCE_DATA iinitData;

                iinitData.pSysMem = &subset.indices[0];
                d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &subset.indexBuff);

                //创建顶点缓冲
                D3D11_BUFFER_DESC vertexBufferDesc;
                ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

                vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;                            // 将会持续更新该缓冲，因此要设为动态的
                vertexBufferDesc.ByteWidth = sizeof( Vertex ) * subset.vertices.size();
                vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
                vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                // 赋给cpu写到缓冲的权限
                vertexBufferDesc.MiscFlags = 0;

                D3D11_SUBRESOURCE_DATA vertexBufferData; 

                ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
                vertexBufferData.pSysMem = &subset.vertices[0];
                hr = d3d11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &subset.vertBuff);

                // 临时蒙皮push会模型的蒙皮向量中
                MD5Model.subsets.push_back(subset);
            }
        }
    }
    else
    {
        SwapChain->SetFullscreenState(false, NULL);    // 确保退出全屏

        // 创建消息
        std::wstring message = L"Could not open: ";
        message += filename;

        MessageBox(0, message.c_str(),    // 显示消息
            L"Error", MB_OK);

        return false;
    }

    return true;
}
///////////////**************new**************////////////////////
bool LoadObjModel(std::wstring filename, 
	ID3D11Buffer** vertBuff, 
	ID3D11Buffer** indexBuff,
	std::vector<int>& subsetIndexStart,
	std::vector<int>& subsetMaterialArray,
	std::vector<SurfaceMaterial>& material, 
	int& subsetCount,
	bool isRHCoordSys,
    bool computeNormals)
{
	HRESULT hr = 0;

	std::wifstream fileIn (filename.c_str());	//打开文件
	std::wstring meshMatLib;					//保存obj材质库文件名的字符串

	//存储我们模型的信息的数组
	std::vector<DWORD> indices;
	std::vector<XMFLOAT3> vertPos;
	std::vector<XMFLOAT3> vertNorm;
	std::vector<XMFLOAT2> vertTexCoord;
	std::vector<std::wstring> meshMaterials;

	//顶点定义索引
	std::vector<int> vertPosIndex;
	std::vector<int> vertNormIndex;
	std::vector<int> vertTCIndex;

	//如果没有定义纹理坐标或发现，确保有一个默认的值
	bool hasTexCoord = false;
	bool hasNorm = false;

	//用于存储向量的临时变量
	std::wstring meshMaterialsTemp;
	int vertPosIndexTemp;
	int vertNormIndexTemp;
	int vertTCIndexTemp;

	wchar_t checkChar;		//每次从文件读出一个字符要存储的地方
	std::wstring face;		//保存包含面顶点的字符串
	int vIndex = 0;			//跟踪顶点索引数
	int triangleCount = 0;	//所有三角形
	int totalVerts = 0;
	int meshTriangles = 0;

	//检测文件是否被打开
	if (fileIn)
	{
		while(fileIn)
		{			
			checkChar = fileIn.get();	//获取下一个字符

			switch (checkChar)
			{		
			case '#':
				checkChar = fileIn.get();
				while(checkChar != '\n')
					checkChar = fileIn.get();
				break;
			case 'v':	//获取向量描述符
				checkChar = fileIn.get();
				if(checkChar == ' ')	//v - 向量位置
				{
					float vz, vy, vx;
					fileIn >> vx >> vy >> vz;	//存储接下来三个类型

					if(isRHCoordSys)	//若模型来自右手坐标系
						vertPos.push_back(XMFLOAT3( vx, vy, vz * -1.0f));	//转换Z轴
					else
						vertPos.push_back(XMFLOAT3( vx, vy, vz));
				}
				if(checkChar == 't')	//vt - 顶点纹理坐标
				{			
					float vtcu, vtcv;
					fileIn >> vtcu >> vtcv;		//保存接下来的两个类型

					if(isRHCoordSys)	//若模型来自右手坐标系
						vertTexCoord.push_back(XMFLOAT2(vtcu, 1.0f-vtcv));	//将v轴翻转
					else
						vertTexCoord.push_back(XMFLOAT2(vtcu, vtcv));	

					hasTexCoord = true;	//模型使用纹理坐标系
				}
				//由于我们在后来计算法线，我们不必在此检测法线
				//In the file, but i'll do it here anyway
				if(checkChar == 'n')	//vn - 顶点法线
				{
					float vnx, vny, vnz;
					fileIn >> vnx >> vny >> vnz;	//存储接下来三个类型

					if(isRHCoordSys)	//若模型来自右手坐标系
						vertNorm.push_back(XMFLOAT3( vnx, vny, vnz * -1.0f ));	//将z轴翻转
					else
						vertNorm.push_back(XMFLOAT3( vnx, vny, vnz ));	

					hasNorm = true;	//模型定义法线
				}
				break;

				//新组（子集）
			case 'g':	//g - 定义一个组
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					subsetIndexStart.push_back(vIndex);		//子集起始索引
					subsetCount++;
				}
				break;

				//获取面索引
			case 'f':	//f - 定义面
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					face = L"";
					std::wstring VertDef;	//一个时刻存储一个顶点定义
					triangleCount = 0;

					checkChar = fileIn.get();
					while(checkChar != '\n')
					{
						face += checkChar;			//将字符添加到面字符串
						checkChar = fileIn.get();	//获取下一个字符
						if(checkChar == ' ')		//若为空格
							triangleCount++;		//增加三角形计数
					}

					//在面字符串结尾检测空格
					if(face[face.length()-1] == ' ')
						triangleCount--;	//每个空格添加到三角形计数

					triangleCount -= 1;		//Ever vertex in the face AFTER the first two are new faces

					std::wstringstream ss(face);

					if(face.length() > 0)
					{
						int firstVIndex, lastVIndex;	//Holds the first and last vertice's index

						for(int i = 0; i < 3; ++i)		//First three vertices (first triangle)
						{
							ss >> VertDef;	//Get vertex definition (vPos/vTexCoord/vNorm)

							std::wstring vertPart;
							int whichPart = 0;		//(vPos, vTexCoord, or vNorm)

							//Parse this string
							for(int j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')	//If there is no divider "/", add a char to our vertPart
									vertPart += VertDef[j];

								//If the current char is a divider "/", or its the last character in the string
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									std::wistringstream wstringToInt(vertPart);	//Used to convert wstring to int

									if(whichPart == 0)	//If vPos
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertNormIndexTemp = 0;
											vertTCIndexTemp = 0;
										}
									}

									else if(whichPart == 1)	//If vTexCoord
									{
										if(vertPart != L"")	//Check to see if there even is a tex coord
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;	//subtract one since c++ arrays start with 0, and obj start with 1
										}
										else	//If there is no tex coord, make a default
											vertTCIndexTemp = 0;

										//If the cur. char is the second to last in the string, then
										//there must be no normal, so set a default normal
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)	//If vNorm
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;		//subtract one since c++ arrays start with 0, and obj start with 1
									}

									vertPart = L"";	//Get ready for next vertex part
									whichPart++;	//Move on to next vertex part					
								}
							}

							//Check to make sure there is at least one subset
							if(subsetCount == 0)
							{
								subsetIndexStart.push_back(vIndex);		//Start index for this subset
								subsetCount++;
							}

							//Avoid duplicate vertices
							bool vertAlreadyExists = false;
							if(totalVerts >= 3)	//Make sure we at least have one triangle to check
							{
								//Loop through all the vertices
								for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									//If the vertex position and texture coordinate in memory are the same
									//As the vertex position and texture coordinate we just now got out
									//of the obj file, we will set this faces vertex index to the vertex's
									//index value in memory. This makes sure we don't create duplicate vertices
									if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);		//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
									}
								}
							}

							//If this vertex is not already in our vertex arrays, put it there
							if(!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;	//We created a new vertex
								indices.push_back(totalVerts-1);	//Set index for this vertex
							}							

							//If this is the very first vertex in the face, we need to
							//make sure the rest of the triangles use this vertex
							if(i == 0)
							{
								firstVIndex = indices[vIndex];	//The first vertex index of this FACE

							}

							//If this was the last vertex in the first triangle, we will make sure
							//the next triangle uses this one (eg. tri1(1,2,3) tri2(1,3,4) tri3(1,4,5))
							if(i == 2)
							{								
								lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE
							}
							vIndex++;	//Increment index count
						}

						meshTriangles++;	//One triangle down

						//If there are more than three vertices in the face definition, we need to make sure
						//we convert the face to triangles. We created our first triangle above, now we will
						//create a new triangle for every new vertex in the face, using the very first vertex
						//of the face, and the last vertex from the triangle before the current triangle
						for(int l = 0; l < triangleCount-1; ++l)	//Loop through the next vertices to create new triangles
						{
							//First vertex of this triangle (the very first vertex of the face too)
							indices.push_back(firstVIndex);			//Set index for this vertex
							vIndex++;

							//Second Vertex of this triangle (the last vertex used in the tri before this one)
							indices.push_back(lastVIndex);			//Set index for this vertex
							vIndex++;

							//Get the third vertex for this triangle
							ss >> VertDef;

							std::wstring vertPart;
							int whichPart = 0;

							//Parse this string (same as above)
							for(int j = 0; j < VertDef.length(); ++j)
							{
								if(VertDef[j] != '/')
									vertPart += VertDef[j];
								if(VertDef[j] == '/' || j ==  VertDef.length()-1)
								{
									std::wistringstream wstringToInt(vertPart);

									if(whichPart == 0)
									{
										wstringToInt >> vertPosIndexTemp;
										vertPosIndexTemp -= 1;

										//Check to see if the vert pos was the only thing specified
										if(j == VertDef.length()-1)
										{
											vertTCIndexTemp = 0;
											vertNormIndexTemp = 0;
										}
									}
									else if(whichPart == 1)
									{
										if(vertPart != L"")
										{
											wstringToInt >> vertTCIndexTemp;
											vertTCIndexTemp -= 1;
										}
										else
											vertTCIndexTemp = 0;
										if(j == VertDef.length()-1)
											vertNormIndexTemp = 0;

									}								
									else if(whichPart == 2)
									{
										std::wistringstream wstringToInt(vertPart);

										wstringToInt >> vertNormIndexTemp;
										vertNormIndexTemp -= 1;
									}

									vertPart = L"";
									whichPart++;							
								}
							}					

							//Check for duplicate vertices
							bool vertAlreadyExists = false;
							if(totalVerts >= 3)	//Make sure we at least have one triangle to check
							{
								for(int iCheck = 0; iCheck < totalVerts; ++iCheck)
								{
									if(vertPosIndexTemp == vertPosIndex[iCheck] && !vertAlreadyExists)
									{
										if(vertTCIndexTemp == vertTCIndex[iCheck])
										{
											indices.push_back(iCheck);			//Set index for this vertex
											vertAlreadyExists = true;		//If we've made it here, the vertex already exists
										}
									}
								}
							}

							if(!vertAlreadyExists)
							{
								vertPosIndex.push_back(vertPosIndexTemp);
								vertTCIndex.push_back(vertTCIndexTemp);
								vertNormIndex.push_back(vertNormIndexTemp);
								totalVerts++;					//New vertex created, add to total verts
								indices.push_back(totalVerts-1);		//Set index for this vertex
							}

							//Set the second vertex for the next triangle to the last vertex we got		
							lastVIndex = indices[vIndex];	//The last vertex index of this TRIANGLE

							meshTriangles++;	//New triangle defined
							vIndex++;		
						}
					}
				}
				break;

			case 'm':	//mtllib - material library filename
				checkChar = fileIn.get();
				if(checkChar == 't')
				{
					checkChar = fileIn.get();
					if(checkChar == 'l')
					{
						checkChar = fileIn.get();
						if(checkChar == 'l')
						{
							checkChar = fileIn.get();
							if(checkChar == 'i')
							{
								checkChar = fileIn.get();
								if(checkChar == 'b')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										//Store the material libraries file name
										fileIn >> meshMatLib;
									}
								}
							}
						}
					}
				}

				break;

			case 'u':	//usemtl - which material to use
				checkChar = fileIn.get();
				if(checkChar == 's')
				{
					checkChar = fileIn.get();
					if(checkChar == 'e')
					{
						checkChar = fileIn.get();
						if(checkChar == 'm')
						{
							checkChar = fileIn.get();
							if(checkChar == 't')
							{
								checkChar = fileIn.get();
								if(checkChar == 'l')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										meshMaterialsTemp = L"";	//Make sure this is cleared

										fileIn >> meshMaterialsTemp; //Get next type (string)

										meshMaterials.push_back(meshMaterialsTemp);
									}
								}
							}
						}
					}
				}
				break;

			default:				
				break;
			}
		}
	}
	else	//If we could not open the file
	{
		SwapChain->SetFullscreenState(false, NULL);	//Make sure we are out of fullscreen

		//create message
		std::wstring message = L"Could not open: ";
		message += filename;

		MessageBox(0, message.c_str(),	//display message
			L"Error", MB_OK);

		return false;
	}

	subsetIndexStart.push_back(vIndex); //There won't be another index start after our last subset, so set it here

	//sometimes "g" is defined at the very top of the file, then again before the first group of faces.
	//This makes sure the first subset does not conatain "0" indices.
	if(subsetIndexStart[1] == 0)
	{
		subsetIndexStart.erase(subsetIndexStart.begin()+1);
		meshSubsets--;
	}

	//Make sure we have a default for the tex coord and normal
	//if one or both are not specified
	if(!hasNorm)
		vertNorm.push_back(XMFLOAT3(0.0f, 0.0f, 0.0f));
	if(!hasTexCoord)
		vertTexCoord.push_back(XMFLOAT2(0.0f, 0.0f));

	//Close the obj file, and open the mtl file
	fileIn.close();
	fileIn.open(meshMatLib.c_str());

	std::wstring lastStringRead;
	int matCount = material.size();	//total materials

	//kdset - 若没有设置漫反射颜色，则使用环境光颜色(通常是一样的)
	//If the diffuse color WAS set, then we don't need to set our diffuse color to ambient
	bool kdset = false;

	if (fileIn)
	{
		while(fileIn)
		{
			checkChar = fileIn.get();	//Get next char

			switch (checkChar)
			{
            	//Check for comment
			case '#':
				checkChar = fileIn.get();
				while(checkChar != '\n')
					checkChar = fileIn.get();
				break;

				//Set diffuse color
			case 'K':
				checkChar = fileIn.get();
				if(checkChar == 'd')	//Diffuse Color
				{
					checkChar = fileIn.get();	//remove space

					fileIn >> material[matCount-1].difColor.x;
					fileIn >> material[matCount-1].difColor.y;
					fileIn >> material[matCount-1].difColor.z;

					kdset = true;
				}

				//Ambient Color (We'll store it in diffuse if there isn't a diffuse already)
				if(checkChar == 'a')	
				{					
					checkChar = fileIn.get();	//remove space
					if(!kdset)
					{
						fileIn >> material[matCount-1].difColor.x;
						fileIn >> material[matCount-1].difColor.y;
						fileIn >> material[matCount-1].difColor.z;
					}
				}
				break;

				//Check for transparency
			case 'T':
				checkChar = fileIn.get();
				if(checkChar == 'r')
				{
					checkChar = fileIn.get();	//remove space
					float Transparency;
					fileIn >> Transparency;

					material[matCount-1].difColor.w = Transparency;

					if(Transparency > 0.0f)
						material[matCount-1].transparent = true;
				}
				break;

				//Some obj files specify d for transparency
			case 'd':
				checkChar = fileIn.get();
				if(checkChar == ' ')
				{
					float Transparency;
					fileIn >> Transparency;

					//'d' - 0 being most transparent, and 1 being opaque, opposite of Tr
					Transparency = 1.0f - Transparency;

					material[matCount-1].difColor.w = Transparency;

					if(Transparency > 0.0f)
						material[matCount-1].transparent = true;					
				}
				break;

				//Get the diffuse map (texture)
			case 'm':
				checkChar = fileIn.get();
				if(checkChar == 'a')
				{
					checkChar = fileIn.get();
					if(checkChar == 'p')
					{
						checkChar = fileIn.get();
						if(checkChar == '_')
						{
							//map_Kd - Diffuse map
							checkChar = fileIn.get();
							if(checkChar == 'K')
							{
								checkChar = fileIn.get();
								if(checkChar == 'd')
								{
									std::wstring fileNamePath;

									fileIn.get();	//Remove whitespace between map_Kd and file

									//Get the file path - We read the pathname char by char since
									//pathnames can sometimes contain spaces, so we will read until
									//we find the file extension
									bool texFilePathEnd = false;
									while(!texFilePathEnd)
									{
										checkChar = fileIn.get();

										fileNamePath += checkChar;

										if(checkChar == '.')
										{
											for(int i = 0; i < 3; ++i)
												fileNamePath += fileIn.get();

											texFilePathEnd = true;
										}							
									}

									//check if this texture has already been loaded
									bool alreadyLoaded = false;
									for(int i = 0; i < textureNameArray.size(); ++i)
									{
										if(fileNamePath == textureNameArray[i])
										{
											alreadyLoaded = true;
											material[matCount-1].texArrayIndex = i;
											material[matCount-1].hasTexture = true;
										}
									}

									//if the texture is not already loaded, load it now
									if(!alreadyLoaded)
									{
										ID3D11ShaderResourceView* tempMeshSRV;
										hr = D3DX11CreateShaderResourceViewFromFile( d3d11Device, fileNamePath.c_str(),
											NULL, NULL, &tempMeshSRV, NULL );
										if(SUCCEEDED(hr))
										{
											textureNameArray.push_back(fileNamePath.c_str());
											material[matCount-1].texArrayIndex = meshSRV.size();
											meshSRV.push_back(tempMeshSRV);
											material[matCount-1].hasTexture = true;
										}
									}	
								}
							}
							//map_d - alpha map
							else if(checkChar == 'd')
							{
								//Alpha maps are usually the same as the diffuse map
								//So we will assume that for now by only enabling
								//transparency for this material, as we will already
								//be using the alpha channel in the diffuse map
								material[matCount-1].transparent = true;
							}
							///////////////**************new**************////////////////////
							//map_bump - bump map (we're usinga normal map though)
							else if(checkChar == 'b')
							{
								checkChar = fileIn.get();
								if(checkChar == 'u')
								{
									checkChar = fileIn.get();
									if(checkChar == 'm')
									{
										checkChar = fileIn.get();
										if(checkChar == 'p')
										{
											std::wstring fileNamePath;

											fileIn.get();	//Remove whitespace between map_bump and file

											//Get the file path - We read the pathname char by char since
											//pathnames can sometimes contain spaces, so we will read until
											//we find the file extension
											bool texFilePathEnd = false;
											while(!texFilePathEnd)
											{
												checkChar = fileIn.get();

												fileNamePath += checkChar;

												if(checkChar == '.')
												{
													for(int i = 0; i < 3; ++i)
														fileNamePath += fileIn.get();

													texFilePathEnd = true;
												}							
											}

											//check if this texture has already been loaded
											bool alreadyLoaded = false;
											for(int i = 0; i < textureNameArray.size(); ++i)
											{
												if(fileNamePath == textureNameArray[i])
												{
													alreadyLoaded = true;
													material[matCount-1].normMapTexArrayIndex = i;
													material[matCount-1].hasNormMap = true;
												}
											}

											//if the texture is not already loaded, load it now
											if(!alreadyLoaded)
											{
												ID3D11ShaderResourceView* tempMeshSRV;
												hr = D3DX11CreateShaderResourceViewFromFile( d3d11Device, fileNamePath.c_str(),
													NULL, NULL, &tempMeshSRV, NULL );
												if(SUCCEEDED(hr))
												{
													textureNameArray.push_back(fileNamePath.c_str());
													material[matCount-1].normMapTexArrayIndex = meshSRV.size();
													meshSRV.push_back(tempMeshSRV);
													material[matCount-1].hasNormMap = true;
												}
											}	
										}
									}
								}
							}
							///////////////**************new**************////////////////////
						}
					}
				}
				break;

			case 'n':	//newmtl - Declare new material
				checkChar = fileIn.get();
				if(checkChar == 'e')
				{
					checkChar = fileIn.get();
					if(checkChar == 'w')
					{
						checkChar = fileIn.get();
						if(checkChar == 'm')
						{
							checkChar = fileIn.get();
							if(checkChar == 't')
							{
								checkChar = fileIn.get();
								if(checkChar == 'l')
								{
									checkChar = fileIn.get();
									if(checkChar == ' ')
									{
										//New material, set its defaults
										SurfaceMaterial tempMat;
										material.push_back(tempMat);
										fileIn >> material[matCount].matName;
										material[matCount].transparent = false;
										material[matCount].hasTexture = false;
										///////////////**************new**************////////////////////
										material[matCount].hasNormMap = false;
										material[matCount].normMapTexArrayIndex = 0;
										///////////////**************new**************////////////////////
										material[matCount].texArrayIndex = 0;
										matCount++;
										kdset = false;
									}
								}
							}
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}	
	else
	{
		SwapChain->SetFullscreenState(false, NULL);	//Make sure we are out of fullscreen

		std::wstring message = L"Could not open: ";
		message += meshMatLib;

		MessageBox(0, message.c_str(),
			L"Error", MB_OK);

		return false;
	}

	//Set the subsets material to the index value
	//of the its material in our material array
	for(int i = 0; i < meshSubsets; ++i)
	{
		bool hasMat = false;
		for(int j = 0; j < material.size(); ++j)
		{
			if(meshMaterials[i] == material[j].matName)
			{
				subsetMaterialArray.push_back(j);
				hasMat = true;
			}
		}
		if(!hasMat)
			subsetMaterialArray.push_back(0); //Use first material in array
	}

	std::vector<Vertex> vertices;
	Vertex tempVert;

	//Create our vertices using the information we got 
	//from the file and store them in a vector
	for(int j = 0 ; j < totalVerts; ++j)
	{
		tempVert.pos = vertPos[vertPosIndex[j]];
		tempVert.normal = vertNorm[vertNormIndex[j]];
		tempVert.texCoord = vertTexCoord[vertTCIndex[j]];

		vertices.push_back(tempVert);
		///////////////**************new**************////////////////////
	}

    //////////////////////Compute Normals///////////////////////////
    //If computeNormals was set to true then we will create our own
    //normals, if it was set to false we will use the obj files normals
    if(computeNormals)
    {
        std::vector<XMFLOAT3> tempNormal;

		//normalized and unnormalized normals
		XMFLOAT3 unnormalized = XMFLOAT3(0.0f, 0.0f, 0.0f);

		///////////////**************new**************////////////////////
		//tangent stuff
		std::vector<XMFLOAT3> tempTangent;
		XMFLOAT3 tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
		float tcU1, tcV1, tcU2, tcV2;
		///////////////**************new**************////////////////////
		//Used to get vectors (sides) from the position of the verts
		float vecX, vecY, vecZ;

		//Two edges of our triangle
		XMVECTOR edge1 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR edge2 = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		//Compute face normals
		for(int i = 0; i < meshTriangles; ++i)
		{
			//Get the vector describing one edge of our triangle (edge 0,2)
			vecX = vertices[indices[(i*3)]].pos.x - vertices[indices[(i*3)+2]].pos.x;
			vecY = vertices[indices[(i*3)]].pos.y - vertices[indices[(i*3)+2]].pos.y;
			vecZ = vertices[indices[(i*3)]].pos.z - vertices[indices[(i*3)+2]].pos.z;		
			edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our first edge

			//Get the vector describing another edge of our triangle (edge 2,1)
			vecX = vertices[indices[(i*3)+2]].pos.x - vertices[indices[(i*3)+1]].pos.x;
			vecY = vertices[indices[(i*3)+2]].pos.y - vertices[indices[(i*3)+1]].pos.y;
			vecZ = vertices[indices[(i*3)+2]].pos.z - vertices[indices[(i*3)+1]].pos.z;		
			edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);	//Create our second edge

			//Cross multiply the two edge vectors to get the un-normalized face normal
			XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

			tempNormal.push_back(unnormalized);

			///////////////**************new**************////////////////////
			//Find first texture coordinate edge 2d vector
			tcU1 = vertices[indices[(i*3)]].texCoord.x - vertices[indices[(i*3)+2]].texCoord.x;
			tcV1 = vertices[indices[(i*3)]].texCoord.y - vertices[indices[(i*3)+2]].texCoord.y;

			//Find second texture coordinate edge 2d vector
			tcU2 = vertices[indices[(i*3)+2]].texCoord.x - vertices[indices[(i*3)+1]].texCoord.x;
			tcV2 = vertices[indices[(i*3)+2]].texCoord.y - vertices[indices[(i*3)+1]].texCoord.y;

			//Find tangent using both tex coord edges and position edges
			tangent.x = (tcV1 * XMVectorGetX(edge1) - tcV2 * XMVectorGetX(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
			tangent.y = (tcV1 * XMVectorGetY(edge1) - tcV2 * XMVectorGetY(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));
			tangent.z = (tcV1 * XMVectorGetZ(edge1) - tcV2 * XMVectorGetZ(edge2)) * (1.0f / (tcU1 * tcV2 - tcU2 * tcV1));

			tempTangent.push_back(tangent);
			///////////////**************new**************////////////////////
		}

		//Compute vertex normals (normal Averaging)
		XMVECTOR normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		int facesUsing = 0;
		float tX, tY, tZ;	//temp axis variables

		//Go through each vertex
		for(int i = 0; i < totalVerts; ++i)
		{
			//Check which triangles use this vertex
			for(int j = 0; j < meshTriangles; ++j)
			{
				if(indices[j*3] == i ||
					indices[(j*3)+1] == i ||
					indices[(j*3)+2] == i)
				{
					tX = XMVectorGetX(normalSum) + tempNormal[j].x;
					tY = XMVectorGetY(normalSum) + tempNormal[j].y;
					tZ = XMVectorGetZ(normalSum) + tempNormal[j].z;

					normalSum = XMVectorSet(tX, tY, tZ, 0.0f);	//If a face is using the vertex, add the unormalized face normal to the normalSum
					///////////////**************new**************////////////////////		
					//We can reuse tX, tY, tZ to sum up tangents
					tX = XMVectorGetX(tangentSum) + tempTangent[j].x;
					tY = XMVectorGetY(tangentSum) + tempTangent[j].y;
					tZ = XMVectorGetZ(tangentSum) + tempTangent[j].z;

					tangentSum = XMVectorSet(tX, tY, tZ, 0.0f); //sum up face tangents using this vertex
					///////////////**************new**************////////////////////
					facesUsing++;
				}
			}

			//Get the actual normal by dividing the normalSum by the number of faces sharing the vertex
			normalSum = normalSum / facesUsing;
			///////////////**************new**************////////////////////
			tangentSum = tangentSum / facesUsing;
			///////////////**************new**************////////////////////

			//Normalize the normalSum vector
			normalSum = XMVector3Normalize(normalSum);
			///////////////**************new**************////////////////////
			tangentSum =  XMVector3Normalize(tangentSum);
			///////////////**************new**************////////////////////

			//Store the normal in our current vertex
			vertices[i].normal.x = XMVectorGetX(normalSum);
			vertices[i].normal.y = XMVectorGetY(normalSum);
			vertices[i].normal.z = XMVectorGetZ(normalSum);

			///////////////**************new**************////////////////////
			vertices[i].tangent.x = XMVectorGetX(tangentSum);
			vertices[i].tangent.y = XMVectorGetY(tangentSum);
			vertices[i].tangent.z = XMVectorGetZ(tangentSum);
			///////////////**************new**************////////////////////
			//Clear normalSum and facesUsing for next vertex
			normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			///////////////**************new**************////////////////////
			tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
			///////////////**************new**************////////////////////
			facesUsing = 0;

		}
	}

	//Create index buffer
	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * meshTriangles*3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, indexBuff);

	//Create Vertex Buffer
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * totalVerts;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = &vertices[0];
	hr = d3d11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, vertBuff);

	return true;
}
///////////////**************new**************////////////////////

void CreateSphere(int LatLines, int LongLines)
{
	NumSphereVertices = ((LatLines-2) * LongLines) + 2;
	NumSphereFaces  = ((LatLines-3)*(LongLines)*2) + (LongLines*2);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	std::vector<Vertex> vertices(NumSphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	vertices[0].pos.x = 0.0f;
	vertices[0].pos.y = 0.0f;
	vertices[0].pos.z = 1.0f;

	for(DWORD i = 0; i < LatLines-2; ++i)
	{
		spherePitch = (i+1) * (3.14f/(LatLines-1));
		Rotationx = XMMatrixRotationX(spherePitch);
		for(DWORD j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28f/(LongLines));
			Rotationy = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal( XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy) );	
			currVertPos = XMVector3Normalize( currVertPos );
			vertices[i*LongLines+j+1].pos.x = XMVectorGetX(currVertPos);
			vertices[i*LongLines+j+1].pos.y = XMVectorGetY(currVertPos);
			vertices[i*LongLines+j+1].pos.z = XMVectorGetZ(currVertPos);
		}
	}

	vertices[NumSphereVertices-1].pos.x =  0.0f;
	vertices[NumSphereVertices-1].pos.y =  0.0f;
	vertices[NumSphereVertices-1].pos.z = -1.0f;


	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( Vertex ) * NumSphereVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = &vertices[0];
	hr = d3d11Device->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


	std::vector<DWORD> indices(NumSphereFaces * 3);

	int k = 0;
	for(DWORD l = 0; l < LongLines-1; ++l)
	{
		indices[k] = 0;
		indices[k+1] = l+1;
		indices[k+2] = l+2;
		k += 3;
	}

	indices[k] = 0;
	indices[k+1] = LongLines;
	indices[k+2] = 1;
	k += 3;

	for(DWORD i = 0; i < LatLines-3; ++i)
	{
		for(DWORD j = 0; j < LongLines-1; ++j)
		{
			indices[k]   = i*LongLines+j+1;
			indices[k+1] = i*LongLines+j+2;
			indices[k+2] = (i+1)*LongLines+j+1;

			indices[k+3] = (i+1)*LongLines+j+1;
			indices[k+4] = i*LongLines+j+2;
			indices[k+5] = (i+1)*LongLines+j+2;

			k += 6; // next quad
		}

		indices[k]   = (i*LongLines)+LongLines;
		indices[k+1] = (i*LongLines)+1;
		indices[k+2] = ((i+1)*LongLines)+LongLines;

		indices[k+3] = ((i+1)*LongLines)+LongLines;
		indices[k+4] = (i*LongLines)+1;
		indices[k+5] = ((i+1)*LongLines)+1;

		k += 6;
	}

	for(DWORD l = 0; l < LongLines-1; ++l)
	{
		indices[k] = NumSphereVertices-1;
		indices[k+1] = (NumSphereVertices-1)-(l+1);
		indices[k+2] = (NumSphereVertices-1)-(l+2);
		k += 3;
	}

	indices[k] = NumSphereVertices-1;
	indices[k+1] = (NumSphereVertices-1)-LongLines;
	indices[k+2] = NumSphereVertices-2;

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * NumSphereFaces * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = &indices[0];
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

}
void InitD2DScreenTexture()
{
	//创建顶点缓冲
	Vertex v[] =
	{
		// Front Face
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f,-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f,-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
		Vertex( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f),
	};

	DWORD indices[] = {
		//字体面
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	d3d11Device->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer);

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = d3d11Device->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &d2dVertBuffer);

	//从纹理D2D，创建一个着色器资源视图
	//因此，能够使用它来创建一个矩形纹理，用于覆盖场景
	d3d11Device->CreateShaderResourceView(sharedTex11, NULL, &d2dTexture);

}

//void ReleaseObjects()
//{
//释放创建的COM对象
//	SwapChain->Release();
//	d3d11Device->Release();
//	d3d11DevCon->Release();
//}

bool InitScene()
{
	//
	InitD2DScreenTexture();
	//编译着色器
	CreateSphere(10, 10);

	///////////////**************new**************////////////////////
    if(!LoadObjModel(L"ground.obj", &meshVertBuff, &meshIndexBuff, meshSubsetIndexStart, meshSubsetTexture, material, meshSubsets, true, true))
        return false;

    ///////////////**************new**************////////////////////
    if(!LoadMD5Model(L"boy.md5mesh", NewMD5Model, meshSRV, textureNameArray))
		return false;
	///////////////**************new**************////////////////////
	    ///////////////**************new**************////////////////////    
    if(!LoadMD5Anim(L"boy.md5anim", NewMD5Model))
        return false;

    // 获取包围体信息
   // CreateBoundingVolumes(bottleVertPosArray, bottleBoundingBoxVertPosArray, bottleBoundingBoxVertIndexArray, bottleBoundingSphere, bottleCenterOffset);
	///////////////**************new**************////////////////////
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, 0, &VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, 0, &PS_Buffer, 0, 0);
	/// new
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "D2D_PS", "ps_4_0", 0, 0, 0, &D2D_PS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_VS", "vs_4_0", 0, 0, 0, &SKYMAP_VS_Buffer, 0, 0);
	hr = D3DX11CompileFromFile(L"Effects.fx", 0, 0, "SKYMAP_PS", "ps_4_0", 0, 0, 0, &SKYMAP_PS_Buffer, 0, 0);

	///////////////**************new**************////////////////////
	//创建着色器对象
	hr = d3d11Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = d3d11Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);
	///new
	hr = d3d11Device->CreatePixelShader(D2D_PS_Buffer->GetBufferPointer(), D2D_PS_Buffer->GetBufferSize(), NULL, &D2D_PS);
	hr = d3d11Device->CreateVertexShader(SKYMAP_VS_Buffer->GetBufferPointer(), SKYMAP_VS_Buffer->GetBufferSize(), NULL, &SKYMAP_VS);
	hr = d3d11Device->CreatePixelShader(SKYMAP_PS_Buffer->GetBufferPointer(), SKYMAP_PS_Buffer->GetBufferSize(), NULL, &SKYMAP_PS);
	///////////////**************new**************////////////////////
	///////////////**************new**************////////////////////
	//设置顶点和像素着色器
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

	///////////////**************new**************////////////////////
	light.pos = XMFLOAT3(0.0f, 7.0f, 0.0f);
	light.dir = XMFLOAT3(0.5f, 0.75f, -0.5f);
	light.range = 1000.0f;
	light.cone = 12.0f;
	light.att = XMFLOAT3(0.4f, 0.02f, 0.000f);
	light.ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	//Create the Input Layout
	//创建输入布局
	d3d11Device->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(),
		VS_Buffer->GetBufferSize(), &vertLayout);

	//设置输入布局
	d3d11DevCon->IASetInputLayout(vertLayout);
	
	//设置图元拓扑
	d3d11DevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//创建视口
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Width;
	viewport.Height = Height;
	
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	//设置视口
	d3d11DevCon->RSSetViewports(1, &viewport);

	//创建缓冲用来发送到效果文件的cbuffer
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerObjectBuffer);

	//创建缓冲用于每帧发送cbuffer到着色器文件
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));
	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(cbPerFrame);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	d3d11Device->CreateBuffer(&cbbd, NULL, &cbPerFrameBuffer);

	//相机信息
	//相机信息
	camPosition = XMVectorSet( 0.0f, 5.0f, -8.0f, 0.0f );
	//camPosition = XMVectorSet(0.0f, 0.0f, -0.5f, 0.0f);
	camTarget = XMVectorSet( 0.0f, 0.5f, 0.0f, 0.0f );
	camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//设置视图矩阵
	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);

	//设置投影矩阵
	camProjection = XMMatrixPerspectiveFovLH(0.4f*3.14f, (float)Width / Height, 1.0f, 1000.0f);

	D3D11_BLEND_DESC blendDesc;
    ZeroMemory( &blendDesc, sizeof(blendDesc) );

    D3D11_RENDER_TARGET_BLEND_DESC rtbd;
    ZeroMemory( &rtbd, sizeof(rtbd) );

    rtbd.BlendEnable             = true;
    rtbd.SrcBlend                 = D3D11_BLEND_SRC_COLOR;
    ///////////////**************new**************////////////////////
    rtbd.DestBlend                 = D3D11_BLEND_INV_SRC_ALPHA;
    ///////////////**************new**************////////////////////
    rtbd.BlendOp                 = D3D11_BLEND_OP_ADD;
    rtbd.SrcBlendAlpha             = D3D11_BLEND_ONE;
    rtbd.DestBlendAlpha             = D3D11_BLEND_ZERO;
    rtbd.BlendOpAlpha             = D3D11_BLEND_OP_ADD;
    rtbd.RenderTargetWriteMask     = D3D10_COLOR_WRITE_ENABLE_ALL;

    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0] = rtbd;
	
	d3d11Device->CreateBlendState(&blendDesc, &d2dTransparency);
	
	///////////////**************new**************////////////////////
	ZeroMemory( &rtbd, sizeof(rtbd) );

	rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.DestBlend				 = D3D11_BLEND_SRC_ALPHA;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_SRC_ALPHA;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	d3d11Device->CreateBlendState(&blendDesc, &Transparency);
	///////////////**************new**************////////////////////
	//加载图像纹理
	//hr = 
//#if 1
//	hr = D3DX11CreateShaderResourceViewFromFile(d3d11Device, L"grass.jpg",
//	NULL, NULL, &CubesTexture, NULL);
	///////////////**************new**************////////////////////
	//告诉D3D我们正在加载一个立方体纹理
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	//加载纹理
	ID3D11Texture2D* SMTexture = 0;
	hr = D3DX11CreateTextureFromFile(d3d11Device, L"skymap.dds",
		&loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);

	//创建纹理描述符
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	//告诉D3D我们有一个立方体纹理,它是一个2D纹理的数组
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	//创建资源视图
	hr = d3d11Device->CreateShaderResourceView(SMTexture, &SMViewDesc, &smrv);
	///////////////**************new**************////////////////////

	//配置采样状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//创建采样状态
	hr = d3d11Device->CreateSamplerState(&sampDesc, &CubesTexSamplerState);


	//d3d11Device->CreateBlendState(&blendDesc, &Transparency);

	//创建逆时针和顺时针状态
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &CWcullMode);

	///////////////**************new**************////////////////////
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = d3d11Device->CreateRasterizerState(&cmdesc, &RSCullNone);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	d3d11Device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

    return true;
}

///////////////**************new**************////////////////////
void StartTimer()
{
    LARGE_INTEGER frequencyCount;
    QueryPerformanceFrequency(&frequencyCount);
    countsPerSecond = double(frequencyCount.QuadPart);

    QueryPerformanceCounter(&frequencyCount);
    CounterStart = frequencyCount.QuadPart;
}

double GetTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return double(currentTime.QuadPart-CounterStart)/countsPerSecond;
}

double GetFrameTime()
{
    LARGE_INTEGER currentTime;
    __int64 tickCount;
    QueryPerformanceCounter(&currentTime);

    tickCount = currentTime.QuadPart-frameTimeOld;
    frameTimeOld = currentTime.QuadPart;

    if(tickCount < 0.0f)
        tickCount = 0.0f;

    return float(tickCount)/countsPerSecond;
}
///////////////**************new**************////////////////////

///////////////**************new**************////////////////////
void UpdateScene(double time)
    ///////////////**************new**************////////////////////
//void UpdateScene()
{
	//Reset cube1World
//	groundWorld = XMMatrixIdentity();

	//Define cube1's world space matrix
	///////////////**************new**************////////////////////
//	Scale = XMMatrixScaling( 500.0f, 10.0f, 500.0f );
//	Translation = XMMatrixTranslation( 0.0f, 10.0f, 0.0f );

	//Set cube1's world space using the transformations
//	groundWorld = Scale * Translation;

	//复位球面世界
	sphereWorld = XMMatrixIdentity();

	//Define sphereWorld's world space matrix
	Scale = XMMatrixScaling( 5.0f, 5.0f, 5.0f );
	//Make sure the sphere is always centered around camera
	Translation = XMMatrixTranslation( XMVectorGetX(camPosition), XMVectorGetY(camPosition), XMVectorGetZ(camPosition) );

	//Set sphereWorld's world space using the transformations
	sphereWorld = Scale * Translation;

	///////////////**************new**************////////////////////
	meshWorld = XMMatrixIdentity();

	//Define cube1's world space matrix
	Rotation = XMMatrixRotationY(3.14f);
	Scale = XMMatrixScaling( 1.0f, 1.0f, 1.0f );
	Translation = XMMatrixTranslation( 0.0f, 0.0f, 0.0f );

	meshWorld = Rotation * Scale * Translation;
    ///////////////**************new**************////////////////////
    Scale = XMMatrixScaling( 0.04f, 0.04f, 0.04f );            // 将模型尺寸缩小
    Translation = XMMatrixTranslation( 0.0f, 3.0f, 0.0f );
    smilesWorld = Scale * Translation;
    ///////////////**************new**************////////////////////
}

///////////////**************new**************////////////////////
void RenderText(std::wstring text, int inInt)
//void RenderText(std::wstring text)
{
	//释放D3D11设备
    d3d11DevCon->PSSetShader(D2D_PS, 0, 0);
	keyedMutex11->ReleaseSync(0);

	//使用D3D10.1设备
	keyedMutex10->AcquireSync(0, 5);

	//绘制D2D内容
	D2DRenderTarget->BeginDraw();
	//清空D2D背景色
	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));


	//创建字符串
	std::wostringstream printString;
    ///////////////**************new**************////////////////////
    printString << text << inInt;
    printText = printString.str();

	//设置字体颜色
    D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);

	//设置D2D绘制要用到的画笔颜色
	Brush->SetColor(FontColor);

	//创建D2D渲染区域
	D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, Width, Height);

	//绘制文本
	D2DRenderTarget->DrawText(
		printText.c_str(),
		wcslen(printText.c_str()),
        TextFormat,
		layoutRect,
		Brush
		);

	D2DRenderTarget->EndDraw();

	//释放D3D10.1设备
	keyedMutex10->ReleaseSync(1);

	//使用D3D11设备
	keyedMutex11->AcquireSync(1, 5);

	//使用着色器资源表示d2d渲染目标来创建一个矩形纹理，该矩形是被渲染进屏幕空间的。使用α混合以便整个D2D
	//渲染目标的背景为不可见的，且只有使用D2D绘制的东西才可见（文本）。

	//为D2D渲染目标纹理对象设置混合状态
	d3d11DevCon->OMSetBlendState(d2dTransparency, NULL, 0xffffffff);
   //Set d2d's pixel shader so lighting calculations are not done
  //  d3d11DevCon->PSSetShader(D2D_PS, 0, 0);
	//设置d2d索引缓冲
 
	d3d11DevCon->IASetIndexBuffer(d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//设置d2d顶点缓冲
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	d3d11DevCon->IASetVertexBuffers(0, 1, &d2dVertBuffer, &stride, &offset);

	WVP = XMMatrixIdentity();
	///new
//	cbPerObj.World = XMMatrixTranspose(WVP);

	cbPerObj.WVP = XMMatrixTranspose(WVP);
	d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
	d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	d3d11DevCon->PSSetShaderResources(0, 1, &d2dTexture);
	d3d11DevCon->PSSetSamplers(0, 1, &CubesTexSamplerState);

	d3d11DevCon->RSSetState(CWcullMode);

	//画第二个立方体
	d3d11DevCon->DrawIndexed(6, 0, 0);

}

void DrawScene()
{
	//将更新的颜色填充后缓冲
//	D3DXCOLOR bgColor(red, green, blue, 1.0f);
//	float bgColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    float bgColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };

	d3d11DevCon->ClearRenderTargetView(renderTargetView, bgColor);

	//刷新深度模板视图
	d3d11DevCon->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//new
	constbuffPerFrame.light = light;
	d3d11DevCon->UpdateSubresource(cbPerFrameBuffer, 0, NULL, &constbuffPerFrame, 0, 0);
	d3d11DevCon->PSSetConstantBuffers(0, 1, &cbPerFrameBuffer);
	//复位顶点和像素着色器
//	d3d11DevCon->VSSetShader(VS, 0, 0);
//	d3d11DevCon->PSSetShader(PS, 0, 0);

	//使能默认光栅化状态
//	d3d11DevCon->RSSetState(NULL);


	//绘制使用背面裁剪的对象
	//关闭背面裁剪
   // d3d11DevCon->RSSetState(noCull);
	d3d11DevCon->OMSetRenderTargets( 1, &renderTargetView, depthStencilView );

	d3d11DevCon->OMSetBlendState(0, 0, 0xffffffff);
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);

    //Set the cubes index buffer
	//设置立方体的索引缓冲
//	d3d11DevCon->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//设置立方体的顶点缓冲
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
//	d3d11DevCon->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);
    ///////////////**************new**************////////////////////
    ///***绘制MD5模型***///
    for(int i = 0; i < NewMD5Model.numSubsets; i ++)
    {
        //设置地面索引缓冲
        d3d11DevCon->IASetIndexBuffer( NewMD5Model.subsets[i].indexBuff, DXGI_FORMAT_R32_UINT, 0);
        //设置地面顶点缓冲
        d3d11DevCon->IASetVertexBuffers( 0, 1, &NewMD5Model.subsets[i].vertBuff, &stride, &offset );

        //设置WVP矩阵并将它传入到效果文件的常量缓冲
        WVP = smilesWorld * camView * camProjection;
        cbPerObj.WVP = XMMatrixTranspose(WVP);    
        cbPerObj.World = XMMatrixTranspose(smilesWorld);    
        cbPerObj.hasTexture = true;        //假设所有的md5蒙皮子集都有纹理
        cbPerObj.hasNormMap = false;    // 假设md5模型没有法线贴图（随后若要修改也会很容易）
        d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
        d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
        d3d11DevCon->PSSetConstantBuffers( 1, 1, &cbPerObjectBuffer );
        d3d11DevCon->PSSetShaderResources( 0, 1, &meshSRV[NewMD5Model.subsets[i].texArrayIndex] );
        d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

        d3d11DevCon->RSSetState(RSCullNone);
        d3d11DevCon->DrawIndexed( NewMD5Model.subsets[i].indices.size(), 0, 0 );

    }
    ///////////////**************new**************////////////////////
	//绘制我们模型的非透明子集
	for(int i = 0; i < meshSubsets; ++i)
	{
		//设置地面索引缓冲
		d3d11DevCon->IASetIndexBuffer( meshIndexBuff, DXGI_FORMAT_R32_UINT, 0);
		//设置地面顶点缓冲
		d3d11DevCon->IASetVertexBuffers( 0, 1, &meshVertBuff, &stride, &offset );

		//设置WVP矩阵并发送它到效果文件中的常量缓冲中
		WVP = meshWorld * camView * camProjection;
		cbPerObj.WVP = XMMatrixTranspose(WVP);	
		cbPerObj.World = XMMatrixTranspose(meshWorld);	
		cbPerObj.difColor = material[meshSubsetTexture[i]].difColor;
		cbPerObj.hasTexture = material[meshSubsetTexture[i]].hasTexture;
		///////////////**************new**************////////////////////
		cbPerObj.hasNormMap = material[meshSubsetTexture[i]].hasNormMap;
		///////////////**************new**************////////////////////
		d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
		d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
		d3d11DevCon->PSSetConstantBuffers( 1, 1, &cbPerObjectBuffer );
		if(material[meshSubsetTexture[i]].hasTexture)
			d3d11DevCon->PSSetShaderResources( 0, 1, &meshSRV[material[meshSubsetTexture[i]].texArrayIndex] );
		///////////////**************new**************////////////////////
		if(material[meshSubsetTexture[i]].hasNormMap)
			d3d11DevCon->PSSetShaderResources( 1, 1, &meshSRV[material[meshSubsetTexture[i]].normMapTexArrayIndex] );
		///////////////**************new**************////////////////////
		d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

		d3d11DevCon->RSSetState(RSCullNone);
		int indexStart = meshSubsetIndexStart[i];
		int indexDrawAmount =  meshSubsetIndexStart[i+1] - meshSubsetIndexStart[i];
		if(!material[meshSubsetTexture[i]].transparent)
			d3d11DevCon->DrawIndexed( indexDrawAmount, indexStart, 0 );
	}

    /////Draw the Sky's Sphere//////
    //Set the spheres index buffer
	/////绘制天空的球面//////
	//设置球面的索引缓冲
	d3d11DevCon->IASetIndexBuffer( sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//设置球面的顶点缓冲
	d3d11DevCon->IASetVertexBuffers( 0, 1, &sphereVertBuffer, &stride, &offset );

	//设置WVP矩阵并将它发送给效果文件中的常量缓冲
	WVP = sphereWorld * camView * camProjection;
	cbPerObj.WVP = XMMatrixTranspose(WVP);	
	cbPerObj.World = XMMatrixTranspose(sphereWorld);	
	d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
	d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
	//发送我们的天空贴图资源视图到像素着色器
	d3d11DevCon->PSSetShaderResources( 0, 1, &smrv );
	d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

	//设置新的VS和PS着色器
	d3d11DevCon->VSSetShader(SKYMAP_VS, 0, 0);
	d3d11DevCon->PSSetShader(SKYMAP_PS, 0, 0);
	//设置新的深度模板和RS状态
	d3d11DevCon->OMSetDepthStencilState(DSLessEqual, 0);
	d3d11DevCon->RSSetState(RSCullNone);
	d3d11DevCon->DrawIndexed( NumSphereFaces * 3, 0, 0 );	

	//设置默认的VS，PS着色器和深度模板状态
	d3d11DevCon->VSSetShader(VS, 0, 0);
	d3d11DevCon->PSSetShader(PS, 0, 0);
	d3d11DevCon->OMSetDepthStencilState(NULL, 0);

	///////////////**************new**************////////////////////	
	//绘制我们的模型的透明度子集
	//设置我们的混合状态
	d3d11DevCon->OMSetBlendState(Transparency, NULL, 0xffffffff);

	for(int i = 0; i < meshSubsets; ++i)
	{
		//设置地面索引缓冲
		d3d11DevCon->IASetIndexBuffer( meshIndexBuff, DXGI_FORMAT_R32_UINT, 0);
		//设置地面顶点缓冲
		d3d11DevCon->IASetVertexBuffers( 0, 1, &meshVertBuff, &stride, &offset );

		//设置WVP矩阵并将它发送给效果文件中的常量缓冲中
		WVP = meshWorld * camView * camProjection;
		cbPerObj.WVP = XMMatrixTranspose(WVP);	
		cbPerObj.World = XMMatrixTranspose(meshWorld);	
		cbPerObj.difColor = material[meshSubsetTexture[i]].difColor;
		cbPerObj.hasTexture = material[meshSubsetTexture[i]].hasTexture;
		///////////////**************new**************////////////////////
		cbPerObj.hasNormMap = material[meshSubsetTexture[i]].hasNormMap;
		///////////////**************new**************////////////////////
		d3d11DevCon->UpdateSubresource( cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0 );
		d3d11DevCon->VSSetConstantBuffers( 0, 1, &cbPerObjectBuffer );
		d3d11DevCon->PSSetConstantBuffers( 1, 1, &cbPerObjectBuffer );
		if(material[meshSubsetTexture[i]].hasTexture)
			d3d11DevCon->PSSetShaderResources( 0, 1, &meshSRV[material[meshSubsetTexture[i]].texArrayIndex] );
		///////////////**************new**************////////////////////
		if(material[meshSubsetTexture[i]].hasNormMap)
			d3d11DevCon->PSSetShaderResources( 1, 1, &meshSRV[material[meshSubsetTexture[i]].normMapTexArrayIndex] );
		///////////////**************new**************////////////////////
		d3d11DevCon->PSSetSamplers( 0, 1, &CubesTexSamplerState );

		d3d11DevCon->RSSetState(RSCullNone);
		int indexStart = meshSubsetIndexStart[i];
		int indexDrawAmount =  meshSubsetIndexStart[i+1] - meshSubsetIndexStart[i];
		if(material[meshSubsetTexture[i]].transparent)
			d3d11DevCon->DrawIndexed( indexDrawAmount, indexStart, 0 );
	}
	///////////////**************new**************////////////////////	
	RenderText(L"FPS: ", fps);

	//Present the backbuffer to the screen
	SwapChain->Present(0, 0);
}

int messageloop(){
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));//清除结构体被设为NULL。

	while (true)
	{
		//使用PeekMessage()检查是否有消息传进来
		/*LPMSG lpMsg 消息结构体的指针
		*HWND hWnd 发送消息的窗口句柄。若设为NULL，那么它会从当前程序中接收来自任何一个窗口的消息
		*UINT wMsgFilterMin 指定消息范围内第一个要检查的消息的值。若wMsgFilterMin和wMsgFilterMax都设为0，那么PeekMessage将会检查素有的消息
		*UINT wMsgFilterMax 指定消息范围内最后一个要检测的消息的值
		*UINT wRemoveMsg 指定消息的处理方式。若设置为PM_REMOVE，则在读取之后会被删除
		*/
		BOOL PeekMessageL( 
            LPMSG lpMsg,
            HWND hWnd,
            UINT wMsgFilterMin,
            UINT wMsgFilterMax,
            UINT wRemoveMsg
            );

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
                break;
			//若消息为窗口消息，则解析并分发它。TranslateMessage()将会让窗口做一些解析，类似键盘的虚拟键值转换到字符形式。
			//而DispatchMessage()则发送消息到窗口过程WndProc。
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else //若没有窗口消息，则运行游戏
 ///////////////**************new**************////////////////////
		{
  			frameCount++;
 	 		if(GetTime() > 1.0f)
            {
                fps = frameCount;
                frameCount = 0;
                StartTimer();
            }    

            frameTime = GetFrameTime();
		
            ///////////////**************new**************////////////////////
            DetectInput(frameTime);
            ///////////////**************new**************////////////////////
            UpdateScene(frameTime);
			DrawScene();

		}
	}
	return msg.wParam;
}

//窗口消息处理函数
//HWND hwnd 获取消息的窗口句柄
//UINT msg 消息的内容
/*
*WM_ACTIVE 当窗口激活时发送的消息
*WM_CLOSE 当窗口关闭时发送的消息
*WM_CREATE 当窗口创建时发送的消息
*WM_DESTROY 当窗口销毁时发送的消息
*/
//wParam和lParam时消息的额外信息。使用wParam来检测键盘输入消息
LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	// 这是事件检测消息的地方，若escape键被按下，会显示一个消息框，询问是否真的退出。若点击yes，则程序关闭。若不点击，则消息框关闭。若消息包含WM_DESTROY
	// 则意味着窗口正在被销毁，返回0并且程序关闭
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, L"Are you sure you want to exit?",
				L"Really?", MB_YESNO | MB_ICONASTERISK) == IDYES)
			{
				DestroyWindow(hwnd);
			}
			return 0;

		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
		///////////////**************new**************////////////////////
	//case WM_SIZE:
	//	ClientWidth  = LOWORD(lParam);
	//	ClientHeight = HIWORD(lParam);
	//	return 0;
	//	break;
	default:
		break;
	}

	//调用默认窗口过程函数
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}