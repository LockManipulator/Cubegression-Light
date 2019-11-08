#include "main.h"

#include <fstream>

#include "memory/memory_helper.h"

//Hook stuff wheee
#include "hook/hooks/artifact/hook_set_artifact_stats.h"
#include "hook/hooks/artifact/display/hook_concat_artifact_suffix.h"
#include "hook/hooks/artifact/display/hook_round_artifact_thing.h"
#include "hook/hooks/artifact/display/hook_artifact_display_roundf.h"
#include "hook/hooks/artifact/base_stats/hook_get_hit_points.h"
#include "hook/hooks/artifact/base_stats/hook_get_player_armor.h"
#include "hook/hooks/artifact/base_stats/hook_get_attack_power.h"
#include "hook/hooks/artifact/base_stats/hook_get_spell_power.h"
#include "hook/hooks/artifact/base_stats/hook_get_player_haste.h"
#include "hook/hooks/artifact/base_stats/hook_get_player_regeneration.h"
#include "hook/hooks/artifact/base_stats/hook_get_player_resistance.h"
#include "hook/hooks/artifact/base_stats/hook_get_player_crit.h"


Main& Main::GetInstance()
{
	static auto instance = Main();
	return instance;
}

void Main::Start()
{
	//Very fun
	PatchRegionLock();
	VanillaGliderFix();
	VanillaRideFix();
	ChangeArtifactDisplay();
	//PatchBytes(auto location, int byte_array, int size);
	SetupHooks();
}


//TODO: Maybe don't use hardcoded offsets but it's not like the game will ever update anyway so it's fine
void Main::SetupHooks()
{
	//Used for updating networked artifact stats
	new HookSetArtifactStats();

	//Artifact display stuff
	new HookConcatArtifactSuffix();
	new HookRoundArtifactThing();
	new Hookroundf();

	//Artifact value stuff
	new HookGetMaxHitPoints();
	new HookGetPlayerArmor();
	new HookGetAttackPower();
	new HookGetSpellPower();
	new HookGetPlayerHaste();
	new HookGetPlayerRegeneration();
	new HookGetPlayerResistance();
	new HookGetPlayerCrit();
}

//TODO: You could replace these with hooks pretty easily, should probably do that
void Main::ChangeArtifactDisplay()
{
	//Item display
	MemoryHelper::FindAndReplaceString(L"Increases hang gliding speed.", L"Increases base regeneration.");
	MemoryHelper::FindAndReplaceString(L"You can climb faster.", L"Increases base health.");
	MemoryHelper::FindAndReplaceString(L"Increases sailing speed.", L"Increases base crit.");
	MemoryHelper::FindAndReplaceString(L"Increases swimming speed.", L"Increases base armor.");
	MemoryHelper::FindAndReplaceString(L"You consume less stamina when diving.", L"Increases base damage.");
	MemoryHelper::FindAndReplaceString(L"Increases riding speed.", L"Increases base haste.");
	MemoryHelper::FindAndReplaceString(L"Increases the radius of your lamp.", L"Increases base resistance.");

	//Inventory display
	MemoryHelper::FindAndReplaceString(L"Climbing Speed", L"Health Boost");
	MemoryHelper::FindAndReplaceString(L"Swimming Speed", L"Armor Boost");
	MemoryHelper::FindAndReplaceString(L"Diving Skill", L"Damage Boost");
	MemoryHelper::FindAndReplaceString(L"Riding Speed", L"Haste Boost");
	MemoryHelper::FindAndReplaceString(L"Hang Gliding Speed", L"Regen Boost");
	MemoryHelper::FindAndReplaceString(L"Sailing Speed", L"Crit Boost");
	MemoryHelper::FindAndReplaceString(L"Light Radius", L"Resistance Boost");
}

//By the Lich Group with help from Geo
void Main::PatchRegionLock()
{
	std::string option1true;
	std::string option1;
	std::string str;
	std::ifstream file("config.txt");
	option1true = "movement=true";

	if (file.is_open())
	{
		while (getline(file, str))
		{
			option1 = str;
		}
		file.close();
	}

	if (option1.compare(0, option1true.length(), option1true) == 0)
	{

		auto glider_use = MemoryHelper::FindPattern("0F 84 86 ? ? ? 49 8B 8E");
		auto boat_use = MemoryHelper::FindPattern("74 66 4D 8B 86 48 04");
		auto rein_use = MemoryHelper::FindPattern("74 14 49 8B 45 08");
		auto rein_stay = MemoryHelper::FindPattern("E8 D4 3B D8 FF 84 C0 75 0B 49 8B 86 48 04");
		auto boat_stay = MemoryHelper::FindPattern("E8 82 3B D8 FF 84 C0 75 0B 49 8B 86 48 04 ? ? C6 40 68");

		MemoryHelper::PatchMemory<uint8_t>(glider_use + 0x01, 0x80);
		MemoryHelper::PatchMemory<uint8_t>(boat_use, 0x70);
		MemoryHelper::PatchMemory<uint8_t>(rein_use, 0x70);
		MemoryHelper::PatchMemory<uint8_t>(rein_stay + 0x07, 0xEB);
		MemoryHelper::PatchMemory<uint8_t>(boat_stay + 0x07, 0xEB);

	}

	else
	{

	}
}

void Main::VanillaGliderFix()
{
	int glider_patch[] = { 0x49, 0x8B, 0xCD, 0xE8, 0x5A, 0x25, 0xDA, 0xFF, 0xF3, 0x0F, 0x10, 0xC8, 0x41, 0x0F, 0x28, 0xC7 };
	auto glider_find = MemoryHelper::FindPattern("F3 0F 5F C1 F3 0F 10 0D 06 0B 1F ? 0F 28 D1 F3");

	for (int i = 0; i < 16; i++)
	{
		MemoryHelper::PatchMemory<uint8_t>(glider_find + i, glider_patch[i]);
	}

}

void Main::VanillaRideFix()
{
	int ride_patch1[] = { 0x49, 0x8B, 0xCD, 0xE8, 0xDA, 0x94, 0xDA, 0xFF, 0x0F, 0x28, 0xF8 };
	int ride_patch2[] = { 0x0F, 0x1F, 0x40, 0x00, 0xF3, 0x44, 0x0F, 0x59, 0xCF };
	int ride_patch3[] = { 0xF3, 0x0F, 0x5C, 0xC1, 0xF3, 0x0F, 0x59, 0x05, 0xA3, 0xD5, 0x3C, 0x00, 0xF3, 0x0F, 0x58, 0x05, 0x17, 0x92, 0x3C, 0x00, 0xF3, 0x0F, 0x59, 0x05, 0x01, 0x00, 0x00, 0x00, 0xC3, 0x00, 0x00, 0xA0, 0x3F };

	auto ride_find1 = MemoryHelper::FindPattern("48 8B D3 49 8B CF E8 27 29 D5 FF");
	auto ride_find2 = MemoryHelper::FindPattern("F3 44 0F 59 0D 1A 27 17 ?");
	auto ride_find3 = MemoryHelper::FindPattern("F3 0F 5C C1 F3 0F 59 05 A3 D5 3C ? F3 0F 58 05 17 92 3C ? C3");

	for (int i = 0; i < 11; i++)
	{
		MemoryHelper::PatchMemory<uint8_t>(ride_find1 + i, ride_patch1[i]);
	}

	for (int i = 0; i < 9; i++)
	{
		MemoryHelper::PatchMemory<uint8_t>(ride_find2 + i, ride_patch2[i]);
	}

	for (int i = 0; i < 33; i++)
	{
		MemoryHelper::PatchMemory<uint8_t>(ride_find3 + i, ride_patch3[i]);
	}
}

Player* Main::GetLocalPlayer()
{
	auto game_base = *(uint64_t*)(MemoryHelper::GetCubeBase() + 0x551A80);
	return *(Player**)(*(uint64_t*)(game_base + 0x8) + 0x448);
}

void Main::SetConsole(bool open)
{
	if (open)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
	}
	else
	{
		fclose(stdout);
		FreeConsole();
	}
}
#define CONFIG_PATH "modcfg.txt"

//probably not needed?
static std::vector<std::string> Split(std::string s, std::string delimiter)
{
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
	{
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}