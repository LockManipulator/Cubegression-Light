GETTER_VAR(void*, asm_base_armor_hook_ret1);
GETTER_VAR(void*, asm_base_armor_hook_ret2);

extern "C" float get_armor(cube::Creature* player) {
	return 0.0f + (float)player->swimming_speed;
}

__attribute__((naked)) void ASM_Base_armor_Hook() {
	asm(".intel_syntax \n"
		"jnz ASM_Base_armor_jnzlbl \n"
		"xorps xmm6, xmm6 \n"
		DEREF_JMP(asm_base_armor_hook_ret1)

		"ASM_Base_armor_jnzlbl: \n"
		PUSH_ALL
		"movq rax, xmm0 \n" //save xmm0
		"push rax \n"
		PREPARE_STACK
		"call get_armor \n"
		RESTORE_STACK
		"movss xmm6, xmm0 \n"  //apply new base armor
		"pop rax \n"
		"movq xmm0, rax \n" //load xmm0 back
		POP_ALL
		DEREF_JMP(asm_base_armor_hook_ret2)
		".att_syntax \n"
	);
}

void go_to_armor_cave() {
	char* base = (char*)CWBase();
	int offset = 0x4F099;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0x1A);
	WriteByte(base + offset + 0x02, 0x73);
	WriteByte(base + offset + 0x03, 0x56);
	WriteByte(base + offset + 0x04, 0x00);
}

void hook_base_armor() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B63B8), (void*)&ASM_Base_armor_Hook);
	asm_base_armor_hook_ret1 = (base + 0x4F09E);
	asm_base_armor_hook_ret2 = (base + 0x4F0D1);
}

void set_base_armor() {
	go_to_armor_cave();
	hook_base_armor();
}
/*
52D39 tests if mage for armor power from 65C70
5FAF0
*/