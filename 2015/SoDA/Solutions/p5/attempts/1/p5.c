#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
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

struct windmill {
	int run_days;
	int power;

	int running_today;
	int days_used;
};

static int total_days;
static int total_windmills;
static struct windmill *windmills;

static int power_generated;

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

	/* number of days */
	if (!(ln = get_line())) format_error();
	total_days = strtoul(ln, NULL, 0);

	/* number of windmills */
	if (!(ln = get_line())) format_error();
	total_windmills = strtoul(ln, NULL, 0);
	windmills = calloc(2+total_windmills, sizeof(*windmills));
	windmills+=1;

	/* each pair */
	for (i=0; i<total_windmills; i++) {
		if (!(ln = get_line())) format_error();
		parse_pair(ln, '(', ')',
		  &windmills[i].run_days,
		  &windmills[i].power);
	}
}

static void run_sim(void)
{
	int day, wm;

	power_generated = 0;

	for (day=1; day<=total_days; day++) {
		//fprintf(stderr, "DAY %d\n", day);

		/* step 0: clear */
		for (wm=0; wm<total_windmills; wm++) {
			windmills[wm].running_today = 1;
		}

		/* step 1: determine if needs repairs */
		for (wm=0; wm<total_windmills; wm++) {
			if (windmills[wm].days_used == windmills[wm].run_days) {
				windmills[wm].days_used = 0;
				windmills[wm].running_today = 0;
				windmills[wm+1].running_today = 0;
				windmills[wm-1].running_today = 0;
			}
		}

		/* step 2: generate power */
		for (wm=0; wm<total_windmills; wm++) {
			if (windmills[wm].running_today) {
				power_generated += windmills[wm].power;
				windmills[wm].days_used++;
				//fprintf(stderr, "  #%d adds %d\n",
				//  wm+1, windmills[wm].power);
			}
		}
		//fprintf(stderr, "  at end of day, %d kWh\n", power_generated);
	}
}

static void print_output(void)
{
	fprintf(output, "%d\n", power_generated);
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
	run_sim();
	print_output();
}
