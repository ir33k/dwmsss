#include <stdio.h>      /* DWMSSS - DWM System Status String            |  | */
#include <stdlib.h>     /* My super stupid simple dwm status     Sss    /  / */
#include <time.h>       /* v1.0 from github.com/ir33k/dwmsss       ____/  /  */
#include <fcntl.h>      /* Public domain (www.unlicense.org)      / o    /   */
#include <unistd.h>     /* snake, snake, snake, snake, snake   >--\_____/    */

#define BAT0	"/sys/bus/acpi/drivers/battery/PNP0C0A:00/power_supply/BAT0"

/* Read content of file under PATH into BUF of SIZ-1.
 * Terminate with NULL and return string length. */
static int cat(char *path, char *buf, int siz) {
	int fd = open(path, O_RDONLY);
	if (fd < 0) return 0;
	if ((siz = read(fd, buf, siz-1))) buf[siz] = 0;
	if (close(fd) == -1) return 0;
	return siz;
}

/* Execute CMD command writing stdout to BUF of SIZ-1.
 * Terminate with NULL and return string length. */
static int cmd(const char *cmd, char *buf, int siz) {
	FILE *fp = popen(cmd, "r");
	if (!fp) return 0;
	if ((siz = fread(buf, 1, siz-1, fp))) buf[siz] = 0;
	if (pclose(fp) == -1) return 0;
	return siz;
}

static char *wifi(void) {
	static char buf[4] = "...";
	cmd("nmcli -g NAME con show --active", buf, 4);
	return buf;
}

static char *volume(void) {
	static char buf[8];
	int n = cmd("amixer get Master | grep -Eom1 '[0-9]{1,3}%'", buf, 8);
	buf[n-2] = 0;	/* Trim right */
	return buf;
}

static char *battery(void) {
	static char buf[8];
	char capacity;
	cat(BAT0"/capacity", buf, 4);
	capacity = atoi(buf);
	cat(BAT0"/status", buf, 2);
	if (buf[0] != 'C') capacity *= -1;	/* Not charging */
	snprintf(buf, 8, "%+d", capacity);
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
