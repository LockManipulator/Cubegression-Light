#include "main.h"
#include "cwmods/cwsdk.h"
#include <wchar.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <windows.h>

//#include "src/hooks/stats/base_damage.h"
//#include "src/hooks/stats/base_spell.h"
//#include "src/hooks/stats/base_hp.h"
//#include "src/hooks/stats/base_armor.h"
//#include "src/hooks/stats/base_crit.h"
//#include "src/hooks/stats/base_haste.h"
//#include "src/hooks/stats/base_regen.h"
//#include "src/hooks/stats/base_resist.h"

#include "src/hooks/display/change_stat_display.h"
#include "src/hooks/display/change_stat_suffix.h"

#include "src/memory/memory_helper.h"

void* Offset(void* x1, uint64_t x2) {
	return (void*)((char*)x1 + x2);
}

void Popup(const char* title, const char* msg) {
	MessageBoxA(0, msg, title, MB_OK | MB_ICONINFORMATION);
}

class Mod : GenericMod {
	bool init = false;

	void ChangeArtifactDisplay(){
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

	virtual void OnCreatureAttackPowerCalculated(cube::Creature* creature, float* power) {
		if (cube::GetGame()->world->local_creature == creature) {
			*power = *power + (float)creature->diving_skill;
		}
	}

	virtual void OnCreatureSpellPowerCalculated(cube::Creature* creature, float* power) {
		if (cube::GetGame()->world->local_creature == creature) {
			*power = *power + (float)creature->diving_skill;
		}
	}

	virtual void OnCreatureHPCalculated(cube::Creature* creature, float* hp) {
		cube::Game* game = cube::GetGame();
		if (!game) return;
		cube::World* world = game->world;
		if (!world) return;
		if (cube::GetGame()->world->local_creature == creature) {
			*hp = *hp + (10.0f * (float)creature->climbing_speed);
		}
	}

	virtual void OnCreatureArmorCalculated(cube::Creature* creature, float* armor) {
		if (cube::GetGame()->world->local_creature == creature) {
			*armor = *armor + (float)creature->swimming_speed;
		}
	}

	virtual void OnCreatureCriticalCalculated(cube::Creature* creature, float* critical) {
		if (cube::GetGame()->world->local_creature == creature) {
			*critical = *critical + (0.00075f * (float)creature->sailing_speed);
		}
	}

	virtual void OnCreatureHasteCalculated(cube::Creature* creature, float* power) {
		if (cube::GetGame()->world->local_creature == creature) {
			*power = *power + (0.00075f * (float)creature->riding_speed);
		}
	}

	virtual void OnCreatureRegenerationCalculated(cube::Creature* creature, float* regeneration) {
		if (cube::GetGame()->world->local_creature == creature) {
			*regeneration = *regeneration + (0.00025f * (float)creature->hang_gliding_speed);
		}
	}

	virtual void OnCreatureResistanceCalculated(cube::Creature* creature, float* resistance) {
		if (cube::GetGame()->world->local_creature == creature) {
			*resistance = *resistance + (float)creature->lamp_diameter;
		}
	}

	virtual void Initialize() override {
		ChangeArtifactDisplay();
		suffix_change();
		change_stats();

		//set_base_damage();
		//set_base_spell();
		//set_base_hp();
		//set_base_armor();
		//set_base_crit();
		//set_base_haste();
		//set_base_regen();
		//set_base_resist();
	}

	virtual void OnGameTick(cube::Game* game) override {
		if (!init) {
			init = true;

		}
	}

};

EXPORT Mod* MakeMod() {
	return new Mod();
}

/*
To do:

1. Function to get all the artifacts in the inventory for each boost
2. Change name of each boost
3. Modify each base stat based off number of artifacts of that type in inventory
4. 

Code caves used:
50301
1694B0
1694C2
50CB1
5F01F

Next hook: 5B6460, 5B646E, 5B647C, 5B648A, 5B6498, 5B64A6, 5B64B4

*/