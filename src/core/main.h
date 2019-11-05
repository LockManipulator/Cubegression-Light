#pragma once

#include <map>
#include <Windows.h>
#include <string>
#include <vector>
#include <intrin.h>


class Player;

class Main
{

public:
	HMODULE module;

private:
	void SetConsole(bool open);

	void SetupHooks();

	void ChangeArtifactDisplay();

	void PatchRegionLock();

	void VanillaGliderFix();

	void VanillaRideFix();

public:
	static Main& GetInstance();

	void Start();

	Player* GetLocalPlayer();
};




































