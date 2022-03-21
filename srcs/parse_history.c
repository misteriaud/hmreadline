/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_history.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mriaud <mriaud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/17 21:09:01 by mriaud            #+#    #+#             */
/*   Updated: 2022/03/21 18:50:10 by mriaud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libhmreadline.h"

int	ft_cstrlen(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

char	*ft_strcat(char *a, char *buff, int *nl)
{
	char	*dest;
	int		i;
	int		j;

	i = -1;
	j = -1;
	if(!xmalloc(&dest, ft_cstrlen(a) + ft_cstrlen(buff) + 1, PARSING_GROUP))
		return (NULL);
	if (dest && !a)
		i = 0;
	while (dest && a && a[++i])
		dest[i] = a[i];
	while (dest && buff[++j])
	{
		dest[i + j] = buff[j];
		if (buff[j] == '\n')
			*nl += 1;
	}
	if (dest)
		dest[i + j] = 0;
	if (a)
		xfree(a, PARSING_GROUP);
	return (dest);
}

char	*get_file(int fd, int *nl)
{
	char	buff[BUFFER_SIZE];
	char	*dest;
	int		read_return;

	dest = NULL;
	read_return = read(fd, buff, BUFFER_SIZE);
	while (read_return > 0)
	{
		buff[read_return] = 0;
		dest = ft_strcat(dest, buff, nl);
		if (!dest)
			return (NULL);
		read_return = read(fd, buff, BUFFER_SIZE);
	}
	if (dest && dest[ft_cstrlen(dest) - 1] != '\n')
		*nl += 1;
	return (dest);
}

char **parse_history(int history_fd)
{
	char 	**history;
	char	*file;
	int		nl;

	history = NULL;
	file = get_file(history_fd, &nl);
	if (!file)
	{
		xfree_group(PARSING_GROUP);
		return (NULL);
	}
	history = ft_split(file, '\n');
	return (history);
}
