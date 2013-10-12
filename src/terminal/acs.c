/* VT100-style line drawing */

#include <string.h>
#include "terminal.h"

#define ACS_BUF_SIZE 20

void init_acs_caps()
{
	if (terminal.type == LINUX_TERM)
		terminal.acs_disabled = "oprs";
	else if (terminal.type & XTERM)
		terminal.acs_disabled = "hoprs+,-.0";

	if (!strcmp(terminal.wm_class[1], "XTerm"))
		terminal.acs_disabled = "h+,-.0";
}

static void set_acs_vt100()
{
	const char *disabled = terminal.acs_disabled;
	char ascii[32];
	set_char_table(acs_vt100, acs_ascii);
	convert_chars(disabled, ascii, sizeof(ascii));
	set_char_table(acs_vt100, acs_vt100);
	set_char_table(disabled, ascii);
}

void set_acs_table()
{
	switch (terminal.acs) {
	case ACS_ASCII:
		set_char_table(acs_vt100, acs_ascii);
		break;
	case ACS_VT100:
		set_acs_vt100();
		break;
	case ACS_IBM:
		set_char_table(acs_vt100, acs_cp437);
		if (terminal.charenc == CHAR_CP850)
			set_char_table("yz{", "\xA8\xA8\xA8");
		replace_codepage_c0();
		break;
	case ACS_UNICODE:
		set_acs_unicode_table(acs_vt100, acs_unicode);
	}
}

/* really switch to ACS */
static void putacs_vt100(const char *s, char *v, int n)
{
	char a[ACS_BUF_SIZE] = "";
	int i;

	/* separate ascii from vt100 codes */
	for (i=0; i<n; i++) {
		if (s[i] != v[i]) {
			a[i] = v[i];
			v[i] = '\0';
		}
	}

	for (i=0; i<n; ) {
		if (v[i]) {
			tputstr("\033(0");
			tputstr(v+i);
			tputstr("\033(B");
			i += strlen(v+i);
		}
		if (a[i]) {
			tputstr(a+i);
			i += strlen(a+i);
		}
	}
}

void tputacs(const char *s)
{
	char buf[ACS_BUF_SIZE];
	int n;

	if (terminal.acs == ACS_UNICODE) {
		for (; *s; s++)
			tputchar(acs_to_unicode(*s));
	} else {
		for (; *s; s += n) {
			n = convert_chars(s, buf, sizeof(buf));
			if (terminal.acs == ACS_VT100)
				putacs_vt100(s, buf, n);
			else
				tputstr(buf);
		}
	}
}
