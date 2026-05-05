#include "argo.h"
#include <stdlib.h>
#include <ctype.h>

static void	err(int c)
{
	if (c == EOF)
		printf("unexpected end of input\n");
	else
		printf("unexpected token '%c'\n", c);
}

static int	next_non_space(FILE *f)
{
	int	c;

	c = getc(f);
	while (c != EOF && isspace((unsigned char)c))
		c = getc(f);
	return (c);
}

/*
** parse_str: stream is positioned AFTER the opening '"'
** reads until closing '"', handles \" and \\
*/
static int	parse_str(char **out, FILE *f)
{
	int		c;
	int		i = 0;
	int		cap = 32;
	char	*buf = malloc(cap);

	if (!buf)
		return (-1);
	while (1)
	{
		c = getc(f);
		if (c == EOF) { free(buf); err(EOF); return (-1); }
		if (c == '"') break ;
		if (c == '\\')
		{
			c = getc(f);
			if (c == EOF) { free(buf); err(EOF); return (-1); }
			if (c != '"' && c != '\\') { free(buf); err(c); return (-1); }
		}
		if (i + 2 >= cap)
		{
			cap *= 2;
			buf = realloc(buf, cap);
			if (!buf) return (-1);
		}
		buf[i++] = c;
	}
	buf[i] = '\0';
	*out = buf;
	return (1);
}

static int	parse_map(t_entry **out, FILE *f)
{
	int		c;
	t_entry	*head = NULL;
	t_entry	**cur = &head;

	getc(f);
	c = next_non_space(f);
	if (c == '}') { *out = NULL; return (1); }
	ungetc(c, f);
	while (1)
	{
		c = next_non_space(f);
		if (c != '"') { err(c); return (-1); }
		*cur = calloc(1, sizeof(t_entry));
		if (!*cur) return (-1);
		if (parse_str(&(*cur)->key, f) < 0) return (-1);
		c = next_non_space(f);
		if (c != ':') { err(c); return (-1); }
		(*cur)->val = calloc(1, sizeof(json));
		if (!(*cur)->val) return (-1);
		if (argo((*cur)->val, f) < 0) return (-1);
		c = next_non_space(f);
		if (c == '}') break ;
		if (c != ',') { err(c); return (-1); }
		cur = &(*cur)->next;
	}
	*out = head;
	return (1);
}

int	argo(json *dst, FILE *f)
{
	int	c = next_non_space(f);

	if (c == EOF) { err(EOF); return (-1); }
	if (c == '"')
	{
		dst->type = json_str;
		return (parse_str(&dst->str, f));
	}
	if (c == '{')
	{
		dst->type = json_map;
		ungetc(c, f);
		return (parse_map(&dst->map, f));
	}
	if (isdigit(c) || c == '-')
	{
		ungetc(c, f);
		dst->type = json_int;
		if (fscanf(f, "%d", &dst->integer) != 1) { err(c); return (-1); }
		return (1);
	}
	err(c);
	return (-1);
}

static void	free_json(json *j)
{
	t_entry	*e;
	t_entry	*n;

	if (!j)
		return ;
	if (j->type == json_str)
		free(j->str);
	else if (j->type == json_map)
	{
		e = j->map;
		while (e)
		{
			n = e->next;
			free(e->key);
			free_json(e->val);
			free(e->val);
			free(e);
			e = n;
		}
	}
}

static void	print_str(const char *s)
{
	printf("\"");
	while (*s)
	{
		if (*s == '"' || *s == '\\')
			printf("\\");
		printf("%c", *s++);
	}
	printf("\"");
}

static void	print_json(const json *j)
{
	t_entry	*e;

	if (j->type == json_int)
		printf("%d", j->integer);
	else if (j->type == json_str)
		print_str(j->str);
	else
	{
		printf("{");
		e = j->map;
		while (e)
		{
			print_str(e->key);
			printf(":");
			print_json(e->val);
			if (e->next)
				printf(",");
			e = e->next;
		}
		printf("}");
	}
}

int __attribute__((weak))	main(int argc, char **argv)
{
	FILE	*f;
	json	j;
	int	c;

	if (argc != 2)
		return (1);
	j.type = json_int;
	j.integer = 0;
	f = fopen(argv[1], "r");
	if (!f)
		return (1);
	if (argo(&j, f) < 0)
	{
		fclose(f);
		free_json(&j);
		return (1);
	}
	c = next_non_space(f);
	fclose(f);
	if (c != EOF)
	{
		err(c);
		free_json(&j);
		return (1);
	}
	print_json(&j);
	printf("\n");
	free_json(&j);
	return (0);
}
