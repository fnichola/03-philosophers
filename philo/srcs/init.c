/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 18:27:14 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/24 11:47:56 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_data(int argc, char **argv, t_data *data)
{
	data->nbr_of_philos = atoi_check(argv[1]);
	data->time_to_die = atoi_check(argv[2]);
	data->time_to_eat = atoi_check(argv[3]);
	data->time_to_sleep = atoi_check(argv[4]);
	if (data->nbr_of_philos > 200 || \
		data->nbr_of_philos < 1 || \
		data->time_to_die < 60 || \
		data->time_to_eat < 60 || \
		data->time_to_sleep < 60)
		return (ERROR);
	data->philo_died = FALSE;
	if (argc == 6)
		data->nbr_of_times_each_must_eat = atoi_check(argv[5]);
	else
		data->nbr_of_times_each_must_eat = 0;
	if (data->nbr_of_philos % 2)
		data->time_between_meals = larger_of_two(2 * data->time_to_eat, \
			data->time_to_sleep) + data->time_to_eat;
	else
		data->time_between_meals = larger_of_two(data->time_to_eat, \
			data->time_to_sleep) + data->time_to_eat;
	data->start_time = get_timestamp_m();
	return (SUCCESS);
}

int	init_philos(t_data *data, t_philo **philos)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philos)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].data = data;
		(*philos)[i].last_meal_time = data->start_time;
		(*philos)[i].meal_count = 0;
		(*philos)[i].finished_eating = FALSE;
		if ((i + 1) % 2)
			(*philos)[i].next_meal_time = data->start_time;
		else
			(*philos)[i].next_meal_time = data->start_time + data->time_to_eat;
		i++;
	}
	if (data->nbr_of_philos % 2)
		(*philos)[i - 1].next_meal_time = \
			data->start_time + data->time_to_eat * 2;
	return (SUCCESS);
}

int	init_mutexes(t_data *data, t_philo **philos)
{
	int	i;

	if (pthread_mutex_init(&data->philo_died_mtx, NULL))
		return (ERROR);
	i = 0;
	while (i < data->nbr_of_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (ERROR);
		if (pthread_mutex_init(&(*philos)[i].eating_mtx, NULL))
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

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
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&(*philos)[i].eating_mtx);
		i++;
	}
	pthread_mutex_destroy(&data->philo_died_mtx);
	free(data->forks);
	free(*philos);
}
