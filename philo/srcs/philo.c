/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 15:05:11 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/17 15:59:00 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_bool	philo_is_dead(t_data *data, t_philo **philos)
{
	int	i;
	int	current_time;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		current_time = get_timestamp_m();
		if (current_time - (*philos)[i].last_meal_time > data->time_to_die)
		{
			printf("%d %d died\n", current_time, i + 1);
			return (TRUE);
		}
		i++;
	}
	return (FALSE);
}

void	philo_eat(pthread_mutex_t *left_fork, pthread_mutex_t *right_fork, t_philo *philo)
{
	if (philo->ID % 2 == 0)
	{
		pthread_mutex_lock(right_fork);
		printf("%d %d has taken a fork\n", get_timestamp_m(), philo->ID);
		pthread_mutex_lock(left_fork);
		printf("%d %d has taken a fork\n", get_timestamp_m(), philo->ID);
	}
	else 
	{
		pthread_mutex_lock(left_fork);
		printf("%d %d has taken a fork\n", get_timestamp_m(), philo->ID);
		pthread_mutex_lock(right_fork);
		printf("%d %d has taken a fork\n", get_timestamp_m(), philo->ID);
	}
	philo->last_meal_time = get_timestamp_m();
	printf("%d %d is eating\n", get_timestamp_m(), philo->ID);
	usleep(philo->data->time_to_eat * 1000);
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(right_fork);
}

void	philo_sleep(t_philo *philo)
{
	printf("%d %d is sleeping\n", get_timestamp_m(), philo->ID);
	usleep(philo->data->time_to_sleep * 1000);
}

void	philo_think(t_philo *philo)
{
	printf("%d %d is thinking\n", get_timestamp_m(), philo->ID);
}

void	*philo_thread(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				i;

	philo = (t_philo *)arg;
	left_fork = &philo->data->forks[philo->ID - 1];
	if (philo->ID == philo->data->number_of_philosophers)
		right_fork = &philo->data->forks[0];
	else
		right_fork = &philo->data->forks[philo->ID];
	i = 0;
	while (1)
	{
		philo_eat(left_fork, right_fork, philo);
		philo_sleep(philo);
		philo_think(philo);
		i++;
		if (philo->data->number_of_times_each_philosopher_must_eat &&
		i > philo->data->number_of_times_each_philosopher_must_eat)
			break ;
	}
	return (NULL);
}
