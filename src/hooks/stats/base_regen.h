GETTER_VAR(void*, asm_base_regen_hook_ret);

extern "C" float get_regen(cube::Creature * creature) {
	return 1.0f + (0.00025f * (float)creature->hang_gliding_speed);
}

__attribute__((naked)) void ASM_Base_regen_Hook() {
	asm(".intel_syntax \n"
		PUSH_ALL
		"movq rax, xmm0 \n" //save xmm0
		"push rax \n"
		PREPARE_STACK
		"call get_regen \n"
		RESTORE_STACK
		"movss xmm6, xmm0 \n"  //apply new base regen
		"pop rax \n"
		"movq xmm0, rax \n" //load xmm0 back
		POP_ALL
		DEREF_JMP(asm_base_regen_hook_ret)
		".att_syntax \n"
	);
}

void go_to_regen_cave() {
	char* base = (char*)CWBase();
	int offset = 0x647BE;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0x1F);
	WriteByte(base + offset + 0x02, 0x1C);
	WriteByte(base + offset + 0x03, 0x55);
	WriteByte(base + offset + 0x04, 0x00);
	WriteByte(base + offset + 0x05, 0x90);
	WriteByte(base + offset + 0x06, 0x90);
	WriteByte(base + offset + 0x07, 0x90);
}

void hook_base_regen() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B63E2), (void*)&ASM_Base_regen_Hook);
	asm_base_regen_hook_ret = (base + 0x647C6);
}

void set_base_regen() {
	hook_base_regen();
	go_to_regen_cave();
}