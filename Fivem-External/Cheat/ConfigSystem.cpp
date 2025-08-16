#include "ConfigSystem.hpp"
#include <fstream>

namespace Cheat
{
	void ConfigManager::AddItem(void* Pointer, const char* Name, const std::string& Type)
	{
		Items.push_back(new C_ConfigItem(std::string(Name), Pointer, Type));
	}

	void ConfigManager::SetupItem(int* Pointer, float Value, const std::string& Name)
	{
		AddItem(Pointer, Name.c_str(), ("int"));
		*Pointer = Value;
	}

	void ConfigManager::SetupItem(float* Pointer, float Value, const std::string& Name)
	{
		AddItem(Pointer, Name.c_str(), ("float"));
		*Pointer = Value;
	}

	void ConfigManager::SetupItem(bool* Pointer, float Value, const std::string& Name)
	{
		AddItem(Pointer, Name.c_str(), ("bool"));
		*Pointer = Value;
	}

	void ConfigManager::Setup()
	{
		// aimbot
		SetupItem(&g_options.aim.aimbot.enabled, false, ("aimbotenabled"));
		SetupItem(&g_options.aim.aimbot.bind, 0, ("aimbotbind"));
		SetupItem(&g_options.aim.aimbot.allownpc, false, ("aimbotallownpc"));
		SetupItem(&g_options.aim.aimbot.maxdistance, 500, ("aimbotmaxdistance"));
		SetupItem(&g_options.aim.aimbot.fov, 90, ("aimbotfov"));
		SetupItem(&g_options.aim.aimbot.ysmooth, 80, ("aimbotysmooth"));
		SetupItem(&g_options.aim.aimbot.xsmooth, 80, ("aimbotxsmooth"));
		SetupItem(&g_options.aim.aimbot.ragemode, false, ("aimbotragemode"));
		SetupItem(&g_options.aim.aimbot.drawfov, 0, ("aimbotdrawfov"));
		SetupItem(&g_options.aim.aimbot.fovcolor[0], 1.f, ("aimbotfovcolor0"));
		SetupItem(&g_options.aim.aimbot.fovcolor[1], 1.f, ("aimbotfovcolor1"));
		SetupItem(&g_options.aim.aimbot.fovcolor[2], 1.f, ("aimbotfovcolor2"));
		SetupItem(&g_options.aim.aimbot.fovcolor[3], 1.f, ("aimbotfovcolor3"));

		// silent aim
		SetupItem(&g_options.aim.silent.enabled, false, ("silentenabled"));
		SetupItem(&g_options.aim.silent.fov, 90, ("silentfov"));
		SetupItem(&g_options.aim.silent.magicbullet, false, ("magicbullet"));
		SetupItem(&g_options.aim.silent.bind, 0, ("silentbind"));
		SetupItem(&g_options.aim.silent.allownpc, false, ("silentallownpc"));
		SetupItem(&g_options.aim.silent.maxdistance, 500, ("silentmaxdistance"));
		SetupItem(&g_options.aim.silent.ragemode, false, ("silentragemode"));
		SetupItem(&g_options.aim.silent.drawfov, 0, ("silentdrawfov"));
		SetupItem(&g_options.aim.silent.fovcolor[0], 1.f, ("silentfovcolor0"));
		SetupItem(&g_options.aim.silent.fovcolor[1], 1.f, ("silentfovcolor1"));
		SetupItem(&g_options.aim.silent.fovcolor[2], 1.f, ("silentfovcolor2"));
		SetupItem(&g_options.aim.silent.fovcolor[3], 1.f, ("silentfovcolor3"));

		// trigger
		SetupItem(&g_options.aim.trigger.enabled, false, ("triggerenabled"));
		SetupItem(&g_options.aim.trigger.bind, 0, ("triggerbind"));
		SetupItem(&g_options.aim.trigger.fov, 90, ("triggerfov"));
		SetupItem(&g_options.aim.trigger.allownpc, false, ("triggerallownpc"));
		SetupItem(&g_options.aim.trigger.maxdistance, 500, ("triggermaxdistance"));
		SetupItem(&g_options.aim.trigger.delay, 0, ("triggerdelay"));
		SetupItem(&g_options.aim.trigger.ragemode, false, ("triggerragemode"));
		SetupItem(&g_options.aim.trigger.drawfov, 0, ("triggerdrawfov"));
		SetupItem(&g_options.aim.trigger.fovcolor[0], 1.f, ("triggerfovcolor0"));
		SetupItem(&g_options.aim.trigger.fovcolor[1], 1.f, ("triggerfovcolor1"));
		SetupItem(&g_options.aim.trigger.fovcolor[2], 1.f, ("triggerfovcolor2"));
		SetupItem(&g_options.aim.trigger.fovcolor[3], 1.f, ("triggerfovcolor3"));

		// miscellaneous
		SetupItem(&g_options.misc.localplayer.antiheadshot, false, ("antiheadshot"));
		SetupItem(&g_options.misc.weapon.norecoil, false, ("norecoil"));
		SetupItem(&g_options.misc.weapon.nospread, false, ("nospread"));
		SetupItem(&g_options.misc.localplayer.speedwalk, false, ("speedwalk"));
		SetupItem(&g_options.misc.localplayer.speedwalkvalue, 1.f, ("speedwalkvalue"));
		SetupItem(&g_options.misc.weapon.fastreload, false, ("fastreload"));
		SetupItem(&g_options.misc.localplayer.godmode, false, ("godmode"));
		SetupItem(&g_options.misc.localplayer.godmodebind, 0, ("godmodebind"));
		SetupItem(&g_options.misc.localplayer.noragdoll, false, ("noragdoll"));
		SetupItem(&g_options.misc.localplayer.nocollision, false, ("nocollision"));
		SetupItem(&g_options.misc.localplayer.shrink, false, ("shrink"));
		SetupItem(&g_options.misc.localplayer.noclip, false, ("noclip"));
		SetupItem(&g_options.misc.localplayer.noclipbind, false, ("noclipbind"));
		SetupItem(&g_options.misc.localplayer.noclipspeed, 50, ("noclipspeed"));
		SetupItem(&g_options.misc.localplayer.usegodmodebind, false, ("usegodmodebind"));
		SetupItem(&g_options.misc.localplayer.infinitestamina, false, ("infinitestamina"));

		// esp player
		SetupItem(&g_options.esp.player.enabled, true, ("espenabled"));
		SetupItem(&g_options.esp.player.allowlocalplayer, false, ("espallowlocalplayer"));
		SetupItem(&g_options.esp.player.allownpc, false, ("espallownpc"));
		SetupItem(&g_options.esp.player.renderingdistance, 500, ("espplayerrenderingdistance"));
		SetupItem(&g_options.esp.player.box, false, ("espbox"));
		SetupItem(&g_options.esp.player.skeleton, false, ("espskeleton"));
		SetupItem(&g_options.esp.player.name, false, ("espname"));
		SetupItem(&g_options.esp.player.healthbar, false, ("esphealthbar"));
		SetupItem(&g_options.esp.player.armorbar, false, ("esparmorbar"));
		SetupItem(&g_options.esp.player.weaponname, false, ("espweaponname"));
		SetupItem(&g_options.esp.player.distance, false, ("espdistance"));
		SetupItem(&g_options.esp.player.lines, false, ("esplines"));

		// esp color
		SetupItem(&g_options.esp.colors.skeletoncolor[0], 1.f, ("esp_players_skelco0"));
		SetupItem(&g_options.esp.colors.skeletoncolor[1], 1.f, ("esp_players_skelco1"));
		SetupItem(&g_options.esp.colors.skeletoncolor[2], 1.f, ("esp_players_skelco2"));
		SetupItem(&g_options.esp.colors.skeletoncolor[3], 1.f, ("esp_players_skelco3"));
		SetupItem(&g_options.esp.colors.namecolor[0], 1.f, ("esp_players_NAMEco0"));
		SetupItem(&g_options.esp.colors.namecolor[1], 1.f, ("esp_players_NAMEco1"));
		SetupItem(&g_options.esp.colors.namecolor[2], 1.f, ("esp_players_NAMEco2"));
		SetupItem(&g_options.esp.colors.namecolor[3], 1.f, ("esp_players_NAMEco3"));
		SetupItem(&g_options.esp.colors.weaponnamecolor[0], 1.f, ("esp_players_weaponNAMEco0"));
		SetupItem(&g_options.esp.colors.weaponnamecolor[1], 1.f, ("esp_players_weaponNAMEco1"));
		SetupItem(&g_options.esp.colors.weaponnamecolor[2], 1.f, ("esp_players_weaponNAMEco2"));
		SetupItem(&g_options.esp.colors.weaponnamecolor[3], 1.f, ("esp_players_weaponNAMEco3"));
		SetupItem(&g_options.esp.colors.distancecolor[0], 1.f, ("esp_players_distanceco0"));
		SetupItem(&g_options.esp.colors.distancecolor[1], 1.f, ("esp_players_distanceco1"));
		SetupItem(&g_options.esp.colors.distancecolor[2], 1.f, ("esp_players_distanceco2"));
		SetupItem(&g_options.esp.colors.distancecolor[3], 1.f, ("esp_players_distanceco3"));
		SetupItem(&g_options.esp.colors.linescolor[0], 1.f, ("esp_players_snaplinesco0"));
		SetupItem(&g_options.esp.colors.linescolor[1], 1.f, ("esp_players_snaplinesco1"));
		SetupItem(&g_options.esp.colors.linescolor[2], 1.f, ("esp_players_snaplinesco2"));
		SetupItem(&g_options.esp.colors.linescolor[3], 1.f, ("esp_players_snaplinesco3"));
		SetupItem(&g_options.esp.colors.boxcolor[0], 1.f, ("esp_players_boxco0"));
		SetupItem(&g_options.esp.colors.boxcolor[1], 1.f, ("esp_players_boxco1"));
		SetupItem(&g_options.esp.colors.boxcolor[2], 1.f, ("esp_players_boxco2"));
		SetupItem(&g_options.esp.colors.boxcolor[3], 1.f, ("esp_players_boxco3"));

		// esp vehicle
		SetupItem(&g_options.esp.vehicle.enabled, false, ("esp_vehicles_enabled"));
		SetupItem(&g_options.esp.vehicle.name, false, ("esp_vehicles_name"));
		SetupItem(&g_options.esp.vehicle.distance, false, ("esp_vehicles_distance"));
		SetupItem(&g_options.esp.vehicle.marker, false, ("esp_vehicles_marker"));

		// vehicle modifications
		SetupItem(&g_options.misc.vehicle.enabled, false, ("enableacceleration"));
		SetupItem(&g_options.misc.vehicle.accelerationvalue, 1.f, ("accelerationvalue"));
		SetupItem(&g_options.misc.vehicle.tractionvalue, 1.f, ("tractionvalue"));
		SetupItem(&g_options.misc.vehicle.unlockbind, 0, ("unlockbind"));
		SetupItem(&g_options.misc.vehicle.fixbind, 0, ("fixbind"));

		// friend color
		SetupItem(&g_options.esp.colors.friendcolor[0], 1.f, ("esp_FriendsColor0"));
		SetupItem(&g_options.esp.colors.friendcolor[1], 1.f, ("esp_FriendsColor1"));
		SetupItem(&g_options.esp.colors.friendcolor[2], 1.f, ("esp_FriendsColor2"));
		SetupItem(&g_options.esp.colors.friendcolor[3], 1.f, ("esp_FriendsColor3"));

		// menu
		SetupItem(&g_options.menu.streamproof, false, ("streamproof"));
		SetupItem(&g_options.menu.menubind, VK_INSERT, ("menubind"));
	}

	void ConfigManager::LoadConfig()
	{
		static auto ReadFromFile = [](const std::string& filePath) -> std::string
			{
				std::ifstream inFile(filePath);
				if (!inFile.is_open()) {
					return "";
				}

				std::string fileContent((std::istreambuf_iterator<char>(inFile)),
					std::istreambuf_iterator<char>());
				inFile.close();
				return fileContent;
			};

		static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem*
			{
				for (int i = 0; i < (int)items.size(); i++)
					if (!items[i]->Name.compare(name))
						return items[i];

				return nullptr;
			};

		auto fileContent = ReadFromFile("C:\\Windows\\Panther\\UnattendGC\\setupact.log");

		if (fileContent.empty()) {
			return;
		}

		auto decoded_string = base64::decode(fileContent);

		if (decoded_string[0] != 't' ||
			decoded_string[1] != 'r' ||
			decoded_string[2] != 'a' ||
			decoded_string[3] != 'c' ||
			decoded_string[4] != 'k' ||
			decoded_string[5] != 'i' ||
			decoded_string[6] != 'n' ||
			decoded_string[7] != 'g')
			return;

		auto parsed_config = nlohmann::json::parse(decoded_string.erase(0, 8));

		nlohmann::json allJson = parsed_config;

		for (auto it = allJson.begin(); it != allJson.end(); ++it)
		{
			nlohmann::json j = *it;

			std::string name = j[("name")];
			std::string type = j[("type")];

			auto item = find_item(Items, name);

			if (item)
			{
				if (!type.compare(("int")))
					*(int*)item->Pointer = j[("value")].get<int>();
				else if (!type.compare(("float")))
					*(float*)item->Pointer = j[("value")].get<float>();
				else if (!type.compare(("bool")))
					*(bool*)item->Pointer = j[("value")].get<bool>();
			}
		}
	}

	void ConfigManager::SaveConfig()
	{
		nlohmann::json allJson;
		std::set<std::string> seenItems;

		for (auto it : Items)
		{
			if (seenItems.count(it->Name) > 0) {
				continue;
			}

			nlohmann::json j;

			j[("name")] = it->Name;
			j[("type")] = it->Type;

			if (!it->Type.compare(("int")))
				j[("value")] = (int)*(int*)it->Pointer;
			else if (!it->Type.compare(("float")))
				j[("value")] = (float)*(float*)it->Pointer;
			else if (!it->Type.compare(("bool")))
				j[("value")] = (bool)*(bool*)it->Pointer;

			allJson.push_back(j);
			seenItems.insert(it->Name);
		}

		auto str = base64::encode((std::string(("tracking")).append(allJson.dump(-1, '~'/*, true*/))).c_str());

		std::ofstream outFile("C:\\Windows\\Panther\\UnattendGC\\setupact.log");
		if (outFile.is_open()) {
			outFile << str;
			outFile.close();
		}
		else {
		}
	}

	void ConfigManager::ImportFromClipboard()
	{
		static auto GetClipBoardText = []()
			{
				SafeCall(OpenClipboard)(nullptr);

				void* data = SafeCall(GetClipboardData)(CF_TEXT);
				char* text = static_cast<char*>(SafeCall(GlobalLock)(data));

				std::string str_text(text);

				SafeCall(GlobalUnlock)(data);
				SafeCall(CloseClipboard)();

				return str_text;
			};

		static auto find_item = [](std::vector< C_ConfigItem* > items, std::string name) -> C_ConfigItem*
			{
				for (int i = 0; i < (int)items.size(); i++)
					if (!items[i]->Name.compare(name))
						return items[i];

				return nullptr;
			};

		if (GetClipBoardText().empty()) {
			return;
		}

		auto decoded_string = base64::decode(GetClipBoardText());
		if (decoded_string[0] != 't' ||
			decoded_string[1] != 'r' ||
			decoded_string[2] != 'a' ||
			decoded_string[3] != 'c' ||
			decoded_string[4] != 'k' ||
			decoded_string[5] != 'i' ||
			decoded_string[6] != 'n' ||
			decoded_string[7] != 'g')
			return;

		auto parsed_config = nlohmann::json::parse(decoded_string.erase(0, 8));

		nlohmann::json allJson = parsed_config;

		for (auto it = allJson.begin(); it != allJson.end(); ++it)
		{
			nlohmann::json j = *it;

			std::string name = j[("name")];
			std::string type = j[("type")];

			auto item = find_item(Items, name);

			if (item)
			{
				if (!type.compare(("int")))
					*(int*)item->Pointer = j[("value")].get<int>();
				else if (!type.compare(("float")))
					*(float*)item->Pointer = j[("value")].get<float>();
				else if (!type.compare(("bool")))
					*(bool*)item->Pointer = j[("value")].get<bool>();
			}
		}
	}

	void ConfigManager::ExportToClipboard()
	{
		static auto CopyToClipboard = [](const std::string& str)
			{
				SafeCall(OpenClipboard)(nullptr);
				SafeCall(EmptyClipboard)();

				void* hg = SafeCall(GlobalAlloc)(GMEM_MOVEABLE, str.size() + 1);

				if (!hg) {
					SafeCall(CloseClipboard)();
					return;
				}

				memcpy(SafeCall(GlobalLock)(hg), str.c_str(), str.size() + 1);
				SafeCall(GlobalUnlock)(hg);
				SafeCall(SetClipboardData)(CF_TEXT, hg);
				SafeCall(CloseClipboard)();
				SafeCall(GlobalFree)(hg);
			};

		nlohmann::json allJson;
		std::set<std::string> seenItems;

		for (auto it : Items)
		{
			if (seenItems.count(it->Name) > 0) {
				continue;
			}

			nlohmann::json j;

			j[("name")] = it->Name;
			j[("type")] = it->Type;

			if (!it->Type.compare(("int")))
				j[("value")] = (int)*(int*)it->Pointer;
			else if (!it->Type.compare(("float")))
				j[("value")] = (float)*(float*)it->Pointer;
			else if (!it->Type.compare(("bool")))
				j[("value")] = (bool)*(bool*)it->Pointer;

			allJson.push_back(j);
			seenItems.insert(it->Name);
		}

		auto str = base64::encode((std::string(("tracking")).append(allJson.dump(-1, '~'/*, true*/))).c_str());
		CopyToClipboard(str);
	}
}