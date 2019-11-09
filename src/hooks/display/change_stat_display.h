extern "C" float regen_stat_hook() { // mod 4
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.00025f * player->hang_gliding_speed;
}

extern "C" float damage_stat_hook() { // mod 2
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.01f * (float)player->diving_skill;
}

extern "C" float armor_stat_hook() { // mod 1
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.01f * (float)player->swimming_speed;
}

extern "C" float resistance_stat_hook() { // mod 6
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.01f * (float)player->lamp_diameter;
}

extern "C" float crit_stat_hook() { // mod 5
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.00075f * (float)player->sailing_speed;
}

extern "C" float haste_stat_hook() { // mod 3
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.00075f * (float)player->riding_speed;
}

extern "C" float hp_stat_hook() { // mod 0
	cube::Creature* player = cube::GetGame()->GetPlayer();
	return 0.1f * (float)player->climbing_speed;
}


void change_regen_display() {
	char* base = (char*)CWBase();
	int offset = 0x5E411;
	WriteFarJMP(Offset(base, offset), (void*)&regen_stat_hook);
	WriteByte(Offset(base, offset+0x14), 0xC3);
}

void change_damage_display() {
	char* base = (char*)CWBase();
	int offset = 0x511C3;
	WriteFarJMP(Offset(base, offset), (void*)&damage_stat_hook);
	WriteByte(Offset(base, offset+0x14), 0xC3);
}

void change_armor_display() {
	char* base = (char*)CWBase();
	int offset = 0x66432;
	int offset2 = 0x66440;
	WriteFarJMP(Offset(base, offset), (void*)&armor_stat_hook);
	WriteByte(Offset(base, offset2), 0xC3);
}

void change_resistance_display() {
	char* base = (char*)CWBase();
	int offset = 0x5F721;
	WriteFarJMP(Offset(base, offset), (void*)&resistance_stat_hook);
	WriteByte(Offset(base, offset + 0x14), 0xC3);
}

void change_crit_display() {
	char* base = (char*)CWBase();
	int offset = 0x65371;
	WriteFarJMP(Offset(base, offset), (void*)&crit_stat_hook);
	WriteByte(Offset(base, offset + 0x14), 0xC3);
}

void change_haste_display() {
	char* base = (char*)CWBase();
	int offset = 0x65311;
	WriteFarJMP(Offset(base, offset), (void*)&haste_stat_hook);
	WriteByte(Offset(base, offset + 0x14), 0xC3);
}

void change_hp_display() {
	char* base = (char*)CWBase();
	int offset = 0x50BA2;
	int offset2 = 0x50BB0;
	WriteFarJMP(Offset(base, offset), (void*)&hp_stat_hook);
	WriteByte(Offset(base, offset2), 0xC3);
}

void remove_roundf_stat_display() {
	char* base = (char*)CWBase();
	int offset = 0x275559;
	for (int i = 0; i < 5; i++) {
		WriteByte(Offset(base, offset + i), 0x90);
	}
}

void change_stats() {
	change_regen_display();
	change_damage_display();
	change_armor_display();
	change_resistance_display();
	change_crit_display();
	change_haste_display();
	change_hp_display();

	remove_roundf_stat_display();
}