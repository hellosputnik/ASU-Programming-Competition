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

static int *array;
static int size;
static int num_swaps = 0;

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
	char *ln;
	int i;

	/* number of elements */
	if (!(ln = get_line())) format_error();
	size = strtoul(ln, NULL, 0);
	array = calloc(size, sizeof(*array));

	/* elements */
	for (i=0; i<size; i++) {
		if (!(ln = get_line())) format_error();
		array[i] = strtoul(ln, NULL, 0);
	}
}

static void run_sort(void)
{
	int i, j, t;

	for (j=0; j<size; j++) {
		for (i=1; i<size; i++) {
			if (array[i-1] <= array[i])
				continue;

			num_swaps ++;
			t = array[i-1];
			array[i-1] = array[i];
			array[i] = t;
		}
	}
}

static void print_output(void)
{
	fprintf(output, "%d\n", num_swaps);
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
	run_sort();
	print_output();
}
