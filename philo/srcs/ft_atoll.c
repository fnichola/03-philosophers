/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/02 16:59:43 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/24 11:51:44 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <errno.h>

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	atoi_check(const char *str)
{
	long long	nbr;
	size_t		i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	nbr = ft_atoll(str);
	if (nbr > INT_MAX || nbr < 0)
		return (0);
	return ((int)nbr);
}

static long long	calculate_result(char **s, const int sign)
{
	unsigned long long	result;
	unsigned long long	cutoff;
	int					next_digit;
	int					cutlim;

	result = 0;
	cutoff = LLONG_MAX / 10;
	cutlim = LLONG_MAX % 10;
	if (sign == -1)
		cutlim += 1;
	while (ft_isdigit(**s))
	{
		next_digit = **s - '0';
		if ((result == cutoff && next_digit > cutlim) || result > cutoff)
		{
			errno = ERANGE;
			if (sign == -1)
				return (LLONG_MIN);
			else
				return (LLONG_MAX);
		}
		result = result * 10 + next_digit;
		(*s)++;
	}
	return ((long long)(result * sign));
}

long long	ft_atoll(const char *str)
{
	int			sign;
	char		*s;

	s = (char *)str;
	sign = 1;
	while ((*s >= '\t' && *s <= '\r') || (*s == ' '))
		s++;
	if (*s == '-' || *s == '+')
		if (*(s++) == '-')
			sign = -1;
	if (!ft_isdigit(*s))
	{
		errno = EINVAL;
		return (0);
	}
	else
		return (calculate_result(&s, sign));
}
