/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 14:34:59 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/17 15:34:54 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_data(int argc, char **argv, t_data *data)
{
	int	i;

	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philosophers);
	if (!data->forks)
		return (ERROR);
	
	i = 0;
	while (i < data->number_of_philosophers)
		if (pthread_mutex_init(&data->forks[i++], NULL) != 0)
			return (ERROR);
	return (SUCCESS);
}

int	init_philos(t_data *data, t_philo **philos)
{
	int	i;


	*philos = malloc(sizeof(t_philo) * data->number_of_philosophers);
	if (!*philos)
	{
		free(data->forks);
		return (ERROR);
	}
	i = 0;
	while (i < data->number_of_philosophers)
	{
		(*philos)[i].ID = i + 1;
		(*philos)[i].data = data;
		(*philos)[i].last_meal_time = get_timestamp_m();
		i++;
	}
	return (SUCCESS);
}

int	create_threads(t_data *data, t_philo **philos)
{
	int		i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		if (pthread_create(&(*philos)[i].thread, NULL, philo_thread, &(*philos)[i]) != 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat " \
			"time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	if (!init_data(argc, argv, &data))
		return (EXIT_FAILURE);
	if (!init_philos(&data, &philos))
		return (EXIT_FAILURE);
	if (!create_threads(&data, &philos))
		return (EXIT_FAILURE);
	while (1)
	{
		if (philo_is_dead(&data, &philos))
			return (EXIT_SUCCESS);
	}
	return (EXIT_SUCCESS);
}
