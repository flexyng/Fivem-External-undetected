#include "aimbot.hpp"
#include "../../../Options.hpp"

namespace Cheat
{
    constexpr int HEAD_INDEX = 0;

    void AimBot::RunThread()
    {
        while (!g_options.menu.shutdown)
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));

            if (g_options.menu.shutdown) {
                break;
            }

            if (!g_options.aim.aimbot.enabled || !g_Fivem.GetLocalPlayerInfo().Ped) {
                continue;
            }

            Entity closestEntity;
            if (!g_Fivem.FindClosestEntity(
                g_options.aim.aimbot.fov,
                g_options.aim.aimbot.maxdistance,
                g_options.aim.aimbot.allownpc,
                &closestEntity)) {
                continue;
            }

            if (!GetAsyncKeyState(g_options.aim.aimbot.bind) ||
                closestEntity.StaticInfo.IsFriend && !closestEntity.Visible) {
                continue;
            }

            Vector3D bonePos = g_Fivem.GetBonePosVec3(closestEntity, SKEL_Head);

            g_Fivem.ProcessCameraMovement(
                bonePos,
                g_options.aim.aimbot.xsmooth,
                g_options.aim.aimbot.ysmooth);
        }
    }
}