#include <windows.h>

// 2、窗口过程函数
LRESULT CALLBACK WindowProc(  HWND hwnd,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam )
{
    //判断消息ID
    switch (uMsg)
    {
        case WM_DESTROY:    // 窗口销毁消息
            PostQuitMessage( 0 );   //  发送退出消息
            return 0;
    }
    // 其他的消息调用缺省的消息处理程序
    return DefWindowProc( hwnd, uMsg, wParam, lParam );
}

// 3、注册窗口类型
BOOL RegisterWindow( LPCSTR lpcWndName, HINSTANCE hInstance )
{
    ATOM nAtom = 0;

    // 构造创建窗口参数
    WNDCLASS wndClass = { 0 };
    
    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WindowProc;      // 指向窗口过程函数
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( 100 ) );// 100为在HelloWorld.rc文件中定义的ID号
    wndClass.hCursor = NULL;
    wndClass.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = lpcWndName;    // 注册的窗口名称，并非标题，以后创建窗口根据此注册的名称创建
    
    nAtom = RegisterClass( &wndClass );
    if( 0 == nAtom)
    {
        MessageBox( NULL, "Register Failed",
                    "Error", MB_OK|MB_ICONERROR );
        return FALSE;
    }
    else
    {
        MessageBox( NULL, "Register Successed",
                    "Sucessed", MB_OK );
    }
    return TRUE;
}

// 4、创建窗口（lpClassName 一定是已经注册过的窗口类型）
HWND CreateMyWindow( LPCTSTR lpClassName, HINSTANCE hInstance )
{
    HWND hWnd = NULL;
    // 创建窗口
    hWnd = CreateWindow( lpClassName, "Hello World",
                  WS_OVERLAPPEDWINDOW,
                  0, 0, 500, 300,
                  NULL, NULL, hInstance, NULL );
    if( NULL == hWnd )
    {
        MessageBox( NULL, "Create Window Failed",
                    "Error", MB_OK|MB_ICONERROR );
        return NULL;
    }
    else
    {
        MessageBox( NULL, "Create Window Successed",
                    "Sucessed", MB_OK );
    }
    return hWnd;
}

// 5、 显示窗口
void DisplayMyWnd( HWND hWnd )
{
    ShowWindow( hWnd, SW_SHOW );
    UpdateWindow( hWnd );
}

void doMessage()        // 消息循环处理函数
{
    MSG msg = { 0 };
    // 获取消息
    while( GetMessage( &msg, NULL, 0, 0 ) ) // 当接收到WM_QIUT消息时，GetMessage函数返回0，结束循环
    {
        DispatchMessage( &msg ); // 派发消息，到WindowPro函数处理
    }
}


// 1、 入口函数
int WINAPI WinMain( HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine,
                    int nShowCmd )
{
    HWND hWnd = NULL;
    LPCTSTR lpClassName = "MyWnd";  // 注册窗口的名称
    RegisterWindow( lpClassName, hInstance );
    hWnd = CreateMyWindow( lpClassName, hInstance );

    DisplayMyWnd( hWnd );
    doMessage();
    
    return 0;
}

