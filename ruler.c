#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__
#define HAS_IOCTL
#endif

#ifdef HAS_IOCTL
#include <sys/ioctl.h>
#define USAGE_STRING "Usage: %s [columns]\n"
#else
#define USAGE_STRING "Usage: %s columns\n"
#endif

int main(int argc, char *argv[])
{
	int cols;
	int arg_error = 0;

	if (argc == 1) {
#ifdef HAS_IOCTL
		struct winsize ws;
		if (ioctl(1, TIOCGWINSZ, &ws) == -1)
		{
			fprintf(stderr, "%s: error getting terminal size\n", argv[0]);
			return 1;
		}
		cols = ws.ws_col;
#else
		arg_error = 1;
#endif
	} else if (argc == 2) {
		cols = atoi(argv[1]);
		if (cols == 0) {
			fprintf(stderr, "Argument must be a number 1 or greater.\n");
			arg_error = 1;
		}
	} else {
		arg_error = 1;
	}

	if (arg_error) {
		printf(USAGE_STRING, argv[0]);
		return 2;
	}

	int digits = snprintf(NULL, 0, "%u", cols-1);

	int d; for (d=digits-1; d>=0; --d) {
		int x; for (x=0; x<cols; ++x) {
			int n = x;
			int i; for (i=0; i<d; ++i) {
				n /= 10;
			}
			putchar('0'+n%10);
		}
		putchar('\n');
	}

	return 0;
}
