#include "Fivem.hpp"
#include <thread>
#include <future>
#include <random>
#include <mutex>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <thread>
#include "FrameWork/FrameWork.hpp"
#include "../Options.hpp"

inline uintptr_t ResolvePattern(const std::string& pattern, int Relative) {
	uintptr_t addr = FrameWork::Memory::FindSignatureStr(pattern);

	//std::cout << "adr: " << std::hex << addr << std::endl;
	if (!addr) {
		return 0;
	}

	auto relativeOffset = FrameWork::Memory::ReadMemory<int32_t>(addr + Relative);

	uintptr_t resolvedAddr = addr + 7 + relativeOffset;

	return resolvedAddr;
}

inline uintptr_t Resolve(uintptr_t addr, int Relative) {

	auto relativeOffset = FrameWork::Memory::ReadMemory<int32_t>(addr + Relative);

	uintptr_t resolvedAddr = addr + 7 + relativeOffset;
	return resolvedAddr;
}

uintptr_t ResolvePatternOffset(const std::string& pattern, int offsetPosition) {
	uintptr_t addr = FrameWork::Memory::FindSignatureStr(pattern);
	if (!addr) {
		return 0;
	}

	int32_t offset = FrameWork::Memory::ReadMemory<int32_t>(addr + offsetPosition);

	return offset;
}

typedef struct _ModuleInfo {
	DWORD_PTR baseAddress;
	DWORD size;
} ModuleInfo;

ModuleInfo GetMainModuleInfo(DWORD processId)
{
	ModuleInfo info = { 0 };

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (snapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);

		if (Module32First(snapshot, &moduleEntry)) // primeiro módulo é o .exe
		{
			info.baseAddress = (DWORD_PTR)moduleEntry.modBaseAddr;
			info.size = moduleEntry.modBaseSize;
		}

		CloseHandle(snapshot);
	}

	return info;
}

namespace Cheat
{
	uint64_t EntityType;
	uint64_t Armor;
	uint64_t BoneManager;
	uint64_t PlayerInfo;
	uint64_t PlayerNetID;
	uint64_t WeaponManager;
	uint64_t FragInsNmGTA;
	uint64_t ConfigFlags;
	uint64_t MaxHealth;
	uint64_t LastVehicle;
	uint64_t Driver;
	uint64_t DoorLock;
	uint64_t Stamina;
	uint64_t VisibleFlag;
	uint64_t entity;
	uint64_t PlayerSpeed;
	uint64_t Handling;
	uint64_t AmmoType;
	uint64_t AmmoExplosiveType;

	void FivemSDK::Intialize()
	{
		if (bIsIntialized)
			return;

		auto g_hGameWindow = FindWindowA("grcWindow", nullptr);
		GetWindowThreadProcessId(g_hGameWindow, &Pid);

		FrameWork::Memory::AttachProces(Pid);

		auto info = GetMainModuleInfo(Pid);

		ModuleBase = info.baseAddress;

		FrameWork::Memory::moduleBase = info.baseAddress;
		FrameWork::Memory::moduleSize = info.size;

		World = ResolvePattern("48 8B 05 ? ? ? ? 33 D2 48 8B 40 08 8A CA 48 85 C0 74 16 48 8B", 3);

		//std::cout << std::hex << FrameWork::Memory::moduleBase << std::endl;

		ReplayInterface = ResolvePattern("48 8B 05 ?? ?? ?? ?? 66 89 0D ?? ?? ?? ?? 4C 89 2C D0", 3);
		ViewPort = ResolvePattern("48 8B 15 ? ? ? ? 48 8D 2D ? ? ? ? 48 8B CD", 3);
		Camera = ResolvePattern("4C 8B 35 ? ? ? ? 33 FF 32 DB", 3);
		Camera2 = ResolvePattern("4C 8B 35 ? ? ? ? 33 FF 32 DB", 3);
		bIsPlayerAiming = ModuleBase + 0x2D3839C;
		//48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 85 C9 74 05 E8 ?? ?? ?? ?? 8A CB
		PlayerAimingAt = ResolvePattern("48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 85 C9 74 05 E8 ?? ?? ?? ?? 8A CB", 3);
		HandleBullet = ResolvePattern("48 8D 45 ?? F3 0F 10 00 F3 0F 10 48 ?? F3 0F 11 45", 3);
		BlipList = ResolvePattern("4C 8D 35 ? ? ? ? 3B 35 ? ? ? ? 74 ? 49 8B 3E", 3);

		EntityType = 0x1098;
		PlayerSpeed = ResolvePatternOffset("0f 2f c2 77 ? f3 0f 10 83 ? ? ? ? 48 83 c4", 9) - g_Fivem.GetModuleBase();
		Armor = ResolvePatternOffset("f3 0f 10 8b ? ? ? ? f3 0f 2c d1", 4) - g_Fivem.GetModuleBase();
		BoneManager = 0x430;
		PlayerInfo = ResolvePatternOffset("48 8b 89 ? ? ? ? 48 85 c9 74 ? 48 8d 81 ? ? ? ? c3", 3) - g_Fivem.GetModuleBase();
		PlayerNetID = 0xE8;
		WeaponManager = ResolvePatternOffset("48 8b 83 ? ? ? ? 44 8a fe", 3) - g_Fivem.GetModuleBase();
		FragInsNmGTA = ResolvePatternOffset("48 3b 83 ? ? ? ? 74 ? 48 8b 83 ? ? ? ? 48 85 c0 74 ? b9", 3);
		//std::cout << std::endl << std::hex << FragInsNmGTA << std::endl;
		ConfigFlags = ResolvePatternOffset("8b 86 ? ? ? ? c1 e8 ? a8 ? 74 ? 48 8b 97", 2) - g_Fivem.GetModuleBase();

		//std::cout << std::hex << ConfigFlags << std::endl;
		MaxHealth = 0x284;
		LastVehicle = 0xD10;
		Driver = 0xC90;
		DoorLock = 0x13C0;
		Stamina = 0xCF4;
		VisibleFlag = 0x145C;
		Handling = 0x918;
		AmmoType = 0x20;
		AmmoExplosiveType = 0x24;

		bIsIntialized = true;

		/*if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2372_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2372_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2372_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2372;
				ModuleName = ("FiveM_b2372_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2372_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2372;
				ModuleName = ("FiveM_b2372_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x252DCD8;
				ReplayInterface = ModuleBase + 0x1F05208;
				ViewPort = ModuleBase + 0x1F9E9F0;
				Camera = ModuleBase + 0x1F9F898;
				Camera2 = ModuleBase + 0x1F9F5C8;
				bIsPlayerAiming = ModuleBase + 0x2BF7CF0;
				PlayerAimingAt = ModuleBase + 0x1FACDE0;
				HandleBullet = ModuleBase + 0xFE7EF8;
				BlipList = ModuleBase + 0x1F9FFA0;



				EntityType = 0x10B8;
				Armor = 0x14E0;
				BoneManager = 0x430;
				PlayerInfo = 0x10C8;
				PlayerSpeed = 0x0CF0;
				PlayerNetID = 0x88;
				WeaponManager = 0x10D8;
				FragInsNmGTA = 0x1400;
				ConfigFlags = 0x1414;
				MaxHealth = 0x2A0;
				LastVehicle = 0xD30;
				Driver = 0xC68;
				DoorLock = 0x1390;
				Stamina = 0xCD4;
				VisibleFlag = 0x142C;
				Handling = 0x938;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				LocalPlayer = FrameWork::Memory::ReadMemory<float>(World + 0x8);

				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2612_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2612_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2612_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2612;
				ModuleName = ("FiveM_b2612_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2612_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2612;
				ModuleName = ("FiveM_b2612_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x2567DB0;
				ReplayInterface = ModuleBase + 0x1F77EF0;
				ViewPort = ModuleBase + 0x1FD8570;
				Camera = ModuleBase + 0x1FD9418;
				Camera2 = ModuleBase + 0x20D9868;
				bIsPlayerAiming = ModuleBase + 0x2C33DB0;
				PlayerAimingAt = ModuleBase + 0x1FE6960;
				HandleBullet = ModuleBase + 0xFF340C;
				BlipList = ModuleBase + 0x1FDDD20;

				EntityType = 0x10B8;
				Armor = 0x1530;
				BoneManager = 0x430;
				PlayerInfo = 0x10C8;
				PlayerNetID = 0x88;
				WeaponManager = 0x10D8;
				PlayerSpeed = 0x0CF0;
				FragInsNmGTA = 0x1450;
				ConfigFlags = 0x1464;
				MaxHealth = 0x2A0;
				LastVehicle = 0xD30;
				Driver = 0xC68;
				DoorLock = 0x1390;
				Stamina = 0xCF4;
				VisibleFlag = 0x147C;
				Handling = 0x938;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2699_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2699_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2699_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2699;
				ModuleName = ("FiveM_b2699_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2699_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2699;
				ModuleName = ("FiveM_b2699_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x26684D8;
				ReplayInterface = ModuleBase + 0x20304C8;
				ViewPort = ModuleBase + 0x20D8C90;
				Camera = ModuleBase + 0x20D9B38;
				Camera2 = ModuleBase + 0x20D9868;
				bIsPlayerAiming = ModuleBase + 0x2DA1950;
				PlayerAimingAt = ModuleBase + 0x20E6CC0;
				HandleBullet = ModuleBase + 0xFF9D90;
				BlipList = ModuleBase + 0x20E1420;

				EntityType = 0x10B8;
				Armor = 0x1530;
				BoneManager = 0x430;
				PlayerInfo = 0x10C8;
				PlayerNetID = 0x88;
				WeaponManager = 0x10D8;
				PlayerSpeed = 0x0CF0;
				FragInsNmGTA = 0x1450;
				ConfigFlags = 0x1464;
				MaxHealth = 0x2A0;
				LastVehicle = 0xD30;
				Driver = 0xC68;
				DoorLock = 0x1390;
				Stamina = 0xCF4;
				VisibleFlag = 0x147C;
				Handling = 0x938;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				LocalPlayer = FrameWork::Memory::ReadMemory<float>(World + 0x8);

				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2189_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2189_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2189_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2189;
				ModuleName = ("FiveM_b2189_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2189_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2189;
				ModuleName = ("FiveM_b2189_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x24E6D90;
				ReplayInterface = ModuleBase + 0x1EE18A8;
				ViewPort = ModuleBase + 0x1F888C0;
				Camera = ModuleBase + 0x1F89768;
				Camera2 = ModuleBase + 0x0;
				bIsPlayerAiming = ModuleBase + 0x2BC0FCC;
				PlayerAimingAt = ModuleBase + 0x1F7CBD0;
				HandleBullet = ModuleBase + 0xFE2154;
				BlipList = ModuleBase + 0x1F6EF80;

				PlayerSpeed = 0x0CF0;
				EntityType = 0x10B8;
				Armor = 0x14E0;
				BoneManager = 0x430;
				PlayerInfo = 0x10C8;
				PlayerNetID = 0x78;
				WeaponManager = 0x10D8;
				FragInsNmGTA = 0x1400;
				ConfigFlags = 0x1414;
				MaxHealth = 0x2A0;
				LastVehicle = 0xD30;
				Driver = 0xC68;
				DoorLock = 0x1390;
				Stamina = 0xCD4;
				VisibleFlag = 0x142C;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				Handling = 0x938;
				LocalPlayer = FrameWork::Memory::ReadMemory<float>(World + 0x8);

				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2802_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2802_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2802_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2802;
				ModuleName = ("FiveM_b2802_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2802_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2802;
				ModuleName = ("FiveM_b2802_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x254D448;
				ReplayInterface = ModuleBase + 0x1F5B820;
				ViewPort = ModuleBase + 0x1FBC100;
				Camera = ModuleBase + 0x1FBCFA8;
				Camera2 = ModuleBase + 0x1FBCCD8;
				bIsPlayerAiming = ModuleBase + 0x2CA3594;
				PlayerAimingAt = ModuleBase + 0x1FCA160;
				HandleBullet = ModuleBase + 0xFF716C;
				BlipList = ModuleBase + 0x1FBD6E0;

				EntityType = 0x1098;
				PlayerSpeed = 0x0CF0;
				Armor = 0x1530;
				BoneManager = 0x410;
				PlayerInfo = 0x10A8;
				PlayerNetID = 0x88;
				WeaponManager = 0x10B8;
				FragInsNmGTA = 0x1430;
				ConfigFlags = 0x1444;
				MaxHealth = 0x284;
				LastVehicle = 0xD10;
				Driver = 0xC48;
				DoorLock = 0x1370;
				Stamina = 0xCF4;
				VisibleFlag = 0x145C;
				Handling = 0x918;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				LocalPlayer = FrameWork::Memory::ReadMemory<float>(World + 0x8);

				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2060_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2060_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2060_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2060;
				ModuleName = ("FiveM_b2060_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2060_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2060;
				ModuleName = ("FiveM_b2060_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x24C8858;
				ReplayInterface = ModuleBase + 0x1EC3828;
				ViewPort = ModuleBase + 0x1F6A7E0;
				Camera = ModuleBase + 0x1F6B940;
				Camera2 = ModuleBase + 0x1F6B3C0;
				bIsPlayerAiming = ModuleBase + 0x2B958CC;
				PlayerAimingAt = ModuleBase + 0x1F5EB20;
				HandleBullet = ModuleBase + 0xFCE6EC;
				BlipList = ModuleBase + 0x1F4F940;

				EntityType = 0x10B8;
				Armor = 0x14E0;
				BoneManager = 0x430;
				PlayerInfo = 0x10A8;
				PlayerSpeed = 0x0CF0;
				PlayerNetID = 0x78;
				WeaponManager = 0x10D8;
				FragInsNmGTA = 0x1400;
				ConfigFlags = 0x1414;
				MaxHealth = 0x2A0;
				LastVehicle = 0xD28;
				Driver = 0xC68;
				DoorLock = 0x1390;
				Stamina = 0xCD4;
				VisibleFlag = 0x142C;
				Handling = 0x938;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				LocalPlayer = FrameWork::Memory::ReadMemory<float>(World + 0x8);

				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2545_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2545_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2545_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2545;
				ModuleName = ("FiveM_b2545_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2545_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2545;
				ModuleName = ("FiveM_b2545_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x25667E8;
				ReplayInterface = ModuleBase + 0x1F2E7A8;
				ViewPort = ModuleBase + 0x1FD6F70;
				Camera = ModuleBase + 0x1FD7E18;
				Camera2 = ModuleBase + 0x1FD7B48;
				bIsPlayerAiming = ModuleBase + 0x2C32630;
				PlayerAimingAt = ModuleBase + 0x1FE5360;
				HandleBullet = ModuleBase + 0xFF1B40;
				BlipList = ModuleBase + 0x1F4F940;

				EntityType = 0x10B8;
				Armor = 0x1530;
				BoneManager = 0x430;
				PlayerSpeed = 0x0CF0;
				PlayerInfo = 0x10A8;
				PlayerNetID = 0x88;
				WeaponManager = 0x10D8;
				FragInsNmGTA = 0x1450;
				ConfigFlags = 0x1464;
				MaxHealth = 0x2A0;
				LastVehicle = 0xD30;
				Driver = 0xC68;
				DoorLock = 0x1390;
				Stamina = 0xCF4;
				VisibleFlag = 0x147C;
				Handling = 0x938;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b2944_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b2944_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2944_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b2944;
				ModuleName = ("FiveM_b2944_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b2944_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b2944;
				ModuleName = ("FiveM_b2944_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x257BEA0;
				ReplayInterface = ModuleBase + 0x1F42068;
				ViewPort = ModuleBase + 0x1FEAAC0;
				Camera = ModuleBase + 0x1FEB968;
				Camera2 = ModuleBase + 0x1FEB698;
				bIsPlayerAiming = ModuleBase + 0x2CE2A94;
				PlayerAimingAt = ModuleBase + 0x1FF8AF0;
				HandleBullet = ModuleBase + 0x1003F80;
				BlipList = ModuleBase + 0x1FF3130;

				EntityType = 0x1098;
				PlayerSpeed = 0x0D40;
				Armor = 0x150C;
				BoneManager = 0x430;
				PlayerInfo = 0x10A8;
				PlayerNetID = 0xE8;
				WeaponManager = 0x10B8;
				FragInsNmGTA = 0x1430;
				ConfigFlags = 0x1444;
				MaxHealth = 0x284;
				LastVehicle = 0xD10;
				Driver = 0xC48;
				DoorLock = 0x1370;
				Stamina = 0xCF4;
				VisibleFlag = 0x145C;
				Handling = 0x918;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				bIsIntialized = true;
			}
		}
		else if (FrameWork::Memory::GetProcessPidByName((L"FiveM_b3095_GameProcess.exe")) || FrameWork::Memory::GetProcessPidByName((L"FiveM_b3095_GTAProcess.exe")))
		{
			Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b3095_GameProcess.exe"));
			if (Pid)
			{
				GameVersion = GAME_VERSION_GAME_b3095;
				ModuleName = ("FiveM_b3095_GameProcess.exe");
			}
			else
			{
				Pid = FrameWork::Memory::GetProcessPidByName((L"FiveM_b3095_GTAProcess.exe"));
				GameVersion = GAME_VERSION_GTA_b3095;
				ModuleName = ("FiveM_b3095_GTAProcess.exe");
			}

			ModuleBase = FrameWork::Memory::GetModuleBaseByName(Pid, FrameWork::Misc::String2WString(ModuleName));
			if (ModuleBase)
			{
				World = ModuleBase + 0x2593320;
				ReplayInterface = ModuleBase + 0x1F58B58;
				ViewPort = ModuleBase + 0x20019E0;
				Camera = ModuleBase + 0x2002888;
				Camera2 = ModuleBase + 0x20025B8;
				bIsPlayerAiming = ModuleBase + 0x2D3839C;
				PlayerAimingAt = ModuleBase + 0x200FA10;
				HandleBullet = ModuleBase + 0x100F5A4;
				BlipList = ModuleBase + 0x2002FA0;

				EntityType = 0x1098;
				PlayerSpeed = 0x0D40;
				Armor = 0x150C;
				BoneManager = 0x430;
				PlayerInfo = 0x10A8;
				PlayerNetID = 0xE8;
				WeaponManager = 0x10B8;
				FragInsNmGTA = 0x1430;
				ConfigFlags = 0x1444;
				MaxHealth = 0x284;
				LastVehicle = 0xD10;
				Driver = 0xC90;
				DoorLock = 0x13C0;
				Stamina = 0xCF4;
				VisibleFlag = 0x145C;
				Handling = 0x918;
				AmmoType = 0x20;
				AmmoExplosiveType = 0x24;
				bIsIntialized = true;
			}
		}*/

		//if (bIsIntialized)
		//	FrameWork::Memory::AttachProces(Pid);
	}

	bool FivemSDK::UpdateEntities()
	{
		if (!pWorld)
		{
			pWorld = (CWorld*)FrameWork::Memory::ReadMemory<uint64_t>(World);

			if (!pWorld)
				return false;
		}

		pLocalPlayer = pWorld->LocalPlayer();

		if (!pLocalPlayer)
			return false;

		static bool Logged = false;
		if (!Logged)
		{
			Logged = true;
		}

		if (!pReplayInterface)
		{
			pReplayInterface = (CReplayInterface*)FrameWork::Memory::ReadMemory<uint64_t>(ReplayInterface);

			if (!pReplayInterface)
				return false;
		}

		if (!pPedInterface)
		{
			pPedInterface = pReplayInterface->PedInterface();

			if (!pPedInterface)
				return false;
		}

		if (!pCamGameplayDirector)
		{
			pCamGameplayDirector = (CCamGameplayDirector*)FrameWork::Memory::ReadMemory<uint64_t>(Camera);

			if (!pCamGameplayDirector)
				return false;
		}


		LockLists.lock();

		EntityList.clear();
		EntityList.shrink_to_fit();

		for (size_t i = 0; i < pPedInterface->PedMaximum(); i++)
		{
			CPed* Ped = pPedInterface->PedList()->Ped(i);
			if (!Ped)
				continue;

			if ((uint64_t)Ped > 0xCCCCCCCCCCCCC)
				continue;

			if (Ped->IsAnimal())
				continue;

			if (Ped->GetHealth() <= 0 || Ped->GetHealth() <= 101.f)
				continue;

			PedStaticInfo StaticInfo;

			{
				StaticInfo.Ped = Ped;
				StaticInfo.iIndex = i;
				StaticInfo.bIsLocalPlayer = (Ped == pLocalPlayer);
				StaticInfo.bIsNPC = Ped->IsNPC();

				if (FriendList.find(Ped) != FriendList.end())
					StaticInfo.IsFriend = true;
				else
					StaticInfo.IsFriend = false;

				if (!StaticInfo.bIsNPC)
				{
					StaticInfo.NetId = Ped->GetPlayerInfo()->GetPlayerID();
				}
				else
				{
					StaticInfo.NetId = -1;
				}

				StaticInfo.crSkeletonData = FrameWork::Memory::ReadMemory<uint64_t>(FrameWork::Memory::ReadMemory<uint64_t>(FrameWork::Memory::ReadMemory<uint64_t>(Ped + FragInsNmGTA) + 0x68) + 0x178);
			}

			Entity CurrentEntity;
			CurrentEntity.StaticInfo = StaticInfo;

			if (CurrentEntity.StaticInfo.Name.empty())
			{
				if (!PlayersInfo.empty() && !PlayerIdToName.empty())
				{
					auto it = PlayerIdToName.find(CurrentEntity.StaticInfo.NetId);
					if (it != PlayerIdToName.end() && CurrentEntity.StaticInfo.NetId != -1)
					{
						CurrentEntity.StaticInfo.Name = it->second;
					}
					else
					{
						CurrentEntity.StaticInfo.Name = ("username");
					}
				}
			}

			CurrentEntity.Cordinates = Ped->GetCoordinate();

			CurrentEntity.HeadPos = WorldToScreen(GetBonePosVec3(CurrentEntity, SKEL_Head));

			CurrentEntity.Visible = Ped->IsVisible();

			if (CurrentEntity.StaticInfo.bIsLocalPlayer)
			{
				LocalPlayerInfo.Ped = Ped;
				LanGame = StaticInfo.bIsNPC;
				LocalPlayerInfo.iIndex = i;
				LocalPlayerInfo.WorldPos = CurrentEntity.Cordinates;
				LocalPlayerInfo.ScreenPos = WorldToScreen(CurrentEntity.Cordinates);
			}

			EntityList.push_back(CurrentEntity);
		}

		LockLists.unlock();

		if (!LanGame)
		{
			if (ServerIp.size() < 2)
			{
				if (FivemFolder.empty() || CrashoMetryLocation.empty())
				{
					HKEY hKey;
					WCHAR Buffer[MAX_PATH];
					DWORD BufferSize = sizeof(Buffer);
					if (SafeCall(RegOpenKeyEx)(HKEY_CURRENT_USER, (L"SOFTWARE\\CitizenFX\\FiveM"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
					{
						if (RegQueryValueEx(hKey, (L"Last Run Location"), NULL, NULL, (LPBYTE)Buffer, &BufferSize) == ERROR_SUCCESS)
						{
							std::string AAA = FrameWork::Misc::Wstring2String(std::wstring(Buffer)) + ("data\\cache\\crashometry");
							if (std::filesystem::exists(AAA))
							{
								FivemFolder = FrameWork::Misc::Wstring2String(std::wstring(Buffer));
								CrashoMetryLocation = FrameWork::Misc::Wstring2String(std::wstring(Buffer)) + ("data\\cache\\crashometry");
							}
						}

						SafeCall(RegCloseKey)(hKey);
					}
				}

				if (!CrashoMetryLocation.empty() || ServerIp.empty())
				{
					std::ifstream Crashometry(CrashoMetryLocation, std::ios::binary);

					std::string RawText;

					if (Crashometry.is_open())
					{
						std::vector<char> Buffer((std::istreambuf_iterator<char>(Crashometry)), std::istreambuf_iterator<char>());

						RawText = std::string(Buffer.begin(), Buffer.end());

						if (RawText.size() > 0)
						{
							size_t StartPos = RawText.find(("last_server_url"));
							if (StartPos != std::string::npos)
							{
								std::string TempServerIp = RawText.substr(StartPos + 15);

								if (TempServerIp.find(("last_server_url")))
								{
									TempServerIp = TempServerIp.substr(TempServerIp.find(("last_server_url")) + 15);
								}

								StartPos = TempServerIp.find(("last_server"));

								TempServerIp = TempServerIp.substr(StartPos + 11);

								StartPos = TempServerIp.find((":"));

								ServerIp = TempServerIp.substr(0, StartPos);

								ServerPort = TempServerIp.substr(StartPos + 1, StartPos + 5);
							}
						}

						Crashometry.close();
					}
				}

				if (ServerIp.size() < 5 || ServerPort.size() < 2)
				{
					LanGame = true;
				}
				else
				{
					std::thread([&]() { this->UpdateNamesThread(); }).detach();
				}
			}
		}

		return true;
	}

	bool FivemSDK::UpdateVehicles()
	{
		if (!pCamGameplayDirector)
			return false;

		if (!pVehicleInterface)
		{
			pVehicleInterface = pReplayInterface->VehicleInterface();

			if (!pVehicleInterface)
				return false;
		}

		LockLists2.lock();

		VehicleList.clear();
		VehicleList.shrink_to_fit();

		for (size_t i = 0; i < pVehicleInterface->VehicleMaximum(); i++)
		{
			CVehicle* Vehicle = pVehicleInterface->VehicleList()->Vehicle(i);
			if (!Vehicle)
				continue;

			if (Vehicle->GetCoordinate().DistTo(GetLocalPlayerInfo().WorldPos) > 600)
				continue;

			VehicleInfo CurrentVeh;
			CurrentVeh.Vehicle = Vehicle;
			CurrentVeh.ModelInfo = Vehicle->GetModelInfo();
			if (CurrentVeh.ModelInfo)
			{
				CurrentVeh.Name = FrameWork::Memory::ReadProcessMemoryString(CurrentVeh.ModelInfo + 0x298, 24);
				if (CurrentVeh.Name.size() > 24)
					continue;
			}
			else
				continue;
			CurrentVeh.iIndex = i;

			VehicleList.push_back(CurrentVeh);
		}

		LockLists2.unlock();
	}

	Vector3D GetBonePosByInstFragAndID(uint64_t crSkeletonData, unsigned int BoneID)
	{
		Matrix4x4 v4 = FrameWork::Memory::ReadMemory<Matrix4x4>(FrameWork::Memory::ReadMemory<uint64_t>(crSkeletonData + 0x8));
		Matrix4x4 Result = FrameWork::Memory::ReadMemory<Matrix4x4>(FrameWork::Memory::ReadMemory<uint64_t>(crSkeletonData + 0x18) + (BoneID << 6));

		Vector3D vec1(v4._11, v4._12, v4._13);
		Vector3D vec2(v4._21, v4._22, v4._23);
		Vector3D vec3(v4._31, v4._32, v4._33);
		Vector3D vec4(v4._41, v4._42, v4._43);
		Vector3D vec5(Result._41, Result._42, Result._43);

		return Vector3D(
			vec1.x * vec5.x + vec4.x + vec2.x * vec5.y + vec3.x * vec5.z,
			vec1.y * vec5.x + vec4.y + vec2.y * vec5.y + vec3.y * vec5.z,
			vec1.z * vec5.x + vec4.z + vec2.z * vec5.y + vec3.z * vec5.z
		);
	}

	Vector3D FivemSDK::GetBonePosVec3(Entity Ped, unsigned int Mask)
	{
		uint64_t FragInstNMGta = FrameWork::Memory::ReadMemory<uint64_t>(Ped.StaticInfo.Ped + FragInsNmGTA);
		if (FragInstNMGta)
		{
			Ped.StaticInfo.crSkeletonData = FrameWork::Memory::ReadMemory<uint64_t>(FrameWork::Memory::ReadMemory<uint64_t>(FragInstNMGta + 0x68) + 0x178);

			auto it = Ped.StaticInfo.MaskToBoneId.find(Mask);
			if (it == Ped.StaticInfo.MaskToBoneId.end())
			{
				unsigned int BoneId = 0;
				if (GetPedBoneIndex(Ped, Mask, BoneId))
				{
					if (BoneId)
					{
						Ped.StaticInfo.MaskToBoneId[Mask] = BoneId;
						return GetBonePosByInstFragAndID(Ped.StaticInfo.crSkeletonData, BoneId);
					}
				}
			}
			else
			{
				return GetBonePosByInstFragAndID(Ped.StaticInfo.crSkeletonData, it->second);
			}
		}

		return Vector3D(0, 0, 0);
	}

	CPed* FivemSDK::GetAimingEntity()
	{
		return (CPed*)FrameWork::Memory::ReadMemory<uint64_t>(PlayerAimingAt);
	}

	bool FivemSDK::IsPlayerAiming()
	{
		return FrameWork::Memory::ReadMemory<bool>(bIsPlayerAiming);
	}

	bool FivemSDK::GetPedBoneIndex(Entity Ped, unsigned int Mask, unsigned int& newIdx)
	{
		uint64_t crSkeletonData = FrameWork::Memory::ReadMemory<uint64_t>(Ped.StaticInfo.crSkeletonData);

		if (FrameWork::Memory::ReadMemory<int16_t>(crSkeletonData + 0x1A))
		{
			uint16_t v1 = FrameWork::Memory::ReadMemory<uint16_t>(crSkeletonData + 0x18);
			if (v1)
			{
				int64_t v2 = FrameWork::Memory::ReadMemory<int64_t>(crSkeletonData + 0x10);
				int Count = 0;
				for (int64_t i = FrameWork::Memory::ReadMemory<int64_t>(v2 + 0x8 * (Mask % v1)); ; i = FrameWork::Memory::ReadMemory<int64_t>(i + 0x8))
				{
					Count++;
					if (!i || i >= 0xCCCCCCCCCCCCCC || Count > 3)
						return false;

					int v5 = FrameWork::Memory::ReadMemory<int>(i);
					if (Mask == v5)
					{
						int v6 = FrameWork::Memory::ReadMemory<int>(i + 0x4);
						newIdx = v6;
						return true;
					}
				}
			}
		}
		else if (Mask < FrameWork::Memory::ReadMemory<uint64_t>(crSkeletonData + 0x5E))
		{
			newIdx = Mask;
			return true;
		}

		return false;
	}

	ImVec2 FivemSDK::GetClosestHitBox(Entity Ped)
	{
		ImVec2 Result = ImVec2(0, 0);

		ImVec2 Head = Ped.HeadPos;
		if (!g_Fivem.IsOnScreen(Head))
			return Result;

		ImVec2 Neck = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Ped, SKEL_Neck_1));
		if (!g_Fivem.IsOnScreen(Neck))
			return Result;

		ImVec2 Chest = g_Fivem.WorldToScreen(g_Fivem.GetBonePosVec3(Ped, SKEL_Spine3));
		if (!g_Fivem.IsOnScreen(Chest))
			return Result;

		ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

	//	float HeadDistance = Head.DistTo(Center);
		//float NeckDistance = Neck.DistTo(Center);
	//	float ChestDistance = Chest.DistTo(Center);

	//	float DistancesArray[] = { HeadDistance, NeckDistance, ChestDistance };

		//float Closest = DistancesArray[0];
		int ClosestBone = 0;

		for (int i = 0; i < 5; ++i)
		{
			
		}

		switch (ClosestBone)
		{
		case 0:
			return Head;
			break;
		case 1:
			return Neck;
			break;
		case 2:
			return Chest;
			break;
		default:
			return Head;
			break;
		}

	}

	bool FivemSDK::FindClosestEntity(float Fov, int MaxDistance, bool NPC, Entity* Output)
	{
		Entity Closest;
		float ClosestScreenDistance = FLT_MAX;
		float ClosestWorldDistance = FLT_MAX;

		bool Found = false;

		LockLists.lock();
		for (Entity Current : EntityList)
		{
			if (Current.StaticInfo.bIsLocalPlayer)
				continue;

			if (Current.StaticInfo.bIsNPC && !NPC)
				continue;

			float WorldDistance = Current.Cordinates.DistTo(GetLocalPlayerInfo().WorldPos);
			if (WorldDistance > MaxDistance)
				continue;

			ImVec2 ClosestBone = GetClosestHitBox(Current);
			if (ClosestBone.x == 0 && ClosestBone.y == 0)
				continue;

			ImVec2 Center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);

		//	float ScreenDistance = ClosestBone.DistTo(Center);

			//if (ScreenDistance > Fov)
			//	continue;

		
		}
		LockLists.unlock();

		if (Found)
		{
			*Output = Closest;
		}
		return Found;
	}

	void FivemSDK::ProcessCameraMovement(Vector3D WorldPosition, int SmoothHorizontal, int SmoothVertical)
	{
		if (!pCamGameplayDirector)
			return;

		auto FollowPedCamera = pCamGameplayDirector->GetFollowPedCamera();

		Vector3D CrosshairPosition = FollowPedCamera->GetCrosshairPosition();
		Vector3D ViewAngles = FollowPedCamera->GetViewAngles();
		float Distance = CrosshairPosition.DistTo(WorldPosition);

		auto futureAimAngles = std::async(std::launch::async, [&]() {
			return Vector3D(
				(WorldPosition.x - CrosshairPosition.x) / Distance,
				(WorldPosition.y - CrosshairPosition.y) / Distance,
				(WorldPosition.z - CrosshairPosition.z) / Distance
			);
			});

		Vector3D AimAngles = futureAimAngles.get();
		Vector3D CameraDelta(AimAngles.x - ViewAngles.x, AimAngles.y - ViewAngles.y, AimAngles.z - ViewAngles.z);
		Vector3D FinalAngles = AimAngles;

		auto futureSmoothHorizontal = std::async(std::launch::async, [&]() {
			if (SmoothHorizontal > 1) {
				FinalAngles.x = ViewAngles.x + CameraDelta.x / static_cast<float>(SmoothHorizontal);
				FinalAngles.y = ViewAngles.y + CameraDelta.y / static_cast<float>(SmoothHorizontal);
			}
			});

		auto futureSmoothVertical = std::async(std::launch::async, [&]() {
			if (SmoothVertical > 1)
				FinalAngles.z = ViewAngles.z + CameraDelta.z / static_cast<float>(SmoothVertical);
			});

		futureSmoothHorizontal.get();
		futureSmoothVertical.get();

		float AimbotFixZ = ViewAngles.z - FollowPedCamera->GetThirdpersonViewAngles().z;
		Vector3D ThirdPersonAngles = FinalAngles;
		ThirdPersonAngles.z -= AimbotFixZ;

		FollowPedCamera->SetThirdpersonViewAngles(ThirdPersonAngles);
		FollowPedCamera->SetViewAngles(FinalAngles);
	}

	void FivemSDK::TeleportObject(uintptr_t Object, uintptr_t Navigation, uintptr_t ModelInfo, Vector3D Position, Vector3D VisualPosition, bool Stop)
	{
		float BackupMagic = 0.f;
		if (Stop)
		{
			BackupMagic = FrameWork::Memory::ReadMemory<float>(ModelInfo + 0x2C);
			FrameWork::Memory::WriteMemory(ModelInfo + 0x2C, 0.f);
		}

		FrameWork::Memory::WriteMemory(Object + 0x90, VisualPosition);
		FrameWork::Memory::WriteMemory(Navigation + 0x50, Position);

		if (Stop)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(40));
			FrameWork::Memory::WriteMemory(ModelInfo + 0x2C, BackupMagic);
		}
	}

	ImVec2 FivemSDK::WorldToScreen(Vector3D Pos)
	{
		if (!pViewPort)
		{
			pViewPort = FrameWork::Memory::ReadMemory<uint64_t>(ViewPort);
			if (!pViewPort)
				return ImVec2(0, 0);
		}

		Matrix4x4 ViewMatrix = FrameWork::Memory::ReadMemory<Matrix4x4>(pViewPort + 0x24C);

		ViewMatrix.TransposeThisMatrix();

		Vector4D VecX(ViewMatrix._21, ViewMatrix._22, ViewMatrix._23, ViewMatrix._24);
		Vector4D VecY(ViewMatrix._31, ViewMatrix._32, ViewMatrix._33, ViewMatrix._34);
		Vector4D VecZ(ViewMatrix._41, ViewMatrix._42, ViewMatrix._43, ViewMatrix._44);

		Vector3D ScreenPos;
		ScreenPos.x = (VecX.x * Pos.x) + (VecX.y * Pos.y) + (VecX.z * Pos.z) + VecX.w;
		ScreenPos.y = (VecY.x * Pos.x) + (VecY.y * Pos.y) + (VecY.z * Pos.z) + VecY.w;
		ScreenPos.z = (VecZ.x * Pos.x) + (VecZ.y * Pos.y) + (VecZ.z * Pos.z) + VecZ.w;

		if (ScreenPos.z <= 0.1f)
			return ImVec2(0, 0);

		ScreenPos.z = 1.0f / ScreenPos.z;
		ScreenPos.x *= ScreenPos.z;
		ScreenPos.y *= ScreenPos.z;

		ScreenPos.x += ImGui::GetIO().DisplaySize.x / 2 + float(0.5f * ScreenPos.x * ImGui::GetIO().DisplaySize.x + 0.5f);
		ScreenPos.y = ImGui::GetIO().DisplaySize.y / 2 - float(0.5f * ScreenPos.y * ImGui::GetIO().DisplaySize.y + 0.5f);

		return ImVec2(ScreenPos.x, ScreenPos.y);
	}

	void FivemSDK::UpdateNamesThread()
	{
		while (!g_options.menu.shutdown)
		{
			if (ServerIp.size() > 5 && ServerPort.size() > 2)
			{
				PlayersInfo = GetServerInfo();

				for (const auto& Player : PlayersInfo)
				{
					PlayerIdToName[Player[("id")]] = Player[("name")];
				}

				AllEntitesList.clear();

				std::this_thread::sleep_for(std::chrono::seconds(10));
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

	nlohmann::json FivemSDK::GetServerInfo()
	{
		if (!ServerIp.empty())
		{
			std::string RawServerInfo = FrameWork::Misc::DownloadServerInfo(FrameWork::Misc::String2WString(ServerIp), std::atoi(ServerPort.c_str()));

			return nlohmann::json::parse(RawServerInfo);
		}

		return NULL;
	}

	bool FivemSDK::IsOnScreen(ImVec2 Pos)
	{
		if (Pos.x < 0.1f || Pos.y < 0.1 || Pos.x > ImGui::GetIO().DisplaySize.x || Pos.y > ImGui::GetIO().DisplaySize.y)
			return false;

		return true;
	}
}