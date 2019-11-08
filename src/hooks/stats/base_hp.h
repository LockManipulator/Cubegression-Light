GETTER_VAR(void*, asm_base_hp_hook_ret);

extern "C" float get_hp(cube::Creature* player) {
	float base_ret = 100.0f;
	if (player->entity_data.classType == 1 && player->entity_data.specialization == 1) base_ret = 125.0f;
	return base_ret + (10.0f * (float)player->climbing_speed);
}

__attribute__((naked)) void ASM_Base_hp_Hook() {
	asm(".intel_syntax \n"
		PUSH_ALL
		"movq rax, xmm0 \n" //save xmm0
		"push rax \n"
		PREPARE_STACK
		"call get_hp \n"
		RESTORE_STACK
		"movss xmm6, xmm0 \n"  //apply new base hp
		"pop rax \n"
		"movq xmm0, rax \n" //load xmm0 back
		POP_ALL
		DEREF_JMP(asm_base_hp_hook_ret)
		".att_syntax \n"
	);
}

void go_to_hp_cave() {
	char* base = (char*)CWBase();
	int offset = 0x5FAD5;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0xC2);
	WriteByte(base + offset + 0x02, 0x68);
	WriteByte(base + offset + 0x03, 0x55);
	WriteByte(base + offset + 0x04, 0x00);
	WriteByte(base + offset + 0x05, 0x90);
	WriteByte(base + offset + 0x06, 0x90);
	WriteByte(base + offset + 0x07, 0x90);
}

void hook_base_hp() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B639C), (void*)&ASM_Base_hp_Hook);
	asm_base_hp_hook_ret = (base + 0x5FAF0);
}

void set_base_hp() {
	go_to_hp_cave();
	hook_base_hp();
	}
/*
52D39 tests if mage for hp power from 65C70
5FAF0
*/