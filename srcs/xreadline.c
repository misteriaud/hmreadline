#include "libhmreadline.h"

int ft_strlen(char *s)
{
	int i;

	i = 0;
	if (!s)
		return (i);
	while (s[i])
		i++;
	return (i);
}

void move_cursor(t_line *line, int direction)
{
	if (line->pos + direction > line->len || line->pos + direction < 0)
		return ;
	if (direction == -1)
		write(STDOUT_FILENO, "\x1b[1D", 4);
	else
		write(STDOUT_FILENO, "\x1b[1C", 4);
	line->pos += direction;
}

void enableRawMode(t_termios *origin) {
	t_termios custom;

	tcgetattr(STDIN_FILENO, origin);
	custom = *origin;
	custom.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &custom);
}

int	add_letter(unsigned char c, t_line *line)
{
	t_letter	*curr;
	int			i;
	t_letter	*new;

	i = 0;
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
	line->letter_added = 1;
	line->len++;
	return (1);
}

void remove_letter(t_line *line, char c)
{
	t_letter *curr;
	t_letter *to_remove;
	int	 i;
	int pos;

	i = 0;
	pos = line->pos;
	if (c == 127)
		pos--;
	if (pos < 0 || pos >= line->len || !line->first)
		return ;
	if (!pos)
	{
		to_remove = line->first;
		line->first = to_remove->next;
		xfree(to_remove, LETTER_GROUP);
	}
	else
	{
		curr = line->first;
		while (curr && ++i < pos)
			curr = curr->next;
		to_remove = curr->next;
		curr->next = to_remove->next;
		xfree(to_remove, LETTER_GROUP);
	}
	if (c == 127)
		move_cursor(line, -1);
	line->len--;
}

void print_prefix(t_line *line)
{
	int to_right;

	to_right = ft_strlen(line->prefix);
	write(STDOUT_FILENO, line->prefix, to_right);
	to_right += 3;
	write(STDOUT_FILENO, " > ", 3);
}

void display_line(t_line *line)
{
	t_letter *curr;
	int	 i;
	int	j;
	char *str;

	i = -1;
	if (line->cursor_move)
		return ;
	write(STDOUT_FILENO, "\x1b[s", 3);
	write(STDOUT_FILENO, "\x1b[0K", 4); //clean until end of line
	i = -1;
	j = -1;
	curr = line->first;
	if (!curr || !xmalloc(&str, line->len + 1 - line->pos, 1))
		return ;
	while (curr && ++i < line->pos)
		curr = curr->next;
	while (curr)
	{
		str[++j] = curr->c;
		curr = curr->next;
	}
	write(STDOUT_FILENO, str, line->len + 1 - line->pos);
	write(STDOUT_FILENO, "\x1b[ur", 3);
	if (line->letter_added)
	{
		move_cursor(line, 1);
		line->letter_added = 0;
	}
	xfree(str, 1);
}

char *get_line(t_line *line)
{
	t_letter *curr;
	int	 i;
	char *str;

	i = -1;
	curr = line->first;
	str = NULL;
	if (!xmalloc(&str, line->len + 1, 1))
		return (NULL);
	while (curr)
	{
		str[++i] = curr->c;
		curr = curr->next;
	}
	return (str);
}

int key_handler(char c, t_line *line, char **history)
{
	if (c == '\x1b') {
		char seq[2];
		if (read(STDIN_FILENO, &seq[0], 1) != 1)
			return (0);
		if (read(STDIN_FILENO, &seq[1], 1) != 1)
			return (0);
		if (seq[0] == '[' && seq[1] == 'A')
			set_autocomp(line, history, prev);
		if (seq[0] == '[' && seq[1] == 'B')
			set_autocomp(line, history, next);
		if (seq[0] == '[' && seq[1] == 'C')
		{
			move_cursor(line, 1);
			line->cursor_move = 1;
		}
		if (seq[0] == '[' && seq[1] == 'D')
		{
			move_cursor(line, -1);
			line->cursor_move = 1;
		}
	}
	else if (c == 127)
		remove_letter(line, c);
	else if (c == 126)
		remove_letter(line, c);
	else if (!add_letter(c, line))
		return (0);
	return (1);
}


char *xreadline(char *(*prefix)(void), int history_fd)
{
	t_termios		origin;
	t_line			line;
	char			**history;
	unsigned char	c;
	// static int		vert_pos;

	history = NULL;
	if (history_fd > 0)
		history = parse_history(history_fd);
	line.first = NULL;
	line.prefix = prefix();
	line.pos = 0;
	line.len = 0;
	line.cursor_move = 0;
	line.letter_added = 0;
	line.history_index = -1;
	enableRawMode(&origin);
	print_prefix(&line);
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 10) {
		if (!key_handler(c, &line, history))
			return (0);
		set_autocomp(&line, history, last);
		display_line(&line);
		display_autocomp(&line, history);
	}
	return (get_line(&line));
}
