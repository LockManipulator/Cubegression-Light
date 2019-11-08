GETTER_VAR(void*, asm_base_resist_hook_ret1);
GETTER_VAR(void*, asm_base_resist_hook_ret2);

extern "C" float get_resist(cube::Creature * creature) {
	return (float)creature->lamp_diameter;
}

__attribute__((naked)) void ASM_Base_resist_Hook() {
	asm(".intel_syntax \n"
		"jnz ASM_Base_resist_jnzlbl \n"
		"xorps xmm6, xmm6 \n"
		DEREF_JMP(asm_base_resist_hook_ret1)

		"ASM_Base_resist_jnzlbl: \n"
		PUSH_ALL
		"movq rax, xmm0 \n" //save xmm0
		"push rax \n"
		PREPARE_STACK
		"call get_resist \n"
		RESTORE_STACK
		"movss xmm6, xmm0 \n"  //apply new base resist
		"pop rax \n"
		"movq xmm0, rax \n" //load xmm0 back
		POP_ALL
		DEREF_JMP(asm_base_resist_hook_ret2)
		".att_syntax \n"
	);
}

void go_to_resist_cave() {
	char* base = (char*)CWBase();
	int offset = 0x64DA9;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0x42);
	WriteByte(base + offset + 0x02, 0x16);
	WriteByte(base + offset + 0x03, 0x55);
	WriteByte(base + offset + 0x04, 0x00);
}

void hook_base_resist() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B63F0), (void*)&ASM_Base_resist_Hook);
	asm_base_resist_hook_ret1 = (base + 0x64DAE);
	asm_base_resist_hook_ret2 = (base + 0x64DE1);
}

void set_base_resist() {
	hook_base_resist();
	go_to_resist_cave();
}