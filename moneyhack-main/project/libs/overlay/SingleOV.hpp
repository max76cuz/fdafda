#pragma once
#ifndef SINGLEOV_H
#define SINGLEOV_H
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <dwmapi.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <thread>

// DX11 SDK -> https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Linker -> Input "d3d11.lib;d3dcompiler.lib;dxgi.lib;%(AdditionalDependencies)"


extern "C"
{
    inline __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    inline __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace overlay
{
    // ** var
    HWND Window;
    WNDCLASSEXA wcex;

    static ULONG G_Width = GetSystemMetrics(SM_CXSCREEN);
    static ULONG G_Height = GetSystemMetrics(SM_CYSCREEN);

    static ID3D11Device* g_pd3dDevice = nullptr;
    static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    static IDXGISwapChain* g_pSwapChain = nullptr;
    static bool                     g_SwapChainOccluded = false;
    static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    static bool ShouldQuit;
    static bool ShouldMouseLoop = true;

    ImDrawList* DrawList;


    // no mouse input if u don't have this 
    VOID MouseLoop()
    {
        while (overlay::ShouldMouseLoop)
        {
            ImGuiIO& io = ImGui::GetIO();
            POINT xy = { 0 };
            POINT p;
            GetCursorPos(&p);
            io.MousePos.x = p.x - xy.x;
            io.MousePos.y = p.y - xy.y;

            if (GetAsyncKeyState(0x1)) {
                io.MouseDown[0] = true;
                io.MouseClicked[0] = true;
                // io.MouseClickedPos[0].x = io.MousePos.x;
               //  io.MouseClickedPos[0].y = io.MousePos.y;
            }
            else io.MouseDown[0] = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(15));
        }
    }

    // - For rectangular primitives, "p_min" and "p_max" represent the upper-left and lower-right corners. 
    VOID CreateRenderTarget();
    bool SetupWindow();
    bool CreateDeviceD3D(HWND hWnd);
    VOID CleanupDeviceD3D();
    VOID CloseOverlay();
    VOID Render();
    VOID EndRender();
}

void overlay::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    overlay::g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    if (pBackBuffer)
    {
        overlay::g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &overlay::g_mainRenderTargetView);
    }
    pBackBuffer->Release();
}
bool overlay::CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 144;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    overlay::CreateRenderTarget();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;

    ImGui_ImplWin32_Init(overlay::Window);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&overlay::MouseLoop, NULL, NULL, NULL);
    if (hThread)
    {
        CloseHandle(hThread);
    }

    SetWindowPos(overlay::Window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return true;
}
bool overlay::SetupWindow()
{
    overlay::wcex = {
        sizeof(WNDCLASSEXA),
        0,
        DefWindowProcA,
        0,
        0,
        nullptr,
        LoadIcon(nullptr, IDI_APPLICATION),
        LoadCursor(nullptr, IDC_ARROW),
        nullptr,
        nullptr,
        ("moneyhack"),
        LoadIcon(nullptr, IDI_APPLICATION)
    };

    RECT Rect;

    GetWindowRect(GetDesktopWindow(), &Rect);

    RegisterClassExA(&wcex);

    overlay::Window = CreateWindowExA(NULL, ("moneyhack"), ("moneyhack"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);
    SetWindowLong(overlay::Window, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
    SetLayeredWindowAttributes(overlay::Window, RGB(0, 0, 0), 255, NULL);

    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(overlay::Window, &margin);

    ShowWindow(overlay::Window, SW_SHOW);
    UpdateWindow(overlay::Window);

    if (!(CreateDeviceD3D(overlay::Window)))
        return false;

    return true;
}
void overlay::CleanupDeviceD3D()
{
    if (overlay::g_mainRenderTargetView) { overlay::g_mainRenderTargetView->Release(); overlay::g_mainRenderTargetView = nullptr; }
    if (overlay::g_pSwapChain) { overlay::g_pSwapChain->Release(); overlay::g_pSwapChain = nullptr; }
    if (overlay::g_pd3dDeviceContext) { overlay::g_pd3dDeviceContext->Release(); overlay::g_pd3dDeviceContext = nullptr; }
    if (overlay::g_pd3dDevice) { overlay::g_pd3dDevice->Release(); overlay::g_pd3dDevice = nullptr; }
}
void overlay::CloseOverlay()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    CleanupDeviceD3D();
    ::DestroyWindow(overlay::Window);
    ::UnregisterClassA(overlay::wcex.lpszClassName, overlay::wcex.hInstance);
}
VOID overlay::Render()
{
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
        if (msg.message == WM_QUIT)
            overlay::ShouldQuit = true;
    }
    if (overlay::ShouldQuit)
    {
        overlay::CloseOverlay();
    }


    overlay::g_SwapChainOccluded = false;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}
VOID overlay::EndRender()
{
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);
    const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
    ImGui::Render();
    overlay::g_pd3dDeviceContext->OMSetRenderTargets(1, &overlay::g_mainRenderTargetView, nullptr);
    overlay::g_pd3dDeviceContext->ClearRenderTargetView(overlay::g_mainRenderTargetView, clear_color_with_alpha);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    HRESULT hr = g_pSwapChain->Present(1, 0);
    overlay::g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
}
#endif // !SINGLEOV_H

