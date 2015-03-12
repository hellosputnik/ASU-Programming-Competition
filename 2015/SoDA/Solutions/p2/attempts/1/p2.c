#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

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

static int get_int(void)
{
	char *ln = get_line();
	return atoi(ln);
}

static int t_num_words = 31;
static const char *t_words[] = {
	"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
	"Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen",
	"Sixteen", "Seventeen", "Eighteen", "Nineteen", "Twenty", "Thirty",
	"Fourty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety",
	"Hundred", "Thousand", "Million", "Negative",
};

static int t_values[] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	20, 30, 40, 50, 60, 70, 80, 90, 100, 1000, 1000000, -1
};

static int *arena, *neighbors;
static int pitch, height;
static int iterations;

static int index_of(char *word)
{
	int i;

	for (i=0; i<t_num_words; i++) {
		if (!strcasecmp(t_words[i], word))
			return i;
	}

	return -1;
}

static int value_of(char *word)
{
	int i = index_of(word);

	if (i < 0) return 0;

	return t_values[i];
}

static bool is_modifier_word(char *word)
{
	return !strcasecmp(word, "Million")
	    || !strcasecmp(word, "Thousand");
}

static int parse_smaller(char **words, int len)
{
	int i;
	int value = 0;
	int v = 0;

	if (len == 0) return 0;

	for (i=0; i<len; i++) {
		v = value_of(words[i]);
		if (i + 1 < len && !strcasecmp(words[i+1], "Hundred")) {
			v *= 100;
			i++;
		}
		value += v;
	}

	return value;
}

static int parse_english_positive(char **words, int len)
{
	int i;
	int modifier;
	int end = len;
	int value = 0;

	if (len == 0) return 0;

	modifier = 1;
	for (i=len-1; i>=0; i--) {
		if (is_modifier_word(words[i])) {
			value += modifier * parse_smaller(words+i+1, end-i-1);
			modifier = value_of(words[i]);
			end = i;
		}
	}

	value += modifier * parse_smaller(words, end);

	return value;
}

static int parse_english(char **words, int len)
{
	int modifier = 1;

	if (len == 0) return 0;

	if (!strcasecmp(words[0], "Negative")) {
		modifier = -1;
		len--;
		words++;
	}

	return modifier * parse_english_positive(words, len);
}

static char **split(char *s, int *n)
{
	int max;
	char *p;
	char **arr;

	for (p=s; *p; p++) {
		if (isspace(*p))
			max++;
	}

	arr = calloc(max+1, sizeof(char*));

	*n = 0;
	for (p=s; *p;) {
		/* skip spaces */
		while (*p && isspace(*p)) p++;

		if (*p) arr[(*n)++] = p;

		/* skip not spaces */
		while (*p && !isspace(*p)) p++;

		if (*p) *p++ = '\0';
	}

	return arr;
}

struct element {
	int value;
	int index;
};

int compare(const void *_a, const void *_b)
{
	const struct element *a = _a;
	const struct element *b = _b;

	return a->value - b->value;
}

int main(int argc, char *argv[])
{
	char **spl, *ln;
	int i, n, len;

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

	struct element *arr;

	n = get_int();
	arr = calloc(n, sizeof(*arr));

	for (i=0; i<n; i++) {
		if (!(ln = get_line())) format_error();

		spl = split(ln, &len);

		arr[i].value = parse_english(spl, len);
		arr[i].index = i + 1;
	}

	qsort(arr, n, sizeof(*arr), compare);

	for (i=0; i<n; i++) {
		fprintf(output, "%d\n", arr[i].index);
	}
}
