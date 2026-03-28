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

#pragma once


/// System headers
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN /// exclude rarely-used stuff from Windows headers
#endif
#include <windows.h>


class GeiSTRenderLoop;


class GeiSTApplication
{
public:
    static HWND GetHwnd();

    static int Run(
        GeiSTRenderLoop * const geiST,
        const HINSTANCE         hInstance,
        const int               nCmdShow);


protected:
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
    /// 主窗口句柄
    static HWND m_hwnd;
};
