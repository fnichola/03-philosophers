/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/24 17:41:14 by fnichola          #+#    #+#             */
/*   Updated: 2022/03/03 17:11:53 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	alloc_memory(t_data *data, t_philo **philos)
{
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nbr_of_philos);
	if (!data->forks)
		return (ERROR);
	*philos = malloc(sizeof(t_philo) * data->nbr_of_philos);
	if (!*philos)
	{
		free(data->forks);
		return (ERROR);
	}
	return (SUCCESS);
}
