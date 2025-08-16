#include "MagicBullet.hpp"
#include "../Fivem-External/Cheat/Options.hpp"

namespace Cheat {
	void MagicBullet::RunThread()
	{
		while (!g_options.menu.shutdown)
		{
			std::this_thread::sleep_for(std::chrono::nanoseconds(1));

			if (!g_options.aim.silent.magicbullet) {
				continue;
			}

			Entity closestEntity;

			if (!g_Fivem.FindClosestEntity(
				g_options.aim.silent.fov,
				g_options.aim.silent.maxdistance,
				&g_options.aim.silent.allownpc,
				&closestEntity)) {
				continue;
			}

			auto bonePosition = g_Fivem.GetBonePosVec3(closestEntity, SKEL_Head);

			uintptr_t weaponManager = (uintptr_t)g_Fivem.GetLocalPlayerInfo().Ped->GetWeaponManager();
			uintptr_t weaponObg = FrameWork::Memory::ReadMemory<uintptr_t>(weaponManager + 0x78);

			uintptr_t CWeapon = FrameWork::Memory::ReadMemory<uintptr_t>(weaponObg + 0x320);

			if (GetAsyncKeyState(g_options.aim.silent.bind)) {
				auto heading = FrameWork::Memory::ReadMemory<Vector3D>(closestEntity.StaticInfo.Ped + 0x70);

				float distanceAhead = 5.f;
				Vector3D forwardVector = { 0.0f, 0.0f, distanceAhead };

				Vector3D spawnPosition = (bonePosition + heading + forwardVector);
				FrameWork::Memory::WriteMemory<Vector3D>(CWeapon + 0x20, spawnPosition);
			}
		}

	}
}