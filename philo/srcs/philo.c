/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 15:05:11 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/18 21:52:07 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

t_bool	philo_is_dead(t_data *data, t_philo **philos)
{
	int				i;
	unsigned long	current_time;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		current_time = get_timestamp_m();
		pthread_mutex_lock(&(*philos)[i].last_meal_time_mtx); // unlock after death?
		if (current_time - (*philos)[i].last_meal_time > (unsigned long)data->time_to_die)
		{
			printf("%06lu %d died\n", get_sim_time(data), i + 1);
			return (TRUE);
		}
		pthread_mutex_unlock(&(*philos)[i].last_meal_time_mtx);
		i++;
	}
	return (FALSE);
}

void	philo_eat(pthread_mutex_t *left_fork, pthread_mutex_t *right_fork, t_philo *philo)
{
	pthread_mutex_lock(left_fork);
	printf("%06lu %d has taken a fork\n", get_sim_time(philo->data), philo->ID);
	pthread_mutex_lock(right_fork);
	printf("%06lu %d has taken a fork\n", get_sim_time(philo->data), philo->ID);
	pthread_mutex_lock(&philo->last_meal_time_mtx);
	philo->last_meal_time = get_timestamp_m();
	printf("%06lu %d is eating\n", get_sim_time(philo->data), philo->ID);
	pthread_mutex_unlock(&philo->last_meal_time_mtx);
	while(get_timestamp_m() - philo->last_meal_time < (unsigned long)philo->data->time_to_eat)
	{
		if (get_timestamp_m() - philo->last_meal_time < (unsigned long)philo->data->time_to_eat - 20)
			usleep(10000);
	}	
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(right_fork);
}

void	philo_sleep(t_philo *philo)
{
	unsigned long	sleep_start;

	sleep_start = get_timestamp_m();
	printf("%06lu %d is sleeping\n", get_sim_time(philo->data), philo->ID);
	while (get_timestamp_m() - sleep_start < (unsigned long)philo->data->time_to_sleep)
	{
		if (get_timestamp_m() - sleep_start < (unsigned long)philo->data->time_to_sleep - 20)
			usleep(10000);
	}
}

void	philo_think(t_philo *philo)
{
	if (get_timestamp_m() < philo->next_meal_time - 5)
		printf("%06lu %d is thinking\n", get_sim_time(philo->data), philo->ID);
	while (get_timestamp_m() < philo->next_meal_time - 5)
	{
		if (get_timestamp_m() < philo->next_meal_time - 25)
			usleep(10000);
	}
	philo->next_meal_time += philo->data->time_between_meals;
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
		philo_think(philo);
		philo_eat(left_fork, right_fork, philo);
		philo_sleep(philo);
		i++;
		if (philo->data->number_of_times_each_philosopher_must_eat &&
		i > philo->data->number_of_times_each_philosopher_must_eat)
			break ;
	}
	return (NULL);
}
