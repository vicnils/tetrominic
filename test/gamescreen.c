#include "terminal.h"
#include "textgfx.h"

void panels_text(const char *s, int w, int x, int y, int attr)
{
	set_text_attr(attr);
	while (*(s += centre_text(s, w, x, y)) == '\n') {
		s++;
		y += 4;
	}
}

int main(int argc, char **argv)
{
	init_terminal();
	gettermtype();
	init_acs_caps();
	terminal.acs = ACS_VT100;

	const char *names[8] = {0};
	names[ACS_ASCII]   = "ascii";
	names[ACS_VT100]   = "vt100";
	names[ACS_IBM]     = "ibm";
	names[ACS_UNICODE] = "unicode";

	int i;
	if (argc > 1) {
		for (i=0; i<8; i++) {
			if (names[i] && !strcmp(argv[1], names[i]))
				terminal.acs = i;
		}
		if (terminal.acs == ACS_UNICODE)
			terminal.charenc = CHAR_UTF8;
	}

	printf("Line drawing: %s\n\n", names[terminal.acs]);

	set_acs_table();
	textgfx_init();

	struct tg_buffer line = {0};

	set_text_attr(FG_COLOR(4));
	line.x = 10;
	drawacs("x19", 1, &line);
	line.x = 31;
	line.y = 0;
	drawacs("x19", 1, &line);

	line.x = 0;
	line.y = 0;
	set_text_attr(BG_COLOR(4) | FG_COLOR(4) | BOLD);
	drawacs("lq7u(2(2xh7x)tq7u)xh7x", 10, &line);

	set_text_attr(FG_COLOR(4) | BOLD);
	drawacs("(2x 7x)mq7u 8", 10, &line);
	drawacs("x1", 1, &line);
	line.x = 32;
	line.y = 0;
	drawacs("x19", 1, &line);

	panels_text("Score\nLevel\nLines\nNext",
		    8, 1, 1, BG_COLOR(4) | FG_COLOR(3) | BOLD);

	panels_text("122704\n09\n044",
		    9, 1, 2, BG_COLOR(0) | FG_COLOR(7));

	struct tg_tiles tiles = {
		{' ', '<', '%', '#', '[', '(', '@', '{',
		 ' ', '>', '%', '#', ']', ')', '@', '}'}
	};
	for (i=0; i<8; i++) {
		tiles.attrs[i]   = BG_COLOR(i) | FG_COLOR(i) | BOLD;
		tiles.attrs[i+8] = BG_COLOR(i) | FG_COLOR(i) | BOLD;
	}
	unsigned char preview_t[] = {
		0x03, 0xB3, 0xB3, 0xB0,
		0x00, 0x03, 0xB0, 0x00
	};
	drawtiles(NULL, preview_t, 4, 2, 1, 15, &tiles);

	unsigned char playfield[] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x3B, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00,
		0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x7F, 0x7F, 0x7F, 0x00, 0x6E,
		0x00, 0x00, 0x00, 0x2A, 0x2A, 0x2A, 0x7F, 0x7F, 0x6E, 0x6E,
		0x00, 0x00, 0x00, 0x5D, 0x5D, 0x2A, 0x6E, 0x19, 0x6E, 0x3B,
		0x00, 0x19, 0x4C, 0x4C, 0x5D, 0x6E, 0x6E, 0x19, 0x3B, 0x3B,
		0x00, 0x19, 0x4C, 0x4C, 0x5D, 0x6E, 0x5D, 0x19, 0x2A, 0x3B,
		0x00, 0x19, 0x4C, 0x4C, 0x5D, 0x5D, 0x5D, 0x19, 0x2A, 0x2A
	};
	drawtiles(NULL, playfield, 10, 20, 11, 0, &tiles);

	textgfx_end();
	return 0;
}
