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

static char *put_spaces(char *s)
{
	char *p, *str = calloc(strlen(s) * 2 + 2, sizeof(char));

	for (p=str; *s; s++) {
		*p++ = *s;
		*p++ = ' ';
	}
	*p = '\0';

	return str;
}

/* returns true if we flipped everything */
static bool increment(char *s)
{
	char *ops = s;

	while (ops[0]) {
		switch (ops[1]) {
		case ' ': ops[1] = '*'; return false;
		case '*': ops[1] = '+'; return false;
		case '+': ops[1] = '-'; return false;
		case '-': ops[1] = ' '; ops += 2;
		}
	}

	return true;
}

struct evalnode {
	int value;
	char op;
	struct evalnode *next;
};

static bool is_valid(char *ex)
{
	char *s;
	bool was_operator = true;
	for (s=ex; *s; s+=2) {
		if (was_operator && s[0] == '0' && s[1] == ' ' && s[2] != '\0')
			return false;
		if (s[1] != ' ')
			was_operator = true;
		else
			was_operator = false;
	}
	return s[strlen(s)-1] == ' ';
}

static int evaluate(char *ex)
{
	struct evalnode *head=NULL, *tail=NULL, *cur, *tmp;
	char *s;
	int value;

	/* first parse it into a list of nodes */
	for (s=ex; *s; s+=2) {
		cur = calloc(1, sizeof(*cur));
		cur->value = s[0] - '0';
		cur->op = s[1];
		cur->next = NULL;
		if (head == NULL)
			head = cur;
		if (tail != NULL)
			tail->next = cur;
		tail = cur;
	}

	/* reduce all the spaces */
	for (cur=head; cur&&cur->next; ) {
		if (cur->op == ' ') {
			cur->value = cur->value * 10 + cur->next->value;
			cur->op = cur->next->op;
			tmp = cur->next->next;
			free(cur->next);
			cur->next = tmp;
		} else {
			cur = cur->next;
		}
	}

	/* reduce all the stars */
	for (cur=head; cur&&cur->next; ) {
		if (cur->op == '*') {
			cur->value = cur->value * cur->next->value;
			cur->op = cur->next->op;
			tmp = cur->next->next;
			free(cur->next);
			cur->next = tmp;
		} else {
			cur = cur->next;
		}
	}

	/* reduce all the plus and minus */
	for (cur=head; cur&&cur->next; ) {
		if (cur->op == '+' || cur->op == '-') {
			if (cur->op == '+')
				cur->value = cur->value + cur->next->value;
			else
				cur->value = cur->value - cur->next->value;
			cur->op = cur->next->op;
			tmp = cur->next->next;
			free(cur->next);
			cur->next = tmp;
		} else {
			cur = cur->next;
		}
	}

	value = head ? head->value : 0;

	for (cur=head; cur; cur=tmp) {
		tmp = cur->next;
		free(cur);
	}

	return value;
}

static int count_eq(char *a, char *borig)
{
	int count = 0;
	char *b;

	b = strdup(borig);

	for (;;) {
		if (is_valid(a)) {
			strcpy(b, borig);
			for (;;) {
				if (is_valid(b)) {
					if (evaluate(a) == evaluate(b))
						count++;
				}
				if (increment(b))
					break;
			}
		}
		if (increment(a))
			break;
	}

	return count;
}

int main(int argc, char *argv[])
{
	char *ln, *b, *s;

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

	for (;;) {
		if ((ln = get_line()) == NULL)
			break;
		if (ln[0] == '=')
			break;

		b = ln;
		while (*b != '=') b++;
		*b++ = '\0';

		s = b;
		while (*s && *s != '\n') s++;
		*s = '\0';
		
		printf("%d\n", count_eq(put_spaces(ln), put_spaces(b)));
	}
}
