#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

static FILE *input;
static FILE *output;

static void format_error(void)
{
	fprintf(stderr, "input format error\n");
	abort();
}

static char *get_line(void)
{
	char buf[2048];
	if (!fgets(buf, 2048, input))
		return NULL;
	return strdup(buf);
}

static int nth;

static void parse_input(void)
{
	char *ln, *s;

	/* nth */
	if (!(ln = get_line())) format_error();
	nth = strtoul(ln, NULL, 0);
}

static bool is_prime(int n)
{
	int d;

	if (n < 3) { return n == 2; }

	for (d=2; d<n; d++) {
		if (n % d == 0)
			return false;
	}

	return true;
}

static int hsum(int n)
{
	int i, s;

	s = 0;
	for (i=0; i<8*sizeof(n); i++) {
		s += (n & 1);
		n >>= 1;
	}

	return s;
}

int main(int argc, char *argv[])
{
	int n;

	input = stdin;
	output = stdout;

	if (argc > 1)
		input = fopen(argv[1], "r");
	if (argc > 2)
		output = fopen(argv[2], "w");

	if (!input || !output) {
		fprintf(stderr, "could not open files\n");
		abort();
	}

	if (input == stdin || output == stdout)
		fprintf(stderr, "WARNING: using debug mode behavior\n");

	parse_input();

	for (n=0; n<1000000000; n++) {
		if (is_prime(n) && is_prime(hsum(n))) {
			if (nth == 1) {
				fprintf(output, "%d\n", n);
				exit(EXIT_SUCCESS);
			}
			nth--;
		}
	}
}
