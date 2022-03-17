/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mriaud <mriaud@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/23 16:29:16 by mriaud            #+#    #+#             */
/*   Updated: 2022/03/17 17:16:45 by mriaud           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	ft_len(int n)
{
	size_t	len;

	if (!n)
		return (1);
	len = 0;
	if (n < 0)
		len++;
	while (n != 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*dest;

	len = ft_len(n);
	dest = malloc(len + 1);
	if (!dest)
		return (NULL);
	dest[len] = 0;
	len--;
	while ((len >= 0 && n > 0) || (len > 0 && n < 0))
	{
		dest[len] = ft_abs(n % 10) + '0';
		n /= 10;
		len--;
	}
	if (!len)
		dest[0] = '-';
	return (dest);
}
