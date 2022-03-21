/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mriaud <mriaud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 18:43:16 by mriaud            #+#    #+#             */
/*   Updated: 2022/03/21 19:27:48 by mriaud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libhmreadline.h"

int history_match(t_line *line, char *history_line)
{
	int	i;
	t_letter *curr;

	i = 0;
	curr = line->first;
	if (!curr)
		return (1);
	while(curr && history_line && history_line[i])
	{
		if (curr->c != history_line[i])
			return (0);
		curr = curr->next;
		i++;
	}
	if (!curr)
		return (1);
	return (0);
}

void set_autocomp(t_line *line, char **history, int prevnext)
{
	int i;

	if (!history || !*history
		|| (line->history_index >= 0 && history_match(line, history[line->history_index]) && !prevnext))
		return ;
	i = line->history_index;
	if (!prevnext || line->history_index < 0)
	{
		i = 0;
		while (history[i])
			i++;
		i--;
		line->history_index = -1;
	}
	if (prevnext)
	{
		i -= prevnext;
		if (!history[i])
			line->history_index = -1;
	}
	while (i >= 0)
	{
		if (history[i] && history_match(line, history[i]))
		{
			line->history_index = i;
			return ;
		}
		i--;
	}
}

void display_autocomp(t_line *line, char **history)
{
	int	i;
	int j;
	t_letter *curr;

	if(line->cursor_move)
	{
		line->cursor_move = 0;
		return ;
	}
	if (!history || line->history_index < 0)
		return ;
	i = 0;
	j = -1;
	curr = line->first;
	while(curr)
	{
		curr = curr->next;
		i++;
	}
	write(STDOUT_FILENO, "\x1b[s", 3);
	while (line->pos + ++j < line->len)
		write(STDOUT_FILENO, "\x1b[1C", 4);
	write(STDOUT_FILENO, "\033[0;90m", 7);
	while (history[line->history_index][i])
		write(STDOUT_FILENO, &history[line->history_index][i++], 1);
	write(STDOUT_FILENO, "\033[0m", 4);
	write(STDOUT_FILENO, "\x1b[u", 3);
}
