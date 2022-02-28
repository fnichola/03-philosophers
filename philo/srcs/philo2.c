/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 17:42:39 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/28 16:09:08 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	philo_is_dead(t_data *data, t_philo *philo)
{
	if (get_timestamp_m() - philo->last_meal_time > \
		(unsigned long)data->time_to_die && !philo->finished_eating)
	{
		pthread_mutex_lock(&data->philo_died_mtx);
		printf("%06lu %d died\n", get_sim_time(data), philo->id);
		data->philo_died = true;
		pthread_mutex_unlock(&data->philo_died_mtx);
		return (true);
	}
	else
		return (false);
}

bool	philo_monitor(t_data *data, t_philo **philos)
{
	int		i;
	int		finished_philos;
	bool	end_simulation;

	end_simulation = false;
	i = 0;
	finished_philos = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(&(*philos)[i].philo_mtx);
		if (philo_is_dead(data, &(*philos)[i]))
			end_simulation = true;
		if ((*philos)[i].finished_eating)
			finished_philos++;
		pthread_mutex_unlock(&(*philos)[i].philo_mtx);
		i++;
	}
	if (finished_philos == data->nbr_of_philos)
		end_simulation = true;
	return (end_simulation);
}

bool	philo_thread_is_finished(t_philo *philo)
{
	bool	is_finished;

	is_finished = false;
	pthread_mutex_lock(&philo->data->philo_died_mtx);
	if (philo->data->philo_died)
		is_finished = true;
	pthread_mutex_unlock(&philo->data->philo_died_mtx);
	pthread_mutex_lock(&philo->philo_mtx);
	if (philo->finished_eating)
		is_finished = true;
	pthread_mutex_unlock(&philo->philo_mtx);
	return (is_finished);
}
