void suffix_change() {
	char* base = (char*)CWBase();

	//change health suffix
	int offset = 0x27558F;
	WriteByte(base + offset, 0xEB);
	WriteByte(base + offset + 0x01, 0x49);

	//change armor
	offset = 0x2755C1;
	WriteByte(base + offset, 0xEB);
	WriteByte(base + offset + 0x01, 0x17);

	//change resistance
	offset = 0x27562E;
	WriteByte(base + offset, 0xEB);
	WriteByte(base + offset + 0x01, 0xAA);
}