#include <stdio.h>      /* DWM System Status String v1.0   sss  / / */
#include <stdlib.h>     /* From: github.com/ir33k/dwmsss     __/ /  */
#include <time.h>       /* Public domain (unlicense.org)  >~('__/   */

#define TMP	"/tmp/dwmsss"
#define BAT	"/sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/BAT0"
#define WLP     "/sys/class/net/wlp3s0"

/* Read file under PATH into BUF of SIZ-1 as string. */
void cat(const char *path, char *buf, int siz) {
	FILE *fp = fopen(path, "r");
	if (!fp) return;
	if ((siz = fread(buf, 1, siz-1, fp))) buf[siz] = 0;
	fclose(fp);
}

static char *wifi(void) {
	static char buf[6];
	cat(WLP"/operstate", buf, 2);
	sprintf(buf, "%s", *buf == 'u' ? "up" : "down");
	return buf;
}

static char *volume(void) {
	static char buf[8];
	system("amixer get Master | grep -om1 '[0-9]\\+%' >"TMP);
	cat(TMP, buf, 8);
	sprintf(buf, "%d", atoi(buf));
	return buf;
}

static char *battery(void) {
	static char buf[8];
	cat(BAT"/status", buf, 2);
	cat(BAT"/capacity", buf+1, 6);
	sprintf(buf, "%c%d", *buf == 'C' ? '+' : '-', atoi(buf+1));
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
