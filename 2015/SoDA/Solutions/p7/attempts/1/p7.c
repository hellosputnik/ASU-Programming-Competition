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

static int wide, high;

struct board {
	char data[60][60]; /* row col */
	int krow, kcol;
	struct board *next;
};

struct board *head = NULL;
struct board *tail = NULL;

static void dump_board(struct board *b)
{
	int r, c;

	for (r=0; r<high; r++) {
		for (c=0; c<wide; c++)
			fprintf(stderr, "%c", r == b->krow && c == b->kcol ?
			  'K' : b->data[r][c]);
		fprintf(stderr, "\n");
	}
}

static void parse_input(void)
{
	char *ln, *s;
	int row, col;

	head = calloc(1, sizeof(*head));
	tail = head;

	ln = get_line();
	if (ln == NULL) format_error();
	wide = strlen(ln) - 1;

	for (row=0; ln; row++) {
		for (col=0; col<wide; col++) {
			if (ln[col] == 'K') {
				head->krow = row;
				head->kcol = col;
				head->data[row][col] = '_';
			}
			head->data[row][col] = ln[col];
		}

		ln = get_line();
	}
	high = row;
}

static void try_dfs_variant(struct board *cur, int drow, int dcol)
{
	struct board *next;
	int nr = cur->krow + drow;
	int nc = cur->kcol + dcol;

	if (nr < 0 || nr >= high || nc < 0 || nc >= wide)
		return;

	if (cur->data[nr][nc] == '_')
		return;

	next = calloc(1, sizeof(*next));
	memcpy(next, cur, sizeof(*next));

	next->krow = nr;
	next->kcol = nc;
	next->data[nr][nc] = '_';

	next->next = head;
	head = next;
}

static void bfs_step(void)
{
	struct board *cur = head;
	head = head->next;

	try_dfs_variant(cur,  1,  2);
	try_dfs_variant(cur, -1,  2);
	try_dfs_variant(cur, -1, -2);
	try_dfs_variant(cur,  1, -2);
	try_dfs_variant(cur,  2,  1);
	try_dfs_variant(cur, -2,  1);
	try_dfs_variant(cur, -2, -1);
	try_dfs_variant(cur,  2, -1);

	free(cur);
}

static bool is_tour(struct board *cur)
{
	int r, c;

	for (r=0; r<high; r++) {
		for (c=0; c<wide; c++) {
			if (cur->data[r][c] != '_')
				return false;
		}
	}
}

static void run_tour(void)
{
	while (head != NULL) {
		if (is_tour(head)) {
			fprintf(output, "YES\n");
			return;
		}

		bfs_step();
	}

	fprintf(output, "NO\n");
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
	run_tour();
}
