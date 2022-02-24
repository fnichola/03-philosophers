/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 11:41:05 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/24 11:45:29 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	atoi_check(const char *str)
{
	long long	nbr;

	nbr = ft_atoll(str);
	if (nbr > INT_MAX || nbr < 0)
		nbr = 0;
	return ((int)nbr);
}
