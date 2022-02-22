/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 15:11:04 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/21 20:47:25 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "t_bool.h"

# define ERROR 0
# define SUCCESS 1

typedef struct s_data {
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philosopher_must_eat;
	int				time_between_meals;
	unsigned long	start_time;
	t_bool			philo_died;
	pthread_mutex_t	philo_died_mtx;
	pthread_mutex_t	*forks;
}	t_data;

typedef struct s_philo {
	int				ID;
	pthread_t		thread;
	unsigned long	last_meal_time;
	pthread_mutex_t	last_meal_time_mtx;
	unsigned long	next_meal_time;
	t_data			*data;
}	t_philo;

int				ft_atoi(const char *nptr);
unsigned long	get_timestamp_m(void);
int				philo_is_dead(t_data *data, t_philo **philos);
void			philo_eat(pthread_mutex_t *left_fork, pthread_mutex_t *right_fork, t_philo *philo);
void			philo_sleep(t_philo *philo);
void			philo_think(t_philo *philo);
void			*philo_thread(void *arg);
int				larger_of_two(int a, int b);
unsigned long	get_sim_time(t_data *data);
void			nap_timer(unsigned long wake_time);

#endif
