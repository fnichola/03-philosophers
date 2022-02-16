/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 14:34:59 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/16 17:41:44 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*philo_thread(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("Hi, I'm philosopher #%d\n", philo->ID);
	return (NULL);
}

void	init_vars(int argc, char **argv, t_data *data)
{
	data->number_of_philosophers = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	else
		data->number_of_times_each_philosopher_must_eat = 0;
}

int	main(int argc, char **argv)
{
	struct timeval	current_time;
	int				timestamp;
	pthread_t		thread;
	pthread_mutex_t	*forks;
	t_data			data;
	t_philo			philo;

	(void)forks;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo number_of_philosophers time_to_die time_to_eat \
			time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	init_vars(argc, argv, &data);
	gettimeofday(&current_time, NULL);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	printf("%d\n", timestamp);
	philo.ID = 42;
	philo.data = data;
	pthread_create(&thread, NULL, philo_thread, &philo);
	pthread_join(thread, NULL);
	return (0);
}
