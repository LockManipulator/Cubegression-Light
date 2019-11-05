#pragma once

#include <cstdint>
#include <sstream>
#include <random>
#include <functional>
#include "main.h"
#include <climits>


struct RegionCoordinates
{
	int x = INT_MIN, y = INT_MIN;

	
	  //Returns the absolute distance between two region coordinates
	 // @param coordinates
	 // @return
	 
	float operator-(RegionCoordinates coordinates)
	{
		float x_diff = abs(x - coordinates.x);
		float y_diff = abs(y - coordinates.y);
		return sqrt(x_diff * x_diff + y_diff * y_diff);
		return sqrt(x_diff * x_diff + y_diff * y_diff);
	}
};

struct Item
{
	uint8_t GetItemType() //TODO: Make this an enum please
	{
		return *(uint8_t*)this;
	}

	bool IsEquipment()
	{
		return GetItemType() >= 3 && GetItemType() <= 9;
	}

	RegionCoordinates GetRegionCoordinates()
	{
		return *(RegionCoordinates*)(this + 0xC);
	}

	void SetRegionCoordinates(RegionCoordinates coordinates)
	{
		*(RegionCoordinates*)(this + 0xC) = coordinates;
	}

	uint32_t GetModifier()
	{
		return *(uint32_t*)(this + 0x8);
	}

	void SetModifier(uint32_t modifier)
	{
		*(uint32_t*)(this + 0x8) = modifier;
	}	
};

enum EntityType
{
	PLAYER, HOSTILE, PASSIVE, NPC_0, NPC_1, UNK, PLAYER_PET, OBJECT
};

struct Entity
{
	EntityType GetEntityType()
	{
		return (EntityType)(*(uint8_t*)(this + 0x60));
	}

	RegionCoordinates GetRegionCoordinates()
	{
		return *(RegionCoordinates*)(this + 0x150);
	}
};

enum ArtifactType
{
	HEALTH, ARMOR, DAMAGE, HASTE, REGENERATION, CRIT, RESISTANCE
};

struct Player : public Entity
{
	std::string GetName()
	{
		return std::string((char*)this + 0x958);
	}

	/**
	 * Artifact stats are stored in the players' wing positions because they're networked and players don't have wings and this is easier
	 * @param artifact_type
	 * @return
	 */
	uint16_t GetNetworkedArtifactStats(ArtifactType artifact_type)
	{
		switch (artifact_type)
		{
		case HEALTH:
			return *(uint16_t*)(this + 0x11C);

		case ARMOR:
			return *(uint16_t*)(this + 0x11E);

		case RESISTANCE:
			return *(uint16_t*)(this + 0x120);

		default:
			//Some stuff doesn't need to be networked because the stats are calculated only by the local player (like damage)
			return 0;
		}
	}

	void UpdateNetworkedArtifactStats()
	{
		//Only 3 that need to be networked really
		*(uint16_t*)(this + 0x11C) = GetArtifactStats(HEALTH);
		*(uint16_t*)(this + 0x11E) = GetArtifactStats(ARMOR);
		*(uint16_t*)(this + 0x120) = GetArtifactStats(RESISTANCE);
	}

	int GetArtifactStats(ArtifactType artifact_type)
	{
		return *(int*)(this + 0xB5C + (4 * artifact_type));
	}

	void SetArtifactStats(ArtifactType artifact_type, int value)
	{
		*(int*)(this + 0xB5C + (4 * artifact_type)) = value;
	}

	//The values that the artifact base stats start at
	float GetArtifactTypeBaseStats(ArtifactType artifact_type)
	{
		switch (artifact_type)
		{
		case HEALTH:
			return 0.f;

		case ARMOR:
			return 0.f;

		case DAMAGE:
			return 0.f;

		case HASTE:
			return 0.f;

		case REGENERATION:
			return 0.f;

		case CRIT:
			return 0.f;

		case RESISTANCE:
			return 0.f;
		}

		return 0.f;
	}

	float GetIncreasedArtifactStats(ArtifactType artifact_type, bool for_display = false)
	{
		//Use the networked stats if we can, if we can't it's either actually 0 or it's not a networked stat
		auto artifact_amount = GetNetworkedArtifactStats(artifact_type);
		if (artifact_amount == 0) artifact_amount = GetArtifactStats(artifact_type);

		float stat_increase;
		switch (artifact_type)
		{
		case HEALTH:
			stat_increase = artifact_amount * 10.f;
			break;

		case ARMOR:
			stat_increase = artifact_amount * 1.0f;
			break;

		case DAMAGE:
			stat_increase = artifact_amount * 1.0f;
			break;

		case HASTE:
			stat_increase = artifact_amount * 0.075f;
			break;

		case REGENERATION:
			stat_increase = artifact_amount * 0.1f;
			break;

		case CRIT:
			stat_increase = artifact_amount * 0.075f;
			break;

		case RESISTANCE:
			stat_increase = artifact_amount * 1.0f;
			break;
		}

		if (for_display) stat_increase += GetArtifactTypeBaseStats(artifact_type);
		return stat_increase;
	}
};