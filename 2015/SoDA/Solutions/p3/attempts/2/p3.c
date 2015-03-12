#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

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

static int *arena, *neighbors;
static int pitch, height;
static int iterations;

static void parse_pair(char *s, char open, char close, int *a, int *b)
{
	char *as, *bs;

	while (*s && *s++ != open); /* first character after open */
	as = s;

	while (*s && *s++ != ','); /* first character after ',' */
	bs = s;

	*a = strtoul(as, NULL, 0);
	*b = strtoul(bs, NULL, 0);
}

static void parse_input(void)
{
	char *ln, *s;
	int i, n, x, y;

	/* dimensions */
	if (!(ln = get_line())) format_error();
	parse_pair(ln, '[', ']', &pitch, &height);
	arena = calloc(pitch * height, sizeof(int));
	neighbors = calloc(pitch * height, sizeof(int));

	/* number of pairs */
	if (!(ln = get_line())) format_error();
	n = strtoul(ln, NULL, 0);

	/* each pair */
	for (i=0; i<n; i++) {
		if (!(ln = get_line())) format_error();
		parse_pair(ln, '(', ')', &x, &y);
		arena[(height-y-1)*pitch + x] = 1;
	}

	/* desired number of iterations  */
	if (!(ln = get_line())) format_error();
	iterations = strtoul(ln, NULL, 0);
}

static int at(int x, int y)
{
	x = (x + pitch) % pitch;
	y = (y + height) % height;
	return arena[y*pitch + x];
}

static void run_game(void)
{
	int iter, x, y, alive, neigh;

	for (iter=0; iter<iterations; iter++) {
		/* count neighbors */
		for (y=0; y<height; y++) {
			for (x=0; x<pitch; x++) {
				neighbors[y*pitch + x] =
				   at(x+1, y+1) +
				   at(x+1, y  ) +
				   at(x+1, y-1) +
				   at(x  , y-1) +
				   at(x-1, y-1) +
				   at(x-1, y  ) +
				   at(x-1, y+1) +
				   at(x  , y+1);
			}
		}

		/* update arena */
		for (y=0; y<height; y++) {
			for (x=0; x<pitch; x++) {
				neigh = neighbors[y*pitch + x];
				alive = arena[y*pitch + x];
				arena[y*pitch + x] =
				  neigh == 3 || (neigh == 2 && alive);
			}
		}
	}
}

static void print_output(void)
{
	int x, y;

	for (y=0; y<height; y++) {
		for (x=0; x<pitch; x++) {
			putc(arena[y*pitch + x] ? '*' : '_', output);
		}
		putc('\n', output);
	}
}

int main(int argc, char *argv[])
{
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
	run_game();
	print_output();
}
