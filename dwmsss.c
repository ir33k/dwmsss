#include <stdio.h>      /* DWM System Status String v2.3   sss  / / */
#include <stdlib.h>     /* From: github.com/ir33k/dwmsss     __/ /  */
#include <time.h>       /* Public domain (unlicense.org)  >~('__/   */

/* Read first word from file under PATH into BUF or SIZ as string.
 * ASCII characters smaller than 48 ('0') are consider delimiters. */
void word(char *buf, int siz, const char *path) {
	FILE *fp = fopen(path, "r");
	if (!fp) return;
	for (; --siz; buf += 1) if ((*buf = getc(fp)) < 48) break;
	*buf = 0;
	fclose(fp);
}

static char *wifi(void) {
	static char buf[8];
	word(buf, 8, "/sys/class/net/wlp3s0/operstate");
	return buf;
}

static char *volume(void) {
	static char buf[8];
	system("amixer get Master | grep -om1 '[0-9]\\+%' > /tmp/dwmsss");
	word(buf, 8, "/tmp/dwmsss");
	return buf;
}

static char *battery(void) {
	static char buf[8];
	word(buf,   2, "/sys/class/power_supply/BAT0/status");
	word(buf+1, 6, "/sys/class/power_supply/BAT0/capacity");
	*buf = *buf == 'C' ? '+' : '-'; /* Convert status to + or - */
	return buf;
}

static char *date(void) {
	static char buf[16];
	time_t now = time(0);
	strftime(buf, 16, "%m-%d %H:%M", localtime(&now));
	return buf;
}

int main(void) {
	return printf(" %s@ %sv %sb | %s",
		wifi(), volume(), battery(), date()) < 0;
}
