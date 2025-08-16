#pragma once
#include <Windows.h>

namespace Cheat
{
	class options
	{
	public:
		struct aim
		{
			struct aimbot
			{
				// bool
				bool enabled;
				bool allownpc;
				bool ragemode;
				bool drawfov;

				// int
				int bind = 0;
				int maxdistance = 250;
				int fov = 90;
				int xsmooth = 0;
				int ysmooth = 0;

				// float
				float fovcolor[4] = { 1.f, 1.f, 1.f, 1.f };
			}aimbot;

			struct silent
			{
				// bool
				bool enabled;
				bool allownpc;
				bool ragemode;
				bool drawfov;
				bool magicbullet;

				// int
				int bind = 0;
				int maxdistance = 250;
				int fov = 90;

				// float
				float fovcolor[4] = { 1.f, 1.f, 1.f, 1.f };
			}silent;

			struct trigger
			{
				// bool
				bool enabled;
				bool allownpc;
				bool ragemode;
				bool drawfov;

				// int
				int bind = 0;
				int maxdistance = 250;
				int fov = 90;
				int delay = 0;

				// float
				float fovcolor[4] = { 1.f, 1.f, 1.f, 1.f };
			}trigger;
		}aim;

		struct esp
		{
			struct player
			{
				// bool
				bool enabled = true;
				bool allowlocalplayer;
				bool allownpc;
				bool box;
				bool corner;
				bool skeleton;
				bool name;
				bool healthbar;
				bool armorbar;
				bool weaponname;
				bool distance;
				bool lines;
				bool chams;
				bool headdot;

				// int
				int healthbarpos;
				int renderingdistance = 250;
			}player;

			struct vehicle
			{
				// bool
				bool enabled;
				bool marker;
				bool box;
				bool distance;
				bool name;
				bool lines;

				// int
				int renderingdistance = 250;
			}vehicle;

			struct colors
			{
				// player
				float linescolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float boxcolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float distancecolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float weaponnamecolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float namecolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float skeletoncolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float friendcolor[4] = { 0.f, 0.f, 1.f, 1.f };

				// vehicle
				float vhmarkercolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float vhdistancecolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float vhnamecolor[4] = { 1.f, 1.f, 1.f, 1.f };
				float vhlinescolor[4] = { 1.f, 1.f, 1.f, 1.f };
			}colors;
		}esp;
		struct misc
		{
			struct localplayer
			{
				// bool
				bool noclip;
				bool shrink;
				bool superjump;
				bool seatbelt;
				bool stealcar;
				bool infinitestamina;
				bool antiheadshot;
				bool nocollision;
				bool noragdoll;
				bool speedwalk;
				bool godmode;
				bool semigod;
				bool usegodmodebind;
				bool overridecamdistance;
				bool sethealth;
				bool suicide;
				bool revive;
				bool bIsSpectatingEntity;
				bool Invisible;
				bool onepunch;
				bool beastjump;
				bool explomeele;
				bool beastmode;
				bool ReganerateArmor;
				bool freeze;

				// int
				int noclipbind;
				int noclipspeed;
				int godmodebind;
				uint64_t SpectatingEntity;

				// float
				float speedwalkvalue = 1.f;
				float camdistance;
				float health;
			}localplayer;

			struct weapon
			{
				bool fireammo;
				bool exploammo;
				bool nospread;
				bool norecoil;
				bool fastreload;
				bool doubleshoot;
				bool infiniteammo;

			}weapon;

			struct vehicle
			{
				// bool
				bool enabled = true;
				bool godmode;
				bool rocketboost;
				
				// int
				int unlockbind;
				int fixbind;

				// float
				float accelerationvalue;
				float tractionvalue;
			}vehicle;
		}misc;

		struct menu
		{
			// bool
			bool shutdown = false;
			bool streamproof = false;
			bool blockinput = false;

			// int
			int menubind = VK_INSERT;
		}menu;
	};
}

inline Cheat::options g_options;