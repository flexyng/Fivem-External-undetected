// Dear ImGui: standalone example application for DirectX 9

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "../../framework/settings/functions.h"
#include "../../framework/data/fonts.h"
#include "../../framework/data/images.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "imgui_freetype.h"
#include <d3d9.h>
#include <D3DX11.h>
#include <tchar.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

// Data
// Data
static LPDIRECT3D9              g_pD3D = nullptr;
static bool                     g_DeviceLost = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
#include <windows.h>
#include <d3d9.h>
#include <imgui.h>
#include <backends/imgui_impl_win32.h>
#include <backends/imgui_impl_dx9.h>

int main(int, char**)
{
    // Create application window
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Shibaavlr", nullptr };
    ::RegisterClassExW(&wc);

    // Create a layered window with transparent background
    HWND hwnd = ::CreateWindowExW(WS_EX_LAYERED, wc.lpszClassName, L"Shibaavlr", WS_POPUP, 0, 0, 1920, 1080, nullptr, nullptr, wc.hInstance, nullptr);

    // Set the layered window attributes (transparent window)
    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

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
    ImGui::StyleColorsDark(); // Use a dark style for ImGui

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(var->device_dx9);

    // Load Fonts (already handled in your code, no need to modify)

    // Our state
    /*ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 0.1f);*/
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
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

        // Handle lost D3D9 device
        if (g_DeviceLost)
        {
            HRESULT hr = var->device_dx9->TestCooperativeLevel();
            if (hr == D3DERR_DEVICELOST)
            {
                ::Sleep(10);
                continue;
            }
            if (hr == D3DERR_DEVICENOTRESET)
                ResetDevice();
            g_DeviceLost = false;
        }

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            g_d3dpp.BackBufferWidth = g_ResizeWidth;
            g_d3dpp.BackBufferHeight = g_ResizeHeight;
            g_ResizeWidth = g_ResizeHeight = 0;
            ResetDevice();
        }

        if (var->dpi_changed)
        {
            var->dpi = var->stored_dpi / 100.f;

            ImFontConfig cfg;
            cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_Bitmap;
            cfg.FontDataOwnedByAtlas = false;

            io.Fonts->Clear();

            var->font.inter[0] = io.Fonts->AddFontFromMemoryTTF(inter_semibold_hex, sizeof(inter_semibold_hex), SCALE(12.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.inter[1] = io.Fonts->AddFontFromMemoryTTF(inter_medium_hex, sizeof(inter_medium_hex), SCALE(12.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.inter[2] = io.Fonts->AddFontFromMemoryTTF(inter_medium_hex, sizeof(inter_medium_hex), SCALE(10.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.inter[3] = io.Fonts->AddFontFromMemoryTTF(inter_semibold_hex, sizeof(inter_semibold_hex), SCALE(13.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());

            var->font.icons[0] = io.Fonts->AddFontFromMemoryTTF(logotype_hex, sizeof(logotype_hex), SCALE(30.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[1] = io.Fonts->AddFontFromMemoryTTF(section_icons_hex, sizeof(section_icons_hex), SCALE(18.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[2] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof(icons_hex), SCALE(10.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[3] = io.Fonts->AddFontFromMemoryTTF(child_icons_hex, sizeof(child_icons_hex), SCALE(12.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[4] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof(icons_hex), SCALE(8.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[5] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof(icons_hex), SCALE(11.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[6] = io.Fonts->AddFontFromMemoryTTF(keybind_icons_hex, sizeof(keybind_icons_hex), SCALE(10.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[7] = io.Fonts->AddFontFromMemoryTTF(keybind_icons_hex, sizeof(keybind_icons_hex), SCALE(5.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[8] = io.Fonts->AddFontFromMemoryTTF(icons_hex, sizeof(icons_hex), SCALE(15.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());
            var->font.icons[9] = io.Fonts->AddFontFromMemoryTTF(color_icons_hex, sizeof(color_icons_hex), SCALE(11.f), &cfg, io.Fonts->GetGlyphRangesCyrillic());

            io.Fonts->Build();
            ImGui_ImplDX9_CreateDeviceObjects();

            var->dpi_changed = false;
        }

        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        /*static IDirect3DTexture9* background = nullptr;

        if (!background)
            D3DXCreateTextureFromFileInMemoryEx(var->device_dx9, &background_hex, sizeof(background_hex), 1920, 1080, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &background);

        GetBackgroundDrawList()->AddImage(background, ImVec2(0, 0), ImVec2(1920, 1080));*/

        gui->render();

        if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
            SendMessage(hwnd, WM_CLOSE, 0, 0);

        // Rendering
        ImGui::EndFrame();
        var->device_dx9->SetRenderState(D3DRS_ZENABLE, FALSE);
        var->device_dx9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        var->device_dx9->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(
            (int)(clear_color.x * clear_color.w * 255.0f),
            (int)(clear_color.y * clear_color.w * 255.0f),
            (int)(clear_color.z * clear_color.w * 255.0f),
            0); // Alpha auf 0 für Transparenz
        var->device_dx9->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (var->device_dx9->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            var->device_dx9->EndScene();
        }
        HRESULT result = var->device_dx9->Present(nullptr, nullptr, nullptr, nullptr);
        if (result == D3DERR_DEVICELOST)
            g_DeviceLost = true;
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
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
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
        return false;

    // Create the D3DDevice
    ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
    g_d3dpp.Windowed = TRUE;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Need to use an explicit format with alpha if needing per-pixel alpha composition.
    g_d3dpp.EnableAutoDepthStencil = TRUE;
    g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
    //g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
    if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &var->device_dx9) < 0)
        return false;

    return true;
}



void CleanupDeviceD3D()
{
    if (var->device_dx9) { var->device_dx9->Release(); var->device_dx9 = nullptr; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = nullptr; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = var->device_dx9->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
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
