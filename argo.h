#ifndef ARGO_H
# define ARGO_H

# include <stdio.h>

typedef enum		e_jtype
{
	json_int,
	json_str,
	json_map
}				t_jtype;

typedef struct		s_entry	t_entry;

typedef struct		s_json
{
	t_jtype		type;
	union
	{
		int		integer;
		char		*str;
		t_entry		*map;
	};
}				json;

struct			s_entry
{
	char		*key;
	json		*val;
	t_entry		*next;
};

int	argo(json *dst, FILE *stream);

#endif
