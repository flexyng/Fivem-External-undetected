#include "esp.hpp"

#include <cmath>

#include "../../FivemSDK/Fivem.hpp"
#include "../../Options.hpp"
#include <optional>
#include <utility>
#include <vector>
#include "../../../FrameWork/Dependencies/ImGui/imgui.h"//"ImGui/imgui.h"
#include "Renderer.h"

struct Position
{
	ImVec2 Pos;
};


namespace Cheat
{
	void ESP::Thread()
	{
		if (!g_Fivem.GetLocalPlayerInfo().Ped)
			return;

		for (Entity Current : g_Fivem.GetEntitiyList())
		{
			if (Current.StaticInfo.bIsLocalPlayer && !g_options.esp.player.allowlocalplayer)
				continue;

			if (Current.StaticInfo.bIsNPC && !g_options.esp.player.allownpc)
				continue;

			Vector3D PedCoordinates = Current.Cordinates;

			float Distance = PedCoordinates.DistTo(g_Fivem.GetLocalPlayerInfo().WorldPos);

			if (Distance > g_options.esp.player.renderingdistance)
				continue;

			ImVec2 PedLocation = g_Fivem.WorldToScreen(PedCoordinates);

			ImVec2 Head = Current.HeadPos;
			if (!g_Fivem.IsOnScreen(Head))
				continue;

			ImVec2 LeftFoot = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_L_Foot));
			if (!g_Fivem.IsOnScreen(LeftFoot))
				continue;

			ImVec2 RightFoot = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, SKEL_R_Foot));
			if (!g_Fivem.IsOnScreen(RightFoot))
				continue;

			float Height = -Head.y;

			if (LeftFoot.y > RightFoot.y)
				Height += LeftFoot.y;
			else
				Height += RightFoot.y;

			float Width = Height / 1.8f;
			float PedCenterY = Head.y + Height / 2.f;
			Height *= 1.2f;

			ImVec2 Padding[4] = { ImVec2(0, 0), ImVec2(0, 0), ImVec2(0, 0), ImVec2(0, 0) };

			if (g_options.esp.player.box)
			{
				ImVec2 boxPos(PedLocation.x - Width / 2, PedCenterY - Height / 2.f);
				ImVec2 boxSize(Width, Height);

				render.outlined_rect(boxPos, boxSize, IM_COL32(255, 255, 255, 255), 0.f);
			}

			if (g_options.esp.player.corner)
			{
				ImVec2 boxPos(PedLocation.x - Width / 2, PedCenterY - Height / 2.f);
				ImVec2 boxSize(Width, Height);

				render.cornered_rect(boxPos, boxSize, IM_COL32(113, 118, 189, 255));
			}

			if (g_options.esp.player.skeleton)
			{
				ImColor Color = (Current.StaticInfo.IsFriend)
					? ImColor(g_options.esp.colors.friendcolor[0], g_options.esp.colors.friendcolor[1], g_options.esp.colors.friendcolor[2], g_options.esp.colors.friendcolor[3])
					: ImColor(g_options.esp.colors.skeletoncolor[0], g_options.esp.colors.skeletoncolor[1], g_options.esp.colors.skeletoncolor[2], g_options.esp.colors.skeletoncolor[3]);

				static const std::pair<int, int> bonePairs[] = {
					{SKEL_Pelvis, SKEL_Neck_1}, {SKEL_Neck_1, SKEL_L_Clavicle},
					{SKEL_Neck_1, SKEL_R_Clavicle}, {SKEL_L_Clavicle, SKEL_L_UpperArm},
					{SKEL_R_Clavicle, SKEL_R_UpperArm}, {SKEL_L_UpperArm, SKEL_L_Forearm},
					{SKEL_R_UpperArm, SKEL_R_Forearm}, {SKEL_L_Forearm, SKEL_L_Hand},
					{SKEL_R_Forearm, SKEL_R_Hand}, {SKEL_Pelvis, SKEL_L_Thigh},
					{SKEL_Pelvis, SKEL_R_Thigh}, {SKEL_L_Thigh, SKEL_L_Calf},
					{SKEL_R_Thigh, SKEL_R_Calf}, {SKEL_L_Calf, SKEL_L_Foot},
					{SKEL_R_Calf, SKEL_R_Foot}
				};

				auto GetBoneScreenPos = [&](int boneId) -> std::optional<ImVec2> {
					ImVec2 pos = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Current, boneId));
					return g_Fivem.IsOnScreen(pos) ? std::optional<ImVec2>{pos} : std::nullopt;
					};

				ImDrawList* drawList = ImGui::GetBackgroundDrawList();
				if (drawList) {
					for (const auto& [boneA, boneB] : bonePairs) {
						auto boneAPos = GetBoneScreenPos(boneA);
						auto boneBPos = GetBoneScreenPos(boneB);

						if (boneAPos && boneBPos) {
							drawList->AddLine(*boneAPos, *boneBPos, Color, 1.f);
						}
					}
				}
			}

			if (g_options.esp.player.chams) {
				static const std::pair<int, int> bonePairs[] = {
					{SKEL_Pelvis, SKEL_Neck_1}, {SKEL_Neck_1, SKEL_L_Clavicle},
					{SKEL_Neck_1, SKEL_R_Clavicle}, {SKEL_L_Clavicle, SKEL_L_UpperArm},
					{SKEL_R_Clavicle, SKEL_R_UpperArm}, {SKEL_L_UpperArm, SKEL_L_Forearm},
					{SKEL_R_UpperArm, SKEL_R_Forearm}, {SKEL_L_Forearm, SKEL_L_Hand},
					{SKEL_R_Forearm, SKEL_R_Hand}, {SKEL_Pelvis, SKEL_L_Thigh},
					{SKEL_Pelvis, SKEL_R_Thigh}, {SKEL_L_Thigh, SKEL_L_Calf},
					{SKEL_R_Thigh, SKEL_R_Calf}, {SKEL_L_Calf, SKEL_L_Foot},
					{SKEL_R_Calf, SKEL_R_Foot}
				};
			}

			if (g_options.esp.player.healthbar)
			{

				float Health = Current.StaticInfo.Ped->GetHealth();
				float MaxHealth = Current.StaticInfo.Ped->GetMaxHealth();

				ImColor healthColor = (Health > MaxHealth * 0.66f) ? ImColor(0, 255, 0) : (Health > MaxHealth * 0.33f) ? ImColor(255, 255, 0) : ImColor(255, 0, 0);

				ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY - Height / 2) - Padding[3] - ImVec2(6, 0);
				ImVec2 Size = ImVec2(0, Height);
				ImVec2 DrawSize = ImVec2(-2, (Height / MaxHealth) * Health);

				ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0, 0, 0));
				ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos, DrawPos + Size + ImVec2(3, 0), ImColor(0, 0, 0));

				ImGui::GetBackgroundDrawList()->AddRectFilled(DrawPos + Size, DrawPos + Size - DrawSize, healthColor);

				Padding[3].x += 4;
			}
			if (g_options.esp.player.armorbar)
			{
				ImColor Color = ImColor(0.f, 168 / 255.f, 1.f);

				float Armor = Current.StaticInfo.Ped->GetArmor();
				const float MaxArmor = 100.0f;

				if (Armor > 0) {
					ImVec2 DrawPos = ImVec2(PedLocation.x - Width / 2, PedCenterY + Height / 2 + Padding[2].y);
					ImVec2 Size = ImVec2(Width, 2);
					ImVec2 DrawSize = ImVec2((Width / MaxArmor) * Armor, 2);

					ImDrawList* drawList = ImGui::GetBackgroundDrawList();
					if (drawList) {
						drawList->AddRectFilled(DrawPos - ImVec2(1, 1), DrawPos + Size + ImVec2(1, 1), ImColor(0, 0, 0));
						drawList->AddRectFilled(DrawPos, DrawPos + Size, ImColor(0, 0, 0));
						drawList->AddRectFilled(DrawPos, DrawPos + DrawSize, Color);
					}

					Padding[2].y += 3;
				}
			}
			if (g_options.esp.player.name)
			{
				std::string Name = Current.StaticInfo.Name;
				if (Name.empty())
					Name = ("npc");

				ImVec2 TextSize = ImGui::CalcTextSize(Name.c_str());

				ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY + Height / 2);
				DrawPos = ImVec2(DrawPos.x - TextSize.x / 2, DrawPos.y + Padding[2].y);
				Padding[2].y += TextSize.y;

				ImGui::PushFont(FrameWork::Assets::InterRegular);

				ImColor Color = ImColor(g_options.esp.colors.namecolor[0], g_options.esp.colors.namecolor[1], g_options.esp.colors.namecolor[2], g_options.esp.colors.namecolor[3]);

				ImGui::GetBackgroundDrawList()->AddText(DrawPos + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_options.esp.colors.namecolor[3]), Name.c_str());
				ImGui::GetBackgroundDrawList()->AddText(DrawPos, Color, Name.c_str());

				ImGui::PopFont();
			}
			if (g_options.esp.player.weaponname)
			{
				CWeaponManager* WeaponManager = Current.StaticInfo.Ped->GetWeaponManager();
				if (WeaponManager)
				{
					CWeaponInfo* WeaponInfo = WeaponManager->GetWeaponInfo();
					if (WeaponInfo)
					{
						std::string WeaponName = WeaponInfo->GetWeaponName();
						if (WeaponName.size() > 0)
						{
							ImVec2 TextSize = ImGui::CalcTextSize(WeaponName.c_str());

							ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY + Height / 2);
							DrawPos = ImVec2(DrawPos.x - TextSize.x / 2, DrawPos.y + Padding[2].y - 3);
							Padding[2].y += TextSize.y;

							ImColor Color = ImColor(g_options.esp.colors.weaponnamecolor[0], g_options.esp.colors.weaponnamecolor[1], g_options.esp.colors.weaponnamecolor[2], g_options.esp.colors.weaponnamecolor[3]);

							ImGui::PushFont(FrameWork::Assets::InterRegular);

							ImGui::GetBackgroundDrawList()->AddText(DrawPos + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_options.esp.colors.weaponnamecolor[3]), WeaponName.c_str());
							ImGui::GetBackgroundDrawList()->AddText(DrawPos, Color, WeaponName.c_str());

							ImGui::PopFont();
						}
					}
				}
			}
			if (g_options.esp.player.distance)
			{
				if (!Current.StaticInfo.bIsLocalPlayer)
				{
					std::string PlayerDistance = std::to_string((int)Distance) + ("m");
					ImVec2 TextSize = ImGui::CalcTextSize(PlayerDistance.c_str());

					ImVec2 DrawPos = ImVec2(PedLocation.x, PedCenterY + Height / 2);

					DrawPos = ImVec2(DrawPos.x - TextSize.x / 2, DrawPos.y + Padding[2].y - 3);
					Padding[2].y += TextSize.y;

					ImColor Color = ImColor(g_options.esp.colors.distancecolor[0], g_options.esp.colors.distancecolor[1], g_options.esp.colors.distancecolor[2], g_options.esp.colors.distancecolor[3]);

					ImGui::PushFont(FrameWork::Assets::InterRegular);

					ImGui::GetBackgroundDrawList()->AddText(DrawPos + ImVec2(1, 1), ImColor(0.f, 0.f, 0.f, g_options.esp.colors.distancecolor[3]), PlayerDistance.c_str());
					ImGui::GetBackgroundDrawList()->AddText(DrawPos, Color, PlayerDistance.c_str());

					ImGui::PopFont();
				}
			}
			if (g_options.esp.player.lines)
			{
				if (!Current.StaticInfo.bIsLocalPlayer) {
					ImColor Color = Current.StaticInfo.IsFriend
						? ImColor(g_options.esp.colors.friendcolor[4], g_options.esp.colors.friendcolor[1], g_options.esp.colors.friendcolor[2], g_options.esp.colors.friendcolor[3])
						: ImColor(g_options.esp.colors.linescolor[0], g_options.esp.colors.linescolor[1], g_options.esp.colors.linescolor[2], g_options.esp.colors.linescolor[3]);

					ImDrawList* drawList = ImGui::GetBackgroundDrawList();
					if (drawList) {
						drawList->AddLine(g_Fivem.GetLocalPlayerInfo().ScreenPos, PedLocation, Color);
					}
				}
			}
		}
	}
}