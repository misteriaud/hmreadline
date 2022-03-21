/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mriaud <mriaud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 15:13:26 by mriaud            #+#    #+#             */
/*   Updated: 2022/03/21 18:37:50 by mriaud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libhmreadline.h"

static char	*ft_string(char *s, char c)
{
	size_t	i;
	char	*dest;

	i = 0;
	while (s[i] != c && s[i])
		i++;
	if(!xmalloc(&dest, i + 1, PARSING_GROUP))
		return (NULL);
	i = 0;
	while (s[i] != c && s[i])
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

static size_t	complete_tab(char *s, char c, char **dest)
{
	int	count;
	int	last_is_char;

	count = 0;
	last_is_char = 1;
	while (*s)
	{
		if (*s == c)
			last_is_char = 1;
		else
		{
			if (last_is_char)
			{
				dest[count] = ft_string(s, c);
				if (!dest[count])
					return (count);
				count++;
			}
			last_is_char = 0;
		}
		s++;
	}
	return (count);
}

static int	str_count(char *s, char c)
{
	int	count;
	int	last_is_char;

	count = 0;
	last_is_char = 1;
	while (*s)
	{
		if (*s == c)
			last_is_char = 1;
		else
		{
			if (last_is_char)
				count++;
			last_is_char = 0;
		}
		s++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	char	**dest;
	size_t	count;

	count = str_count((char *)s, c);
	if(!xmalloc(&dest, sizeof(*dest) * (count + 1), PARSING_GROUP))
		return (NULL);
	if (complete_tab((char *)s, c, dest) < count)
	{
		xfree_group(PARSING_GROUP);
		return (NULL);
	}
		dest[count] = NULL;
	return (dest);
}
