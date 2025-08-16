#pragma once
#include <cstdint>

#include "Classes.hpp"
#include <mutex>

#include <FrameWork/FrameWork.hpp>
#include "../../FrameWork/Math/Vectors/Vector3D.hpp"

namespace Cheat
{
	enum GAME_VERSION
	{
		GAME_VERSION_GAME_b2372,
		GAME_VERSION_GTA_b2372,
		GAME_VERSION_GAME_b2612,
		GAME_VERSION_GTA_b2612,
		GAME_VERSION_GAME_b2699,
		GAME_VERSION_GTA_b2699,
		GAME_VERSION_GAME_b2189,
		GAME_VERSION_GTA_b2189,
		GAME_VERSION_GAME_b2802,
		GAME_VERSION_GTA_b2802,
		GAME_VERSION_GAME_b2060,
		GAME_VERSION_GTA_b2060,
		GAME_VERSION_GAME_b2545,
		GAME_VERSION_GTA_b2545,
		GAME_VERSION_GAME_b2944,
		GAME_VERSION_GTA_b2944,
		GAME_VERSION_GAME_b3095,
		GAME_VERSION_GTA_b3095,
		GAME_VERSION_GAME_b3258,
		GAME_VERSION_GTA_b3258,
	};

	struct PedStaticInfo
	{
		CPed* Ped;
		int iIndex;
		int NetId;
		bool bIsLocalPlayer;
		bool bIsNPC;
		std::string Name;
		uint64_t crSkeletonData;
		bool IsFriend = false;

		std::unordered_map<unsigned int, unsigned> MaskToBoneId;
	};

	struct Entity
	{
		PedStaticInfo StaticInfo;

		Vector3D Cordinates;
		bool Visible;
		ImVec2 HeadPos;
	};

	struct LocalPEDInfo
	{
		CPed* Ped;
		int iIndex;
		ImVec2 ScreenPos;
		Vector3D WorldPos;
	};

	struct VehicleInfo
	{
		CVehicle* Vehicle;
		std::string Name;
		uint64_t ModelInfo;
		int iIndex = -1;
	};

	class FivemSDK
	{
	public:
		void Intialize();
		bool UpdateEntities();
		bool UpdateVehicles();

		DWORD GetPid() { return Pid; }
		uint64_t GetModuleBase() { return ModuleBase; };
		LocalPEDInfo GetLocalPlayerInfo() { return LocalPlayerInfo; }
		std::vector<Entity> GetEntitiyList() { return EntityList; }
		std::vector<VehicleInfo> GetVehicleList() { return VehicleList; }
		uint64_t GetHandleBulletAddress() { return HandleBullet; }
		uint64_t GetBlipListAddress() { return BlipList; }
		CCamGameplayDirector* GetCamGameplayDirector() { return pCamGameplayDirector; }

		CPed* GetAimingEntity();
		bool IsPlayerAiming();

		Vector3D GetBonePosVec3(Entity Ped, unsigned int Mask);
		bool GetPedBoneIndex(Entity Ped, unsigned int Mask, unsigned int& newIdx);

		ImVec2 GetClosestHitBox(Entity Ped);
		bool FindClosestEntity(float Fov, int MaxDistance, bool NPC, Entity* Output);
		void SpectatePed(uint64_t Ped, bool Toggle);
		void ProcessCameraMovement(Vector3D WorldPosition, int SmoothHorizontal, int SmoothVertical);
		void TeleportObject(uintptr_t Object, uintptr_t Navigation, uintptr_t ModelInfo, Vector3D Position, Vector3D VisualPosition, bool Stop);

		ImVec2 WorldToScreen(Vector3D Pos);
		bool IsOnScreen(ImVec2 Pos);

		void UpdateNamesThread();

		nlohmann::json GetServerInfo();

		uint64_t Camera2;

		bool IsInitialized() { return bIsIntialized; }

		std::unordered_map<CPed*, PedStaticInfo> AllEntitesList;
		std::unordered_map<CPed*, PedStaticInfo> FriendList;

	private:
		bool bIsIntialized = false;

		uint64_t World;
		uint64_t ReplayInterface;
		uint64_t ViewPort;
		uint64_t Camera;
		uint64_t bIsPlayerAiming;
		uint64_t PlayerAimingAt;
		uint64_t HandleBullet;
		uint64_t Handling;
		uint64_t BlipList;
		uint64_t ViewAngles;
		uint64_t ViewAnglesPointer;
		uint64_t GameplayCamHolder;
		uint64_t GameplayCamTarget;

		DWORD Pid;
		uint64_t ModuleBase;
		std::string ModuleName;
		std::string FivemFolder;
		std::string CrashoMetryLocation;
		std::string ServerIp;
		std::string ServerPort;
		nlohmann::json PlayersInfo;
		std::unordered_map<int, std::string> PlayerIdToName;
		bool LanGame;
		GAME_VERSION GameVersion;

	private:
		CWorld* pWorld;
		CPed* pLocalPlayer;
		CReplayInterface* pReplayInterface;
		CPedInterface* pPedInterface;
		CVehicleInterface* pVehicleInterface;
		CCamGameplayDirector* pCamGameplayDirector;

		uint64_t pViewPort;

	private:
		std::mutex LockLists;
		std::mutex LockLists2;
		std::vector<Entity> EntityList;
		LocalPEDInfo LocalPlayerInfo;
		std::vector<VehicleInfo> VehicleList;
	};

	inline FivemSDK g_Fivem;
}