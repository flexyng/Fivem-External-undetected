// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "../../framework/settings/functions.h"
#include "../../framework/data/fonts.h"
#include "../../framework/data/images.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_freetype.h"
#include <d3d11.h>
#include <tchar.h>
#include <d3dx11.h>
#pragma comment(lib, "d3dx11.lib")

// Data
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(var->device_dx11, var->device_context);

    // Load Fonts

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && var->swap_chain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            var->swap_chain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        if (var->dpi_changed)
        {
            var->dpi = var->stored_dpi / 100.f;

            ImFontConfig cfg;
            cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_Bitmap;
            cfg.FontDataOwnedByAtlas = false;

            io.Fonts->Clear();

            var->font.inter[0] = io.Fonts->AddFontFromMemoryTTF(inter_semibold_hex, sizeof inter_semibold_hex, SCALE(12.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.inter[1] = io.Fonts->AddFontFromMemoryTTF(inter_medium_hex, sizeof inter_medium_hex, SCALE(12.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.inter[2] = io.Fonts->AddFontFromMemoryTTF(inter_medium_hex, sizeof inter_medium_hex, SCALE(10.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.inter[3] = io.Fonts->AddFontFromMemoryTTF(inter_semibold_hex, sizeof inter_semibold_hex, SCALE(13.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());

            var->font.icons[0] = io.Fonts->AddFontFromMemoryTTF(logotype_hex, sizeof logotype_hex, SCALE(30.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[1] = io.Fonts->AddFontFromMemoryTTF(section_icons_hex, sizeof section_icons_hex, SCALE(18.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[2] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof icons_hex, SCALE(10.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[3] = io.Fonts->AddFontFromMemoryTTF(child_icons_hex, sizeof child_icons_hex, SCALE(12.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[4] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof icons_hex, SCALE(8.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[5] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof icons_hex, SCALE(11.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[6] = io.Fonts->AddFontFromMemoryTTF(keybind_icons_hex, sizeof keybind_icons_hex, SCALE(10.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[7] = io.Fonts->AddFontFromMemoryTTF(keybind_icons_hex, sizeof keybind_icons_hex, SCALE(5.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[8] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof icons_hex, SCALE(15.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[9] = io.Fonts->AddFontFromMemoryTTF(color_icons_hex, sizeof color_icons_hex, SCALE(11.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());

            io.Fonts->Build();
            ImGui_ImplDX11_CreateDeviceObjects();

            var->dpi_changed = false;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
        static ID3D11ShaderResourceView* background = nullptr;

        D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };

        if (background == nullptr)
            D3DX11CreateShaderResourceViewFromMemory(var->device_dx11, background_hex, sizeof(background_hex), &info, pump, &background, 0);

        GetBackgroundDrawList()->AddImage(background, ImVec2(0, 0), ImVec2(1920, 1080));

        gui->render();

        if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            SendMessage(hwnd, WM_CLOSE, 0, 0);

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        var->device_context->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        var->device_context->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = var->swap_chain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &var->swap_chain, &var->device_dx11, &featureLevel, &var->device_context);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &var->swap_chain, &var->device_dx11, &featureLevel, &var->device_context);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (var->swap_chain) { var->swap_chain->Release(); var->swap_chain = nullptr; }
    if (var->device_context) { var->device_context->Release(); var->device_context = nullptr; }
    if (var->device_dx11) { var->device_dx11->Release(); var->device_dx11 = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    var->swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    var->device_dx11->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
