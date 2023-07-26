#include <stdio.h>      /* DWM System Status String v2.1   sss  / / */
#include <stdlib.h>     /* From: github.com/ir33k/dwmsss     __/ /  */
#include <time.h>       /* Public domain (unlicense.org)  >~('__/   */

/* Read first word from file under PATH into BUF or SIZ as string.
 * ASCII characters smaller than 48 ('0') are consider delimiters. */
void word(char *buf, int siz, const char *path) {
	FILE *fp = fopen(path, "r");
	if (!fp) return;
	while (--siz && (*buf = getc(fp)) > 47) buf += 1;
	*buf = 0;
	fclose(fp);
}

int main(void) {
	char vol[4], wifi[6], bat_s[2], bat_c[4], date[16];
	time_t now = time(0);

	system("amixer get Master | grep -om1 '[0-9]\\+%' > /tmp/dwmsss");
	word(vol,   4, "/tmp/dwmsss");
	word(wifi,  6, "/sys/class/net/wlp3s0/operstate");
	word(bat_s, 2, "/sys/class/power_supply/BAT0/status");
	word(bat_c, 4, "/sys/class/power_supply/BAT0/capacity");
	strftime(date, 16, "%m-%d %H:%M", localtime(&now));

	return printf(" %s@ %sv %c%sb | %s",
		wifi, vol, *bat_s == 'C' ? '+' : '-', bat_c, date) < 0;
}
