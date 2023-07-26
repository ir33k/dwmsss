#include <stdio.h>      /* DWM System Status String v1.0   sss  / / */
#include <stdlib.h>     /* From: github.com/ir33k/dwmsss     __/ /  */
#include <time.h>       /* Public domain (unlicense.org)  >~('__/   */

#define TMP	"/tmp/dwmsss"
#define BAT	"/sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/BAT0"
#define WLP     "/sys/class/net/wlp3s0"

/* Read first word from file under PATH into BUF or SIZ as string.
 * ASCII characters smaller than 48 ('0') are consider delimiters. */
void word(const char *path, char *buf, int siz) {
	FILE *fp = fopen(path, "r");
	if (!fp) return;
	while (--siz && (*buf = getc(fp)) > 47) buf += 1;
	*buf = 0;
	fclose(fp);
}

static char *wifi(void) {
	static char buf[8];
	word(WLP"/operstate", buf, 8);
	return buf;
}

static char *volume(void) {
	static char buf[8];
	system("amixer get Master | grep -om1 '[0-9]\\+%' >"TMP);
	word(TMP, buf, 8);
	return buf;
}

static char *battery(void) {
	static char buf[8];
	word(BAT"/status", buf, 2);
	word(BAT"/capacity", buf+1, 6);
	buf[0] = buf[0] == 'C' ? '+' : '-';
	return buf;
}

static char *date(void) {
	static char buf[32];
	time_t now = time(0);
	strftime(buf, 32, "%m-%d %H:%M", localtime(&now));
	return buf;
}

int main(void) {
	return printf(" %s@ %sv %sb | %s",
		wifi(), volume(), battery(), date()) < 0;
}
