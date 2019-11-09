GETTER_VAR(void*, asm_base_haste_hook_ret1);
GETTER_VAR(void*, asm_base_haste_hook_ret2);

extern "C" float get_haste(cube::Creature * player) {
	return 1.0f + (0.00075f * (float)player->riding_speed);
}

__attribute__((naked)) void ASM_Base_haste_Hook() {
	asm(".intel_syntax \n"
		PUSH_ALL
		PREPARE_STACK
		"call get_haste \n"
		RESTORE_STACK
		POP_ALL
		"movss xmm8, xmm0 \n"
		"cmp byte ptr [rcx+0x14C], 1 \n"
		"jnz asm_base_haste_not_warriorlbl \n"
		"cmp byte ptr [rcx+0x14D], 0 \n"
		"jnz asm_base_haste_not_warriorlbl \n"
		DEREF_JMP(asm_base_haste_hook_ret2)

		"asm_base_haste_not_warriorlbl: \n"
		"movss xmm7, xmm8 \n"
		DEREF_JMP(asm_base_haste_hook_ret1)
		".att_syntax \n"
	);
}

void go_to_haste_cave() {
	char* base = (char*)CWBase();
	int offset = 0x6647E;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0x51);
	WriteByte(base + offset + 0x02, 0xFF);
	WriteByte(base + offset + 0x03, 0x54);
	WriteByte(base + offset + 0x04, 0x00);
	WriteByte(base + offset + 0x05, 0x90);
	WriteByte(base + offset + 0x06, 0x90);
	WriteByte(base + offset + 0x07, 0x90);
	WriteByte(base + offset + 0x08, 0x90);
}

void hook_base_haste() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B63D4), (void*)&ASM_Base_haste_Hook);
	asm_base_haste_hook_ret1 = (base + 0x664C0); //not warrior
	asm_base_haste_hook_ret2 = (base + 0x66496); //are warrior
}

void set_base_haste() {
	go_to_haste_cave();
	hook_base_haste();
}
/*
52D39 tests if mage for haste power from 65C70
5FAF0
*/