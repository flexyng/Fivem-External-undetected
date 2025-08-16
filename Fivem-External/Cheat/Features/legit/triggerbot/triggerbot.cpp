#include "TriggerBot.hpp"

#include <thread>

#include "../../../Options.hpp"

namespace Cheat
{
    void TriggerBot::RunThread()
    {
        while (!g_options.menu.shutdown)
        {
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));

            if (!g_options.aim.trigger.enabled)
                continue;

            if (!g_Fivem.GetLocalPlayerInfo().Ped)
                continue;

            static bool Shooting = false;

            bool CanShoot = false;

            Entity closestEntity;
            if (g_Fivem.FindClosestEntity(
                g_options.aim.trigger.fov,
                g_options.aim.trigger.maxdistance,
                g_options.aim.trigger.allownpc,
                &closestEntity))
            {
                if (closestEntity.StaticInfo.IsFriend || !closestEntity.Visible)
                {
                    continue;
                }

                if (GetAsyncKeyState(g_options.aim.trigger.bind))
                {
                    CanShoot = true;
                }
            }

            if (CanShoot && !Shooting)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(g_options.aim.trigger.delay));

                INPUT input = { 0 };
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                SafeCall(SendInput)(1, &input, sizeof(INPUT));

                Shooting = true;
            }
            else if (Shooting && !CanShoot)
            {
                INPUT input = { 0 };
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SafeCall(SendInput)(1, &input, sizeof(INPUT));

                Shooting = false;
            }
        }
    }
}