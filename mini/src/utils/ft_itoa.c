/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayboudya <ayboudya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 23:23:33 by ayboudya          #+#    #+#             */
/*   Updated: 2025/08/01 06:02:08 by ayboudya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/utils.h"

static int	length(int n)
{
	int	i;

	i = 0;
	if (n < 0)
		i = 1;
	while (n != 0)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static void	reverse(char *r, int len)
{
	int		i;
	char	tmp;

	i = 0;
	if (r[i] == '-')
	{
		len += 1;
		i++;
	}
	while (i < len / 2)
	{
		tmp = r[i];
		r[i] = r[len - i - 1];
		r[len - i - 1] = tmp;
		i++;
	}
}

char	*ft_itoa(int n)
{
	char	*a;
	int		i;
	int		len;

	i = 0;
	len = length(n);
	a = malloc(length(n) + 1);
	if (!a)
		return (NULL);
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (n < 0)
	{
		a[i++] = '-';
		n *= -1;
	}
	while (n != 0)
	{
		a[i++] = n % 10 + '0';
		n = n / 10;
	}
	a[i] = 0;
	return (reverse(a, len), a);
}
