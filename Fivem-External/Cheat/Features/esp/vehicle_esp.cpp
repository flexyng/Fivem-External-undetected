#pragma once
#include "vehicle_esp.hpp"

#include "../../FivemSDK/Fivem.hpp"
#include "../../Options.hpp"
#include "renderer.h"

namespace Cheat
{
	void ESP::Vehicles()
	{
		if (!g_Fivem.GetLocalPlayerInfo().Ped)
			return;

		for (VehicleInfo Current : g_Fivem.GetVehicleList())
		{
			ImVec2 Position = g_Fivem.WorldToScreen(Current.Vehicle->GetCoordinate());
			if (!g_Fivem.IsOnScreen(Position))
				continue;

			float OffsetY = 0;

			if (g_options.esp.vehicle.marker)
			{
				ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position.x, Position.y + OffsetY), 4, ImColor(0.f, 0.f, 0.f, g_options.esp.colors.vhmarkercolor[3]));
				ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(Position.x, Position.y + OffsetY), 3, ImColor(g_options.esp.colors.vhmarkercolor[0], g_options.esp.colors.vhmarkercolor[1], g_options.esp.colors.vhmarkercolor[2], g_options.esp.colors.vhmarkercolor[3]));

				OffsetY += 12;
			}

			ImGui::PushFont(FrameWork::Assets::InterBold12);

			if (g_options.esp.vehicle.name)
			{
				ImVec2 TextSize = ImGui::CalcTextSize(Current.Name.c_str());

				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Position.x + 1 - TextSize.x / 2, Position.y + OffsetY + 1), ImColor(0.f, 0.f, 0.f, g_options.esp.colors.vhnamecolor[3]), Current.Name.c_str());
				ImGui::GetBackgroundDrawList()->AddText(ImVec2(Position.x - TextSize.x / 2, Position.y + OffsetY), ImColor(1.f, 1.f, 1.f, g_options.esp.colors.vhnamecolor[3]), Current.Name.c_str());

				OffsetY += 12;
			}

			if (g_options.esp.vehicle.lines) // Certifique-se de ter uma opção correspondente no menu
			{
				Vector3D VehicleCoordinates = Current.Vehicle->GetCoordinate(); // Obtém as coordenadas do veículo no mundo
				float Distance = VehicleCoordinates.DistTo(g_Fivem.GetLocalPlayerInfo().WorldPos); // Calcula a distância do veículo ao jogador local

				if (Distance > g_options.esp.vehicle.renderingdistance) // Verifica a distância de renderização configurada
					return;

				ImVec2 VehicleScreenPosition = g_Fivem.WorldToScreen(VehicleCoordinates); // Converte coordenadas do mundo para a tela

				if (!g_Fivem.IsOnScreen(VehicleScreenPosition)) // Verifica se o veículo está visível na tela
					return;

				ImColor LineColor = ImColor(1.f, 1.f, 1.f, 1.f);

				ImDrawList* drawList = ImGui::GetBackgroundDrawList();
				if (drawList) {
					drawList->AddLine(
						g_Fivem.GetLocalPlayerInfo().ScreenPos,
						VehicleScreenPosition,
						LineColor
					);
				}
			}


			ImGui::PopFont();
		}
	}
}