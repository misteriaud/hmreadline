#include "../libmemory/includes/libmemory.h"
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

typedef struct termios t_termios;
#define CTRL_KEY(q) ((q) & 0x1f)
#define LETTER_GROUP 1

typedef struct s_letter {
	unsigned char c;
	struct s_letter *next;
}				t_letter;

typedef struct s_line {
	t_letter	*first;
	size_t		pos;
	int			offset;
	size_t		len;
}				t_line;

char *xreadline(char *(*prefix)(void), int history_fd);
char	*ft_itoa(int n);
