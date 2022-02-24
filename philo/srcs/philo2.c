/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 17:42:39 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/24 11:27:53 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_bool	philo_is_dead(t_data *data, t_philo *philo)
{
	if (get_timestamp_m() - philo->last_meal_time > \
		(unsigned long)data->time_to_die && !philo->finished_eating)
	{
		pthread_mutex_lock(&data->philo_died_mtx);
		printf("%06lu %d died\n", get_sim_time(data), philo->id);
		data->philo_died = TRUE;
		pthread_mutex_unlock(&data->philo_died_mtx);
		pthread_mutex_unlock(&philo->eating_mtx);
		return (TRUE);
	}
	else
		return (FALSE);
}

t_bool	philo_monitor(t_data *data, t_philo **philos)
{
	int	i;
	int	finished_philos;

	i = 0;
	finished_philos = 0;
	while (i < data->nbr_of_philos)
	{
		pthread_mutex_lock(&(*philos)[i].eating_mtx);
		if (philo_is_dead(data, &(*philos)[i]))
			return (TRUE);
		if ((*philos)[i].finished_eating)
			finished_philos++;
		pthread_mutex_unlock(&(*philos)[i].eating_mtx);
		i++;
	}
	if (finished_philos == data->nbr_of_philos || data->philo_died)
		return (TRUE);
	else
		return (FALSE);
}

t_bool	philo_thread_is_finished(t_philo *philo)
{
	t_bool	is_finished;

	is_finished = FALSE;
	pthread_mutex_lock(&philo->data->philo_died_mtx);
	if (philo->data->philo_died)
		is_finished = TRUE;
	pthread_mutex_unlock(&philo->data->philo_died_mtx);
	pthread_mutex_lock(&philo->eating_mtx);
	if (philo->finished_eating)
		is_finished = TRUE;
	pthread_mutex_unlock(&philo->eating_mtx);
	return (is_finished);
}
