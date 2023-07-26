#include <stdio.h>      /* DWM System Status String    sss   __/ /  */
#include <time.h>       /* v1.0 @irek Public domain       >~('__/   */

#define BAT0	"/sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/BAT0"

/* Read file under PATH into BUF of SIZ-1, return string length. */
static int cat(const char *path, char *buf, int siz) {
	FILE *fp = fopen(path, "r");
	if (!fp) return 0;
	if ((siz = fread(buf, 1, siz-1, fp))) buf[siz] = 0;
	return (fclose(fp) == -1) ? 0 : siz;
}

/* Run CMD reading stdout to BUF of SIZ-1, return string length. */
static int cmd(const char *cmd, char *buf, int siz) {
	FILE *fp = popen(cmd, "r");
	if (!fp) return 0;
	if ((siz = fread(buf, 1, siz-1, fp))) buf[siz] = 0;
	return (pclose(fp) == -1) ? 0 : siz;
}

static char *wifi(void) {
	static char buf[4] = "...";
	cmd("nmcli -g NAME con show --active", buf, 4);
	return buf;
}

static char *volume(void) {
	static char buf[8];
	int n = cmd("amixer get Master | grep -om1 '[0-9]\\+%'", buf, 8);
	buf[n-2] = 0;	/* Trim "%\n" */
	return buf;
}

static char *battery(void) {
	static char buf[8];
	int n;
	cat(BAT0"/status", buf, 2);
	buf[0] = buf[0] == 'C' ? '+' : '-';
	n = cat(BAT0"/capacity", buf+1, 6);
	buf[n] = 0;	/* Trim "\n" */
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
