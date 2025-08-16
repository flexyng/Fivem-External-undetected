#include "rage.hpp"
#include "../../Options.hpp"
#include <Windows.h>
#include <chrono>
#include "../../FivemSDK/Fivem.hpp"
#include "../../Options.hpp"
#include <cmath>
#include <optional>
#include <utility>
#include <vector>

bool TPVehicle = false;
uint64_t TPModelInfo = NULL;
uint64_t TPObject = NULL;
uint64_t TPNavigation = NULL;
Vector3D TPPosition = Vector3D(0, 0, 0);

namespace Cheat
{
	void Noclip(Vector3D CameraPos)
	{
		if (!TPObject)
			return;

		if (!TPModelInfo)
			return;

		static float MagicValue = 0.0f;
		static bool Restoring = false;

		if (GetAsyncKeyState(g_options.misc.localplayer.noclipbind) & 1)
		{
			g_options.misc.localplayer.noclip = !g_options.misc.localplayer.noclip;
		}

		if (!g_options.misc.localplayer.noclip)
		{
			if (Restoring)
			{
				FrameWork::Memory::WriteMemory(TPModelInfo + 0x2C, MagicValue);
			}

			Restoring = false;
			return;
		}

		if (!Restoring)
		{
			MagicValue = FrameWork::Memory::ReadMemory<float>(TPModelInfo + 0x2C);
			FrameWork::Memory::WriteMemory(TPModelInfo + 0x2C, 0.f);

			Restoring = true;
		}


		auto Speed = static_cast<float>(g_options.misc.localplayer.noclip) / 100.f * 10.0f; 

		if (GetAsyncKeyState(VK_SHIFT))
		{
			Speed *= 4.f;  
		}

		Vector3D NewPosition = TPPosition;

		if (GetAsyncKeyState('W'))
		{
			NewPosition.x += (CameraPos * Speed).x;
			NewPosition.y += (CameraPos * Speed).y;
			NewPosition.z += (CameraPos * Speed).z;
		}

		if (GetAsyncKeyState('S'))
		{
			NewPosition.x -= (CameraPos * Speed).x;
			NewPosition.y -= (CameraPos * Speed).y;
			NewPosition.z -= (CameraPos * Speed).z;
		}

		Vector3D Right = CameraPos.Cross(Vector3D(0, 0, 1));

		if (GetAsyncKeyState('A'))
		{
			NewPosition.x -= (Right * Speed).x;
			NewPosition.y -= (Right * Speed).y;
			NewPosition.z -= (Right * Speed).z;
		}

		if (GetAsyncKeyState('D'))
		{
			NewPosition.x += (Right * Speed).x;
			NewPosition.y += (Right * Speed).y;
			NewPosition.z += (Right * Speed).z;
		}

		if (GetAsyncKeyState(VK_SPACE))
		{
			NewPosition.z += (Speed);
		}

		if (GetAsyncKeyState(VK_LCONTROL))
		{
			NewPosition.z -= (Speed);
		}

		g_Fivem.TeleportObject(TPObject, TPNavigation, 0, NewPosition, NewPosition, false);
	}



	void Exploits::TeleportToWaypoint()
	{
		for (int i = 0; i < 2000; i++)
		{
			uint64_t Blip = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.GetBlipListAddress() + (i * 8));
			if (!Blip)
				continue;

			int BlipIcon = FrameWork::Memory::ReadMemory<int>(Blip + 0x40);
			int BlipColor = FrameWork::Memory::ReadMemory<int>(Blip + 0x48);

			if ((BlipColor != BlipColor::ColorWaypoint) || (BlipIcon != BlipSprite::SpriteWaypoint))
				continue;

			Vector2D WaypointPos = FrameWork::Memory::ReadMemory<Vector2D>(Blip + 0x10);

			if (WaypointPos.x != 0 && WaypointPos.y != 0)
			{
				uint64_t Object = NULL;
				uint64_t Navigation = NULL;
				uint64_t ModelInfo = NULL;

				if (g_Fivem.GetLocalPlayerInfo().Ped->HasConfigFlag(CPED_CONFIG_FLAG_InVehicle))
				{
					CVehicle* Vehicle = g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle();
					if (Vehicle)
					{
						Object = (uint64_t)Vehicle;
						Navigation = Vehicle->GetNavigation();
						ModelInfo = Vehicle->GetModelInfo();
					}
				}

				if (!Object || !Navigation || !ModelInfo)
				{
					Object = (uint64_t)g_Fivem.GetLocalPlayerInfo().Ped;
					Navigation = g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation();
					ModelInfo = g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo();
				}

				Vector3D TeleportPos = Vector3D(WaypointPos.x, WaypointPos.y, -210.f);

				g_Fivem.TeleportObject(Object, Navigation, ModelInfo, TeleportPos, TeleportPos, true);
			}
		}
	}

	struct CarInfo {
		uintptr_t carInfo;
		float defaultSpeed;
		float defaultTraction;
	};

	std::vector<CarInfo> carList;

	const CarInfo* findCarByInfo(uintptr_t carInfo) {
		for (const CarInfo& car : carList) {
			if (car.carInfo == carInfo) {
				return &car;
			}
		}
		return nullptr;
	}

	void Exploits::Revive()
	{
		static float MaxHealth = g_Fivem.GetLocalPlayerInfo().Ped->GetMaxHealth();
		g_Fivem.GetLocalPlayerInfo().Ped->SetHealth(MaxHealth);
	}

	void Exploits::FullArmor()
	{
		g_Fivem.GetLocalPlayerInfo().Ped->SetArmor(200.0f);
	}

	void Exploits::Suicide()
	{
		g_Fivem.GetLocalPlayerInfo().Ped->SetHealth(0.f);
	}

	void Exploits::UnlockClosestVehicle()
	{
		Cheat::VehicleInfo nearestVehicle;
		float minDistance = FLT_MAX;

		if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped != nullptr)
		{
			for (Cheat::VehicleInfo Current : Cheat::g_Fivem.GetVehicleList())
			{
				if (Current.Vehicle != nullptr)
				{
					float distance = Current.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos);

					if (distance < minDistance && distance < 600.0f)
					{
						minDistance = distance;
						nearestVehicle = Current;
					}
				}
			}

			if (nearestVehicle.Vehicle != nullptr)
			{
				nearestVehicle.Vehicle->SetDoorLock(1);
			}
		}
	}

	void Exploits::RunThread()
	{
		while (!g_options.menu.shutdown)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));

			uintptr_t weaponinfo = FrameWork::Memory::ReadMemory<uintptr_t>(WeaponManager + 0x20);
			uintptr_t playerInfo = FrameWork::Memory::ReadMemory<uintptr_t>(g_Fivem.GetLocalPlayerInfo().Ped + PlayerInfo);
			uint64_t cur_vehicle = FrameWork::Memory::ReadMemory<uint64_t>(playerInfo + LastVehicle);
			auto* vehicle = Cheat::g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle();
			if (!g_Fivem.GetLocalPlayerInfo().Ped)
				continue;

			if (g_options.misc.localplayer.speedwalk)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->GetPlayerInfo()->SetSpeed(g_options.misc.localplayer.speedwalkvalue);
			}
			else
			{
				g_Fivem.GetLocalPlayerInfo().Ped->GetPlayerInfo()->SetSpeed(1.f);
			}



			if (g_options.misc.localplayer.superjump)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->superjump(g_options.misc.localplayer.superjump);
			}

			if (g_options.misc.localplayer.beastjump)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->beastjump(g_options.misc.localplayer.beastjump);
			}

			if (g_options.misc.localplayer.explomeele)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->explomeele(g_options.misc.localplayer.explomeele);
			}

			if (g_options.misc.weapon.fireammo)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->fireammo(g_options.misc.weapon.fireammo);
			}


			if (g_options.misc.weapon.doubleshoot)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->doubleshoot(g_options.misc.weapon.doubleshoot);
			}

			if (g_options.misc.localplayer.onepunch)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->onepunch_on(g_options.misc.localplayer.onepunch);
			}
			if (!g_options.misc.localplayer.onepunch)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->onepunch_off(g_options.misc.localplayer.onepunch);
			}



			if (g_options.misc.vehicle.godmode)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->vehgodmode_on(g_options.misc.vehicle.godmode);
			}
			if (!g_options.misc.vehicle.godmode)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->vehgodmode_off(g_options.misc.vehicle.godmode);
			}

			if (g_options.misc.localplayer.Invisible)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->invisible_on(g_options.misc.localplayer.Invisible);
			}
			else
			{
				g_Fivem.GetLocalPlayerInfo().Ped->invisible_off(g_options.misc.localplayer.Invisible);
			}

			if (g_options.misc.localplayer.semigod)
			{
				static float MaxHealth = g_Fivem.GetLocalPlayerInfo().Ped->GetMaxHealth();
				g_Fivem.GetLocalPlayerInfo().Ped->SetHealth(MaxHealth);
			}


			if (g_options.misc.localplayer.ReganerateArmor)
			{
				if (g_Fivem.GetLocalPlayerInfo().Ped->GetArmor() < 200.0f)
				{
					g_Fivem.GetLocalPlayerInfo().Ped->SetArmor(200.0f);
				}
			}


		
			g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_UseAmbientModelScaling, g_options.misc.localplayer.shrink);
			g_Fivem.GetLocalPlayerInfo().Ped->SetInvencible(g_options.misc.localplayer.godmode);

			if (g_options.misc.localplayer.sethealth)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetHealth(g_options.misc.localplayer.health);

				g_options.misc.localplayer.sethealth = false;
			}

			if (g_options.misc.localplayer.infinitestamina)
			{
				FrameWork::Memory::WriteMemory<float>(playerInfo + 0x0CF4, -1);
			}
			else
			{
				FrameWork::Memory::WriteMemory<float>(playerInfo + 0x0CF4, 100);
			}

			if (GetAsyncKeyState(g_options.misc.vehicle.unlockbind) & 0x8000)
			{
				Cheat::VehicleInfo nearestVehicle;
				float minDistance = FLT_MAX;

				if (Cheat::g_Fivem.GetLocalPlayerInfo().Ped != nullptr)
				{
					for (Cheat::VehicleInfo Current : Cheat::g_Fivem.GetVehicleList())
					{
						if (Current.Vehicle != nullptr)
						{
							float distance = Current.Vehicle->GetCoordinate().DistTo(Cheat::g_Fivem.GetLocalPlayerInfo().WorldPos);

							if (distance < minDistance && distance < 600.0f)
							{
								minDistance = distance;
								nearestVehicle = Current;
							}
						}
					}

					if (nearestVehicle.Vehicle != nullptr)
					{
						nearestVehicle.Vehicle->SetDoorLock(1);
					}
				}
			}

			if (g_options.misc.localplayer.usegodmodebind)
			{
				if (GetAsyncKeyState(g_options.misc.localplayer.godmodebind) & 0x8000) {
					g_options.misc.localplayer.godmode = !g_options.misc.localplayer.godmode;
					Sleep(500);
				}
			}


			if (g_options.misc.localplayer.stealcar)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_NotAllowedToJackAnyPlayers, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_PlayerCanJackFriendlyPlayers, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_WillJackAnyPlayer, true);
			}
			else
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_NotAllowedToJackAnyPlayers, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_PlayerCanJackFriendlyPlayers, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_WillJackAnyPlayer, false);
			}

			static bool SeatBeltApplied = false;
			if (SeatBeltApplied != g_options.misc.localplayer.seatbelt)
			{
				//FrameWork::Memory::WriteMemory(g_Fivem.GetLocalPlayerInfo().Ped + Cheat::SeatBelt, g_options.misc.localplayer.seatbelt ? (uint8_t)0xC9 : (uint8_t)0xC8);
				

				SeatBeltApplied = g_options.misc.localplayer.seatbelt;
			}

			if (g_options.misc.localplayer.beastmode)
			{
				uint32_t previous_frame_flags = FrameWork::Memory::ReadMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetPlayerInfo() + 0x280);
				FrameWork::Memory::WriteMemory<uint32_t>(g_Fivem.GetLocalPlayerInfo().Ped->GetPlayerInfo() + 0x270, previous_frame_flags |
					(g_options.misc.localplayer.beastmode ? 1 << 14 : 0));
			}

			uint64_t ModelInfo = g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo();
			if (ModelInfo)
			{
				static uint64_t CurrentModelinfo = ModelInfo;

				static float OriginalCamDistance = FrameWork::Memory::ReadMemory<float>(ModelInfo + 0x38);

				float FinalValue = -5.f + (g_options.misc.localplayer.camdistance / 100.f) * (1.f - -5.f);
				FrameWork::Memory::WriteMemory(ModelInfo + 0x38, g_options.misc.localplayer.overridecamdistance ? FinalValue : OriginalCamDistance);
			}

			if (g_options.misc.localplayer.nocollision)
			{
				uint64_t navig = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.GetLocalPlayerInfo().Ped + 0x30);

				uint64_t addr1 = navig;
				if (addr1) {
					uint64_t addr2 = FrameWork::Memory::ReadMemory<uint64_t>(addr1 + 0x10);
					if (addr2) {
						uint64_t addr3 = FrameWork::Memory::ReadMemory<uint64_t>(addr2 + 0x20);
						if (addr3) {
							uint64_t addr4 = FrameWork::Memory::ReadMemory<uint64_t>(addr3 + 0x70);
							if (addr4) {
								uint64_t addr5 = FrameWork::Memory::ReadMemory<uint64_t>(addr4 + 0x0);
								if (addr5) {
									FrameWork::Memory::WriteMemory<float>(addr5 + 0x2C, -1.0f);
								}
							}
						}
					}
				}
			}
			else
			{
				uint64_t navig = FrameWork::Memory::ReadMemory<uint64_t>(g_Fivem.GetLocalPlayerInfo().Ped + 0x30);

				uint64_t addr1 = navig;
				if (addr1) {
					uint64_t addr2 = FrameWork::Memory::ReadMemory<uint64_t>(addr1 + 0x10);
					if (addr2) {
						uint64_t addr3 = FrameWork::Memory::ReadMemory<uint64_t>(addr2 + 0x20);
						if (addr3) {
							uint64_t addr4 = FrameWork::Memory::ReadMemory<uint64_t>(addr3 + 0x70);
							if (addr4) {
								uint64_t addr5 = FrameWork::Memory::ReadMemory<uint64_t>(addr4 + 0x0);
								if (addr5) {
									FrameWork::Memory::WriteMemory<float>(addr5 + 0x2C, 0.25f);
								}
							}
						}
					}
				}
			}

			if (g_options.misc.localplayer.noragdoll)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromAnyPedImpact, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromVehicleImpact, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromBulletImpact, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromExplosions, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromFire, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromElectrocution, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_CanActivateRagdollWhenVehicleUpsideDown, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromImpactObject, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromMelee, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromWaterJet, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromDrowning, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromFalling, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromRubberBullet, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollOnPedCollisionWhenDead, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollOnVehicleCollisionWhenDead, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromDrowning, true);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromDrowning, true);
			}
			else
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromAnyPedImpact, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromVehicleImpact, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromBulletImpact, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromExplosions, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromFire, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromElectrocution, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_CanActivateRagdollWhenVehicleUpsideDown, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromImpactObject, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromMelee, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromWaterJet, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromDrowning, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromFalling, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromRubberBullet, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollOnPedCollisionWhenDead, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollOnVehicleCollisionWhenDead, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromDrowning, false);
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_DontActivateRagdollFromDrowning, false);
			}

			auto Vehicle = (uint64_t)g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle();
			if (!findCarByInfo(Vehicle))
			{
				CarInfo currentCar;
				currentCar.carInfo = Vehicle;
				currentCar.defaultTraction = FrameWork::Memory::ReadMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->GetHandling() + 0x90);
				currentCar.defaultSpeed = FrameWork::Memory::ReadMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->GetHandling() + 0x4C);

				carList.push_back(currentCar);
			}

			auto handling = FrameWork::Memory::ReadMemory<uintptr_t>(Vehicle + Cheat::Handling);
			auto VehDriver = ((CVehicle*)Vehicle)->GetDriver();

			if (g_options.misc.vehicle.enabled)
			{
				FrameWork::Memory::WriteMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->GetHandling() + 0x90, g_options.misc.vehicle.tractionvalue); //Traction
				FrameWork::Memory::WriteMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->GetHandling() + 0x4C, g_options.misc.vehicle.accelerationvalue); //Boost
			}
			else
			{
				FrameWork::Memory::WriteMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->GetHandling() + 0x90, findCarByInfo(Vehicle)->defaultTraction); //Traction
				FrameWork::Memory::WriteMemory<float>(g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle()->GetHandling() + 0x4C, findCarByInfo(Vehicle)->defaultSpeed); //Boost
			}

			if (g_options.misc.localplayer.antiheadshot)
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_NoCriticalHits, true);
			}
			else
			{
				g_Fivem.GetLocalPlayerInfo().Ped->SetConfigFlag(CPED_CONFIG_FLAG_NoCriticalHits, false);
			}

			Noclip(g_Fivem.GetCamGameplayDirector()->GetFollowPedCamera()->GetViewAngles());

			CWeaponManager* WeaponManager = g_Fivem.GetLocalPlayerInfo().Ped->GetWeaponManager();
			if (WeaponManager)
			{
				uint64_t WeaponInfo = (uint64_t)WeaponManager->GetWeaponInfo();
				if (WeaponInfo)
				{
					static float Spread = FrameWork::Memory::ReadMemory<float>(WeaponInfo + 0x74);
					static uint32_t RecoilShakeHash = FrameWork::Memory::ReadMemory<uint32_t>(WeaponInfo + 0x2E4);
					static uint32_t RecoilShakeHashFP = FrameWork::Memory::ReadMemory<uint32_t>(WeaponInfo + 0x2E8);
					static float RecoveryTime = FrameWork::Memory::ReadMemory<float>(WeaponInfo + 0x74 + 0x18);
					static float RecoilShakeAmplitude = FrameWork::Memory::ReadMemory<float>(WeaponInfo + 0x2E4 + 0x10);

					static bool RemoveSpreadOn = false;
					if (RemoveSpreadOn != g_options.misc.weapon.nospread)
					{
						FrameWork::Memory::WriteMemory(WeaponInfo + 0x74, g_options.misc.weapon.nospread ? 0.f : Spread);

						RemoveSpreadOn = g_options.misc.weapon.nospread;
					}

					static bool RemoveRecoilOn = false;
					if (RemoveRecoilOn != g_options.misc.weapon.norecoil)
					{
						FrameWork::Memory::WriteMemory(WeaponInfo + 0x2E4, g_options.misc.weapon.norecoil ? 0 : RecoilShakeHash);
						FrameWork::Memory::WriteMemory(WeaponInfo + 0x2E8, g_options.misc.weapon.norecoil ? 0 : RecoilShakeHashFP);
						FrameWork::Memory::WriteMemory(WeaponInfo + 0x2E4 + 0x10, g_options.misc.weapon.norecoil ? 0.01f : RecoilShakeAmplitude);
						FrameWork::Memory::WriteMemory(WeaponInfo + 0x74 + 0x18, g_options.misc.weapon.norecoil ? 210.00f : RecoveryTime);

						RemoveRecoilOn = g_options.misc.weapon.norecoil;
					}
				}
			}

			if (g_Fivem.GetLocalPlayerInfo().Ped->HasConfigFlag(CPED_CONFIG_FLAG_InVehicle))
			{
				CVehicle* CurrentVehicle = g_Fivem.GetLocalPlayerInfo().Ped->GetLastVehicle();
				if (CurrentVehicle)
				{
					TPVehicle = true;
					TPModelInfo = CurrentVehicle->GetModelInfo();
					TPObject = (uint64_t)CurrentVehicle;
					TPNavigation = CurrentVehicle->GetNavigation();
					TPPosition = CurrentVehicle->GetCoordinate();
				}
				else
				{
					TPVehicle = false;
					TPModelInfo = g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo();
					TPObject = (uint64_t)g_Fivem.GetLocalPlayerInfo().Ped;
					TPNavigation = g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation();
					TPPosition = g_Fivem.GetLocalPlayerInfo().Ped->GetCoordinate();
				}
			}
			else
			{
				TPVehicle = false;
				TPModelInfo = g_Fivem.GetLocalPlayerInfo().Ped->GetModelInfo();
				TPObject = (uint64_t)g_Fivem.GetLocalPlayerInfo().Ped;
				TPNavigation = g_Fivem.GetLocalPlayerInfo().Ped->GetNavigation();
				TPPosition = g_Fivem.GetLocalPlayerInfo().Ped->GetCoordinate();
			}
		}
	}
}