//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************


/// Library headers
#include "GeiSTRenderLoop.hpp"
/// Self header
#include "GeiSTApplication.hpp"


HWND GeiSTApplication::m_hwnd = nullptr;


HWND GeiSTApplication::GetHwnd()
{
    return m_hwnd;
}


int GeiSTApplication::Run(
    GeiSTRenderLoop * const renderLoop,
    const HINSTANCE         hInstance,
    const int               nCmdShow)
{
    /// 解析命令行
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    renderLoop->ParseCommandLineArgs(argv, argc);
    LocalFree(argv);

    /// 注册Window Class
    WNDCLASSEX windowClass{ 0 };
    windowClass.cbSize = sizeof(WNDCLASSEX);

    windowClass.style           = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc     = WindowProc;
    windowClass.hInstance       = hInstance;
    windowClass.hCursor         = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName   = L"SKY-DoG-Class";
    RegisterClassEx(&windowClass);

    RECT windowRect
    {
        0, 0,
        (LONG)renderLoop->GetWidth(), (LONG)renderLoop->GetHeight()
    };
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    /// 创建主窗口
    m_hwnd = CreateWindow(
        windowClass.lpszClassName,
        renderLoop->GetTitle(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,        // We have no parent window.
        nullptr,        // We aren't using menus.
        hInstance,
        renderLoop);

    /// 初始化RenderLoop
    renderLoop->OnInit();

    ShowWindow(m_hwnd, nCmdShow);

    /// 主消息Loop
    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    /// 清理RenderLoop
    renderLoop->OnDestroy();

    /// Return this part of the WM_QUIT message to Windows
    return (int)msg.wParam;
}


/// 消息循环
///
///  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
///  WM_COMMAND  - Menu处理
///  WM_PAINT    - 绘制主窗体
///  WM_DESTROY  - Post退出消息
///
LRESULT CALLBACK GeiSTApplication::WindowProc(
    const HWND   hWnd,
    const UINT   message,
    const WPARAM wParam,
    const LPARAM lParam)
{
    GeiSTRenderLoop * const geiST = reinterpret_cast<GeiSTRenderLoop*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    switch (message)
    {
        case WM_CREATE:
        {
            // Save the DXSample* passed in to CreateWindow.
            LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            return 0;
        }

        case WM_KEYDOWN:
        {
            if (geiST)
            {
                geiST->OnKeyDown(static_cast<UINT8>(wParam));
            }
            return 0;
        }

        case WM_KEYUP:
        {
            if (geiST)
            {
                geiST->OnKeyUp(static_cast<UINT8>(wParam));
            }
            return 0;
        }

        case WM_PAINT:
        {
            if (geiST)
            {
                geiST->OnUpdate();
                geiST->OnRender();
            }
            return 0;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

        default:
        {
            /// 让系统缺省处理为止消息
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
}
