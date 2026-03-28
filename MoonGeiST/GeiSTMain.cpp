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


/// System headers
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN /// exclude rarely-used stuff from Windows headers
#endif
#include <windows.h>
/// Library headers
#include "GeiSTApplication.hpp"
#include "GeiSTRenderLoop.hpp"


int WINAPI WinMain(
    const HINSTANCE hInstance,
    const HINSTANCE hPrevInstance,
    const LPSTR     lpCmdLine,
    const int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    GeiSTRenderLoop renderLoop(1280, 720, L"SKY-DoG");
    return GeiSTApplication::Run(&renderLoop, hInstance, nCmdShow);
}
