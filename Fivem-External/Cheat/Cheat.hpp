#pragma once

/*
#include <FrameWork/Dependencies/ImGui/imgui.h>
#include <FrameWork/Dependencies/ImGui/imgui_impl_dx11.h>
#include <FrameWork/Dependencies/ImGui/imgui_impl_win32.h>
#include <FrameWork/Dependencies/ImGui/imgui_internal.h>
*/
#include "../FrameWork/Dependencies/ImGui/imgui.h"
#include "../FrameWork/Dependencies/ImGui/imgui_impl_dx11.h"
#include "../FrameWork/Dependencies/ImGui/imgui_impl_win32.h"
#include "../FrameWork/Dependencies/ImGui/imgui_internal.h"
#include "FivemSDK/Fivem.hpp"
#include "Features/esp/esp.hpp"
#include "Features/esp/vehicle_esp.hpp"
#include "Features/legit/aimbot/aimbot.hpp"
#include "Features/legit/triggerbot/triggerbot.hpp"
#include "Features/legit/silent_aim/silent_aim.hpp"
#include "Features/rage/rage.hpp"
#include "Features/legit/magicbullet/magicbullet.hpp"
#include "Options.hpp"

namespace Cheat
{
	void Initialize();
	void ShutDown();
}
