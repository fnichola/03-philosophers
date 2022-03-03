/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 17:05:32 by fnichola          #+#    #+#             */
/*   Updated: 2022/03/03 17:05:52 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	create_threads(t_data *data, t_philo **philos)
{
	int		i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_create(&(*philos)[i].thread, NULL, \
			philo_thread, &(*philos)[i]) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

void	clean_up_threads(t_data *data, t_philo **philos)
{
	int		i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_join((*philos)[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_destroy(&(*philos)[i].philo_mtx);
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->philo_died_mtx);
	free(data->forks);
	free(*philos);
}
