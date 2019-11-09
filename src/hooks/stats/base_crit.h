GETTER_VAR(void*, asm_base_crit_hook_ret1);
GETTER_VAR(void*, asm_base_crit_hook_ret2);

extern "C" float get_crit(cube::Creature * player) {
	return 0.00075f * (float)player->sailing_speed;
}

__attribute__((naked)) void ASM_Base_crit_Hook() {
	asm(".intel_syntax \n"
		"jnz ASM_Base_crit_jnzlbl \n"
		"addss xmm6, xmm0 \n"
		DEREF_JMP(asm_base_crit_hook_ret1)

		"ASM_Base_crit_jnzlbl: \n"
		PUSH_ALL
		"movq rax, xmm6 \n" //save xmm6
		"push rax \n"
		PREPARE_STACK
		"call get_crit \n" //put crit in xmm0
		RESTORE_STACK
		"pop rax \n"
		"movq xmm6, rax \n" //load xmm6 back
		"addss xmm6, xmm0 \n" //add crit to where it should be
		POP_ALL
		DEREF_JMP(asm_base_crit_hook_ret2)
		".att_syntax \n"
	);
}

void go_to_crit_cave() {
	char* base = (char*)CWBase();
	int offset = 0x50D65;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0x5C);
	WriteByte(base + offset + 0x02, 0x56);
	WriteByte(base + offset + 0x03, 0x56);
	WriteByte(base + offset + 0x04, 0x00);
	WriteByte(base + offset + 0x05, 0x90);
}

void hook_base_crit() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B63C6), (void*)&ASM_Base_crit_Hook);
	asm_base_crit_hook_ret1 = (base + 0x50D6B);
	asm_base_crit_hook_ret2 = (base + 0x50D85);
}

void set_base_crit() {
	go_to_crit_cave();
	hook_base_crit();
}
/*
52D39 tests if mage for crit power from 65C70
5FAF0
*/