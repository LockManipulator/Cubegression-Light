GETTER_VAR(void*, asm_base_dmg_hook_ret);

extern "C" float get_dmg(cube::Creature* creature) {
	if (creature->entity_data.hostility_type != 0) return 5.0f;
	return 5.0f + (float)creature->diving_skill;
}

__attribute__((naked)) void ASM_Base_Damage_Hook() {
	asm(".intel_syntax \n"
		PUSH_ALL
		"movq rax, xmm0 \n" //save xmm0
		"push rax \n"
		PREPARE_STACK
		"call get_dmg \n"
		RESTORE_STACK
		"movss xmm7, xmm0 \n"  //apply new base damage
		"pop rax \n"
		"movq xmm0, rax \n" //load xmm0 back
		POP_ALL
		DEREF_JMP(asm_base_dmg_hook_ret)
		".att_syntax \n"
	);
}

void go_to_damage_cave() {
	char* base = (char*)CWBase();
	int offset = 0x4FAA0;
	WriteByte(base + offset, 0xE9);
	WriteByte(base + offset + 0x01, 0x05);
	WriteByte(base + offset + 0x02, 0x69);
	WriteByte(base + offset + 0x03, 0x56);
	WriteByte(base + offset + 0x04, 0x00);
	WriteByte(base + offset + 0x05, 0x90);
	WriteByte(base + offset + 0x06, 0x90);
	WriteByte(base + offset + 0x07, 0x90);
}

void hook_base_damage() {
	char* base = (char*)CWBase();
	WriteFarJMP(Offset(base, 0x5B63AA), (void*)&ASM_Base_Damage_Hook);
	asm_base_dmg_hook_ret = (base + 0x4FAA8);
}

void set_base_damage() {
	hook_base_damage();
	go_to_damage_cave();
}
/*
Since Monk class has no weapons, base damage = actual damage so instead of
our damage calculation being (base_damage + max_hp) * 0.06, I changed it to
base_damage + (max_hp * 0.06) so that artifact boosts work properly (actually
adding +1 to overall damage).

So for our custom class framework we need to have a hook here at base damage,
let's call that "base damage", and at where we hooked as well in our custom
Monk class, let's call that "additional damage calculations". So people can
modify base damage as well as do additional damage calculations after the game 
does it's own calculations based on the currently equipped weapons. This would
make skill trees/xp systems easier as people can modify base damage, overall
damage, and if they want to modify damage by weapons they can just modify the
weapon modifiers.

So for our custom class framework we can use this file as the base damage hook
and just we (and others) can just call the functions here.

52D39 tests if mage for spell power from 65C70
*/