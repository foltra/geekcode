/*
 * Geek Code Generator v2
 * Copyright (C) 2007 Charlie Shepherd <masterdriverz@gentoo.org>
 * Based on code by Chris Gushue <chris@blackplasma.net>
 * License: GPL2
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "geekcode.h"
#include "getanswer.h"
#include "consoleio.h"

#define eof_error() do {		\
	fprintf(stderr,			\
"\n\nGot EOF on stdin, aborting...\n");	\
	exit(1);			\
} while (0);

/* Return the amount of character c's in s */
static int count(char *s, char c)
{
	int i=0;
	for (; *s; s++)
		if (*s == c)
			i++;
	return i;
}

static char *make_spaces(int amount)
{
	/* Cache? */
	char *s=malloc(amount+1);
	if (!s)
		return NULL;
	memset(s, ' ', amount);
	s[amount] = '\0';
	return s;
}

int getanswer(const char *name, int page_num,
		const struct stuff *objects,
		const char *additional)
{
	int selection, line_count;
	char *spaces=make_spaces(66-strlen(name));

	do {
		int i, num_count=MAX_LINES, overflowed=0;
		line_count = 0;
		clearscreen();

		printf("%s%sPage %2i of %i\n", name, spaces, PAGES-(PAGES-page_num), PAGES);
		puts("===============================================================================");
		for (i=0; objects[i].comment; i++) {
			char *s=objects[i].alias;
			if (!num_count) {
				printf("Press enter to continue: ");
				while (1) {
					int c = getchar();
					if (c == EOF)
						eof_error();
					if (c == '\n')
						break;
				}
				num_count = MAX_LINES;
				overflowed = 1;
			}
			if (additional) {
				s = malloc(strlen(s)+strlen(additional)+1);
				if (!s) {
					perror(NULL);
					exit(-1);
				}
				sprintf(s, objects[i].alias, additional);
			}
			printf("%2d %-5s %s\n", i+1, s, objects[i].comment);
			num_count -= count(objects[i].comment, '\n')+1;
			if (additional)
				free(s);
			line_count++;
		}
		if (!overflowed)
			while (num_count-- > 0)
				printf("\n");
		printf("Enter your %s code number here [0 to quit]: ", name);
		if (scanf("%d", &selection) == EOF)
			eof_error();

		clear_kb();
	} while (selection < 0 || selection > line_count);

	free(spaces);
	if (selection == 0)
		exit(0);
	else
		return selection;
}

struct stuff *getcontent(struct stuff2 *obj)
{
	if (!obj->answer || obj->answer == -1) {
		errno = EINVAL;
		return NULL;
	}

	return &obj->contents[obj->answer];
}