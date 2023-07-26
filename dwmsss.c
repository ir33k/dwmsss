#include <stdio.h>      /* DWM System Status String v2.0   sss  / / */
#include <stdlib.h>     /* From: github.com/ir33k/dwmsss     __/ /  */
#include <time.h>       /* Public domain (unlicense.org)  >~('__/   */

/* Read first word from file under PATH into BUF or SIZ as string.
 * ASCII characters smaller than 48 ('0') are consider delimiters.
 * Return pointer after null terminator. */
char *word(char *buf, int siz, const char *path) {
	FILE *fp = fopen(path, "r");
	if (!fp) return buf;
	while (--siz && (*buf = getc(fp)) > 47) buf += 1;
	*buf = 0;
	fclose(fp);
	return buf+1;
}

int main(void) {
	char buf[64], *bp = buf, *wifi, *vol, *bat_s, *bat_c, *date;
	time_t now = time(0);

	system("amixer get Master | grep -om1 '[0-9]\\+%' > /tmp/dwmsss");
	bp = word(vol   = bp, 8, "/tmp/dwmsss");
	bp = word(wifi  = bp, 8, "/sys/class/net/wlp3s0/operstate");
	bp = word(bat_s = bp, 8, "/sys/class/power_supply/BAT0/status");
	bp = word(bat_c = bp, 8, "/sys/class/power_supply/BAT0/capacity");
	strftime(date = bp, 16, "%m-%d %H:%M", localtime(&now));

	return printf(" %s@ %sv %c%sb | %s",
		wifi, vol, *bat_s == 'C' ? '+' : '-', bat_c, date) < 0;
}
