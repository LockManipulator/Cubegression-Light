GETTER_VAR(void*, asm_base_spell_hook_ret);

extern "C" float get_spell(cube::Creature* creature) {
	if (creature->entity_data.hostility_type != 0) return 5.0f;
	return 5.0f + (float)creature->diving_skill;
}
__attribute__((naked)) void ASM_Base_spell_Hook() {
	asm(".intel_syntax \n"
		PUSH_ALL
		"movq rax, xmm0 \n" //save xmm0
		"push rax \n"
		PREPARE_STACK
		"call get_spell \n"
		RESTORE_STACK
		"movss xmm6, xmm0 \n"  //apply new base damage
		"pop rax \n"
		"movq xmm0, rax \n" //load xmm0 back
		POP_ALL
		DEREF_JMP(asm_base_spell_hook_ret)
		".att_syntax \n"
	);
}

void go_to_spell_cave() {
	char* base = (char*)CWBase();
	int offset = 0x65C99;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0xF0);
	WriteByte(base + offset + 0x02, 0x06);
	WriteByte(base + offset + 0x03, 0x55);
	WriteByte(base + offset + 0x04, 0x00);
	WriteByte(base + offset + 0x05, 0x90);
	WriteByte(base + offset + 0x06, 0x90);
	WriteByte(base + offset + 0x07, 0x90);
}

void hook_base_spell() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B638E), (void*)&ASM_Base_spell_Hook);
	asm_base_spell_hook_ret = (base + 0x65CA1);
}

void set_base_spell() {
	hook_base_spell();
	go_to_spell_cave();
}
/*
52D39 tests if mage for spell power from 65C70
*/