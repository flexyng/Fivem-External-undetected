#include "Interface.hpp"

#include <Cheat/Options.hpp>
#include <Cheat/Cheat.hpp>
#include <Cheat/ConfigSystem.hpp>
#include <Cheat/FivemSDK/Fivem.hpp>
#include "../Dependencies/ImGui/font.h"
#include "../Dependencies/ImGui/image.h"
#include "../Dependencies/ImGui/imgui_freetype.h"
#include "../Dependencies/ImGui/imgui_settings.h"
#include <FrameWork/framework/settings/functions.h>
#include <FrameWork/framework/data/fonts.h>

inline Cheat::ConfigManager Condif;

struct MarkerT
{
	Vector3D Position;
	std::string Name;
};

std::vector<MarkerT> MarkerList;

//DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;





DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); 
namespace FrameWork
{
	void Interface::Initialize(HWND Window, HWND TargetWindow, ID3D11Device* Device, ID3D11DeviceContext* DeviceContext)
	{
		hWindow = Window;
		IDevice = Device;

		ImGui::CreateContext(); 
		ImGui_ImplWin32_Init(hWindow); 
		ImGui_ImplDX11_Init(Device, DeviceContext); 
		ImGuiIO& io = ImGui::GetIO();  (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImFontConfig cfg;
		cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

		D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
		//if (image::background == nullptr) D3DX11CreateShaderResourceViewFromMemory(Device, background, sizeof(background), &info, pump, &image::background, 0);


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
			ImGui_ImplDX11_CreateDeviceObjects();

			var->dpi_changed = false;
		}

		MarkerList.push_back({ Vector3D(212.6681f, -813.7118f, 30.7386f), ("Meeting Point") }); 
		MarkerList.push_back({ Vector3D(99.7721f, -743.7130f, 45.7547f), ("FIB-Tower") }); 
		MarkerList.push_back({ Vector3D(-1039.2391f, -2666.4702f, 13.8307f), ("Airport") }); 
		MarkerList.push_back({ Vector3D(3627.5176f, 3754.3137f, 28.5157f), ("Humanlabs") }); 
		MarkerList.push_back({ Vector3D(1404.8857f, 3162.1936f, 40.4341f), ("Sandyshores Airfield") }); 
		MarkerList.push_back({ Vector3D(-2326.4241f, 3053.1711f, 32.8150f), ("Armybase") }); 
		MarkerList.push_back({ Vector3D(501.6581f, 5604.9321f, 797.9105f), ("Mount Chiliad") }); 
		MarkerList.push_back({ Vector3D(-1448.0753f, -766.6392f, 23.5332f), ("Del Perro Pier") }); 
		MarkerList.push_back({ Vector3D(298.7408f, -581.9695f, 43.2608f), ("Pillbox") }); 
	}

	void Particles()
	{
		ImVec2 screen_size = { (float)GetSystemMetrics(SM_CXSCREEN), (float)GetSystemMetrics(SM_CYSCREEN) };

		static ImVec2 partile_pos[100];
		static ImVec2 partile_target_pos[100];
		static float partile_speed[100];
		static float partile_radius[100];


		for (int i = 1; i < 50; i++)
		{
			if (partile_pos[i].x == 0 || partile_pos[i].y == 0)
			{
				partile_pos[i].x = rand() % (int)screen_size.x + 1;
				partile_pos[i].y = 15.f;
				partile_speed[i] = 1 + rand() % 25;
				partile_radius[i] = rand() % 4;

				partile_target_pos[i].x = rand() % (int)screen_size.x;
				partile_target_pos[i].y = screen_size.y * 2;
			}

			partile_pos[i] = ImLerp(partile_pos[i], partile_target_pos[i], ImGui::GetIO().DeltaTime * (partile_speed[i] / 60)); 

			if (partile_pos[i].y > screen_size.y)
			{
				partile_pos[i].x = 0;
				partile_pos[i].y = 0;
			}

			ImGui::GetWindowDrawList()->AddCircleFilled(partile_pos[i], partile_radius[i], ImColor(71, 226, 67, 255 / 2)); 
		}

	}

	std::string GetDesktopUsername()
	{
		char username[256];
		DWORD username_len = sizeof(username); 
		if (GetUserNameA(username, &username_len))
		{
			return std::string(username); 
		}
		return "Unknown User";
	}

	void Interface::UpdateStyle()
	{
		Assets::Initialize(IDevice); 
		Condif.Setup(); 
	}

	void Interface::RenderGui()
	{
		static bool menu_open = false;

		if (GetAsyncKeyState(VK_F1) & 1)
			menu_open = !menu_open;

		if (!menu_open)
			return;

		gui->render();
	}
	void Interface::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			if (wParam != SIZE_MINIMIZED)
			{
				ResizeWidht = (UINT)LOWORD(lParam); 
				ResizeHeight = (UINT)HIWORD(lParam); 
			}
			break;
		}

		if (bIsMenuOpen)
		{
			ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam); 
		}
	}

	void Interface::HandleMenuKey()
	{
		static bool MenuKeyDown = false;
		if (GetAsyncKeyState(g_options.menu.menubind) & 0x8000)
		{
			if (!MenuKeyDown)
			{
				MenuKeyDown = true;
				bIsMenuOpen = !bIsMenuOpen;

				if (bIsMenuOpen)
				{
					SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE); 
					SetForegroundWindow(hWindow); 
				}
				else
				{
					SetWindowLong(hWindow, GWL_EXSTYLE, WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_NOACTIVATE); 
					SetForegroundWindow(hTargetWindow); 
				}
			}
		}
		else
		{
			MenuKeyDown = false;
		}
	}

	void Interface::ShutDown()
	{
		SafeCall(ExitProcess)(0); 
	}
}