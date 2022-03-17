#include "libhmreadline.h"

void display_cursor(int x)
{
	char *num;
	int	i = 0;

	num = ft_itoa(x + 1);
	while (num[i])
		i++;
	write(STDOUT_FILENO, "\x1b[1;", 4);
	write(STDOUT_FILENO, num, i);
	write(STDOUT_FILENO, "H", 1);
}

void disableRawMode(t_termios *origin) {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, origin);
}

void editorRefreshScreen(int pos) {
	write(STDOUT_FILENO, "\x1b[?25l", 6);
	write(STDOUT_FILENO, "\x1b[2J", 4);
	display_cursor(pos);
	write(STDOUT_FILENO, "\x1b[H", 3);
	write(STDOUT_FILENO, "\x1b[?25h", 6);
}

void enableRawMode(t_termios *origin) {
	t_termios custom;

	tcgetattr(STDIN_FILENO, origin);
	custom = *origin;
	custom.c_lflag &= ~(ECHO | ICANON);
	custom.c_oflag &= ~(OPOST);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &custom);
}

int	add_letter(unsigned char c, t_line *line)
{
	t_letter *curr;
	size_t		i;
	t_letter *new;

	i = 0;
	// printf("\npos : %ld\n", line->pos);
	curr = line->first;
	while (curr && curr->next && ++i < line->pos)
		curr = curr->next;
	if (!xmalloc(&new, sizeof(*new), LETTER_GROUP))
		return (0);
	new->c = c;
	if (curr && line->pos)
	{
		new->next = curr->next;
		curr->next = new;
	}
	else
	{
		new->next = curr;
		line->first = new;
	}
	line->pos++;
	line->len++;
	return (1);
}

void display_line(t_line *line)
{
	t_letter *curr;
	int	 i;
	char *str;

	i = -1;
	curr = line->first;
	if (!xmalloc(&str, line->len + 1, 1))
		return ;
	while (curr)
	{
		str[++i] = curr->c;
		curr = curr->next;
	}
	write(STDOUT_FILENO, str, line->len);
	// printf("pos: %ld\n", line->pos);
	xfree(line, 1);
}

char *xreadline(char *(*prefix)(void), int history_fd)
{
	t_termios		origin;
	t_line			line;
	unsigned char	c;

	(void)prefix;
	(void)history_fd;
	line.first = NULL;
	line.pos = 0;
	enableRawMode(&origin);
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 10) {
		editorRefreshScreen(line.pos);
		if (c == '\x1b') {
			char seq[2];
			if (read(STDIN_FILENO, &seq[0], 1) != 1) break ;
			if (read(STDIN_FILENO, &seq[1], 1) != 1) break ;
			if (seq[0] == '[' && seq[1] == 'C' && line.pos < line.len)
				line.pos++;
			if (seq[0] == '[' && seq[1] == 'D' && line.pos > 0)
				line.pos--;
		}
		else if (!add_letter(c, &line))
			return (0);
		display_line(&line);
		display_cursor(line.pos);
	}
	return ("");
}
