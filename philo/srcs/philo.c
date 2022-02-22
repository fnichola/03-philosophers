/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 15:05:11 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/22 18:57:20 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	philo_is_dead(t_data *data, t_philo **philos)
{
	int				i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_mutex_lock(&(*philos)[i].last_meal_time_mtx); // unlock after death?
		if (get_timestamp_m() - (*philos)[i].last_meal_time > (unsigned long)data->time_to_die)
		{
			pthread_mutex_lock(&data->philo_died_mtx);
			printf("%06lu %d died\n", get_sim_time(data), i + 1);
			data->philo_died = TRUE;
			pthread_mutex_unlock(&data->philo_died_mtx);
			pthread_mutex_unlock(&(*philos)[i].last_meal_time_mtx);
			return (i + 1);
		}
		pthread_mutex_unlock(&(*philos)[i].last_meal_time_mtx);
		i++;
	}
	return (0);
}

void	philo_eat(pthread_mutex_t *left_fork, pthread_mutex_t *right_fork, t_philo *philo)
{
	unsigned long	current_time;

	pthread_mutex_lock(&philo->data->philo_died_mtx);
	if (philo->data->philo_died)
	{
		pthread_mutex_unlock(&philo->data->philo_died_mtx);
		return ;
	}
	pthread_mutex_unlock(&philo->data->philo_died_mtx);
	if (philo->ID % 2)
	{
		pthread_mutex_lock(right_fork);
		printf("%06lu %d has taken a fork\n", get_sim_time(philo->data), philo->ID);
		pthread_mutex_lock(left_fork);
		printf("%06lu %d has taken a fork\n", get_sim_time(philo->data), philo->ID);
	}
	else
	{
		pthread_mutex_lock(left_fork);
		printf("%06lu %d has taken a fork\n", get_sim_time(philo->data), philo->ID);
		pthread_mutex_lock(right_fork);
		printf("%06lu %d has taken a fork\n", get_sim_time(philo->data), philo->ID);
	}
	pthread_mutex_lock(&philo->last_meal_time_mtx);
	current_time = get_timestamp_m();
	if (current_time - philo->last_meal_time < (unsigned long)philo->data->time_to_die)
	{
		philo->last_meal_time = current_time;
		printf("%06lu %d is eating\n", get_sim_time(philo->data), philo->ID);
	}
	pthread_mutex_unlock(&philo->last_meal_time_mtx);
	nap_timer(philo->last_meal_time + philo->data->time_to_eat);
	pthread_mutex_unlock(left_fork);
	pthread_mutex_unlock(right_fork);
}

void	philo_sleep(t_philo *philo)
{
	unsigned long	sleep_start;

	pthread_mutex_lock(&philo->data->philo_died_mtx);
	if (philo->data->philo_died)
	{
		pthread_mutex_unlock(&philo->data->philo_died_mtx);
		return ;
	}
	pthread_mutex_unlock(&philo->data->philo_died_mtx);
	sleep_start = get_timestamp_m();
	printf("%06lu %d is sleeping\n", get_sim_time(philo->data), philo->ID);
	nap_timer(sleep_start + philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->philo_died_mtx);
	if (philo->data->philo_died)
	{
		pthread_mutex_unlock(&philo->data->philo_died_mtx);
		return ;
	}
	pthread_mutex_unlock(&philo->data->philo_died_mtx);
	if (get_timestamp_m() < philo->next_meal_time - 5)
		printf("%06lu %d is thinking\n", get_sim_time(philo->data), philo->ID);
	nap_timer(philo->next_meal_time - 5);
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
		pthread_mutex_lock(&philo->data->philo_died_mtx);
		if (philo->data->philo_died)
		{
			pthread_mutex_unlock(&philo->data->philo_died_mtx);
			break ;
		}
		pthread_mutex_unlock(&philo->data->philo_died_mtx);
		i++;
		if (philo->data->number_of_times_each_philosopher_must_eat &&
		i > philo->data->number_of_times_each_philosopher_must_eat)
			break ;
	}
	return (NULL);
}
