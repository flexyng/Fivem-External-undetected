#include "Cheat.hpp"
#include "FivemSDK/Fivem.hpp"
#include <thread>
#include <FrameWork/FrameWork.hpp>
#include <chrono>

namespace Cheat
{
	void Initialize()
	{
		while (!g_Fivem.IsInitialized())
		{
			g_Fivem.Intialize();

			if (!g_Fivem.IsInitialized())
				std::this_thread::sleep_for(std::chrono::seconds(5));
		}

		FrameWork::Overlay::Setup(g_Fivem.GetPid());
		FrameWork::Overlay::Initialize();

		std::thread([&]() { TriggerBot::RunThread(); }).detach();
		std::thread([&]() { AimBot::RunThread(); }).detach();
		std::thread([&]() { SilentAim::RunThread(); }).detach();
		std::thread([&]() { Exploits::RunThread(); }).detach();
		std::thread([&]() { MagicBullet::RunThread(); }).detach();

		if (FrameWork::Overlay::IsInitialized())
		{
			FrameWork::Interface Interface(FrameWork::Overlay::GetOverlayWindow(), FrameWork::Overlay::GetTargetWindow(), FrameWork::Overlay::dxGetDevice(), FrameWork::Overlay::dxGetDeviceContext());
			Interface.UpdateStyle();
			FrameWork::Overlay::SetupWindowProcHook(std::bind(&FrameWork::Interface::WindowProc, &Interface, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));

			MSG Message;
			ZeroMemory(&Message, sizeof(Message));
			while (Message.message != WM_QUIT)
			{
				if (PeekMessage(&Message, FrameWork::Overlay::GetOverlayWindow(), NULL, NULL, PM_REMOVE))
				{
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}

				g_Fivem.UpdateEntities();
				g_Fivem.UpdateVehicles();

				if (Interface.ResizeHeight != 0 || Interface.ResizeWidht != 0)
				{
					FrameWork::Overlay::dxCleanupRenderTarget();
					FrameWork::Overlay::dxGetSwapChain()->ResizeBuffers(0, Interface.ResizeWidht, Interface.ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
					Interface.ResizeHeight = Interface.ResizeWidht = 0;
					FrameWork::Overlay::dxCreateRenderTarget();
				}

				Interface.HandleMenuKey();
				FrameWork::Overlay::UpdateWindowPos();

				static bool streamproofon = false;
				if (g_options.menu.streamproof != streamproofon)
				{
					streamproofon = g_options.menu.streamproof;
					SafeCall(SetWindowDisplayAffinity)(FrameWork::Overlay::GetOverlayWindow(), streamproofon ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
				}

				ImGui_ImplDX11_NewFrame();
				ImGui_ImplWin32_NewFrame();
				ImGui::NewFrame();
				{
					ImColor aimbotcolor =
						ImColor(g_options.aim.aimbot.fovcolor[0], g_options.aim.aimbot.fovcolor[1], g_options.aim.aimbot.fovcolor[2], g_options.aim.aimbot.fovcolor[3]);

					ImColor silentcolor =
						ImColor(g_options.aim.silent.fovcolor[0], g_options.aim.silent.fovcolor[1], g_options.aim.silent.fovcolor[2], g_options.aim.silent.fovcolor[3]);

					ImColor triggercolor =
						ImColor(g_options.aim.trigger.fovcolor[0], g_options.aim.trigger.fovcolor[1], g_options.aim.trigger.fovcolor[2], g_options.aim.trigger.fovcolor[3]);

					if (g_options.aim.aimbot.drawfov)
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), g_options.aim.aimbot.fov, aimbotcolor, 360);

					if (g_options.aim.silent.drawfov)
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), g_options.aim.silent.fov, silentcolor, 360);

					if (g_options.aim.trigger.drawfov)
						ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), g_options.aim.trigger.fov, triggercolor, 360);

					if (g_options.esp.vehicle.enabled)
						ESP::Vehicles();

					if (g_options.esp.player.enabled)
						ESP::Thread();

					if (g_options.misc.localplayer.bIsSpectatingEntity)
					{
						ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(ImGui::GetIO().DisplaySize.x, 30), ImColor(60, 70, 180, 100));

						ImVec2 TextSize = ImGui::CalcTextSize(("Spectating, press ESC to leave"));

						ImGui::GetBackgroundDrawList()->AddText(ImVec2(ImGui::GetIO().DisplaySize.x / 2 - TextSize.x / 2, 15 - TextSize.y / 2), ImColor(255, 255, 255), ("Spectating, press ESC to leave"));

						if (GetAsyncKeyState(VK_ESCAPE))
						{
							//g_Fivem.SpectatePed(NULL, false);

							g_options.misc.localplayer.bIsSpectatingEntity = false;
						}
					}

					Interface.RenderGui();
				}
				ImGui::EndFrame();
				ImGui::Render();

				FrameWork::Overlay::dxRefresh();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
				FrameWork::Overlay::dxGetSwapChain()->Present(0, 0);

				if (g_options.menu.shutdown)
					return;
			}
		}
	}
}