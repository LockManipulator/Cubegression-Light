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
	int offset = 0x27554F;
	WriteFarJMP(Offset(base, 0x5B6452), (void*)&regen_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0xFE);
	WriteByte(base + offset + 0x02, 0x0E);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
}

void change_damage_display() {
	char* base = (char*)CWBase();
	int offset = 0x2755DF;
	WriteFarJMP(Offset(base, 0x5B6444), (void*)&damage_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0x60);
	WriteByte(base + offset + 0x02, 0x0E);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
}

void change_armor_display() {
	char* base = (char*)CWBase();
	int offset = 0x2755BC;
	WriteFarJMP(Offset(base, 0x5B6436), (void*)&armor_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0x75);
	WriteByte(base + offset + 0x02, 0x0E);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
}

void change_resistance_display() {
	char* base = (char*)CWBase();
	int offset = 0x275629;
	WriteFarJMP(Offset(base, 0x5B6428), (void*)&resistance_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0xFA);
	WriteByte(base + offset + 0x02, 0x0D);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
}


void change_crit_display() {
	char* base = (char*)CWBase();
	int offset = 0x2755A3;
	WriteFarJMP(Offset(base, 0x5B641A), (void*)&crit_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0x72);
	WriteByte(base + offset + 0x02, 0x0E);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
}

void change_haste_display() {
	char* base = (char*)CWBase();
	int offset = 0x27560D;
	WriteFarJMP(Offset(base, 0x5B640C), (void*)&haste_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0xFA);
	WriteByte(base + offset + 0x02, 0x0D);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
}

void change_hp_display() {
	char* base = (char*)CWBase();
	int offset = 0x27558A;
	WriteFarJMP(Offset(base, 0x5B63FE), (void*)&hp_stat_hook);

	WriteByte(Offset(base, offset), 0xE9);
	WriteByte(base + offset + 0x01, 0x6F);
	WriteByte(base + offset + 0x02, 0x0E);
	WriteByte(base + offset + 0x03, 0x34);
	WriteByte(base + offset + 0x04, 0x00);
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