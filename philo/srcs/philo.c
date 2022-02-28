/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 15:05:11 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/28 16:09:09 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	philo_take_forks(t_philo *philo)
{
	if (philo->id % 3 == 0 || \
		(philo->id == 2 && philo->data->nbr_of_philos == 2))
	{
		pthread_mutex_lock(philo->right_fork);
		print_log_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_log_message(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_log_message(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_log_message(philo, "has taken a fork");
	}
}

static void	philo_eat(t_philo *philo)
{
	unsigned long	current_time;

	philo_take_forks(philo);
	pthread_mutex_lock(&philo->philo_mtx);
	current_time = get_timestamp_m();
	if (current_time - philo->last_meal_time < \
		(unsigned long)philo->data->time_to_die)
	{
		philo->last_meal_time = current_time;
		print_log_message(philo, "is eating");
	}
	pthread_mutex_unlock(&philo->philo_mtx);
	nap_timer(philo->last_meal_time + philo->data->time_to_eat);
	pthread_mutex_lock(&philo->philo_mtx);
	philo->meal_count++;
	if (philo->data->nbr_of_times_each_must_eat \
		&& philo->meal_count >= \
		philo->data->nbr_of_times_each_must_eat)
		philo->finished_eating = true;
	pthread_mutex_unlock(&philo->philo_mtx);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	philo_sleep(t_philo *philo)
{
	unsigned long	sleep_start;

	sleep_start = get_timestamp_m();
	print_log_message(philo, "is sleeping");
	nap_timer(sleep_start + philo->data->time_to_sleep);
}

static void	philo_think(t_philo *philo)
{
	if (get_timestamp_m() < philo->next_meal_time - 10)
	{
		print_log_message(philo, "is thinking");
		nap_timer(philo->next_meal_time - 10);
	}
	philo->next_meal_time += philo->data->time_between_meals;
}

void	*philo_thread(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *)arg;
	if (philo->data->nbr_of_philos == 1)
	{
		pthread_mutex_lock(&philo->data->forks[0]);
		print_log_message(philo, "has taken a fork");
		pthread_mutex_unlock(&philo->data->forks[0]);
		return (NULL);
	}
	while (!philo_thread_is_finished(philo))
	{
		philo_think(philo);
		philo_eat(philo);
		philo_sleep(philo);
	}
	return (NULL);
}
