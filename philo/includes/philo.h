/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/16 15:11:04 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/28 16:08:57 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <limits.h>

# define ERROR 0
# define SUCCESS 1

/**
 * Shared data accessible by all threads.
 * 
 * philo_died is the only variable that should change
 * after philospher threads have been created. It is
 * set to true when any philospher has died.
 */
typedef struct s_data {
	int				nbr_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nbr_of_times_each_must_eat;
	int				time_between_meals;
	unsigned long	start_time;
	bool			philo_died;
	pthread_mutex_t	philo_died_mtx;
	pthread_mutex_t	*forks;
}	t_data;

/**
 * A data structure for each philospher, passed to each
 * thread as it is created.
 * 
 * philo_mtx protects last_meal_time and finished_eating
 */
typedef struct s_philo {
	int				id;
	pthread_t		thread;
	pthread_mutex_t	philo_mtx;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	unsigned long	last_meal_time;
	unsigned long	next_meal_time;
	int				meal_count;
	bool			finished_eating;
	t_data			*data;
}	t_philo;

long long		ft_atoll(const char *str);
int				alloc_memory(t_data *data, t_philo **philos);
int				atoi_check(const char *str);
int				init_mutexes(t_data *data, t_philo **philos);
int				init_data(int argc, char **argv, t_data *data);
int				init_philos(t_data *data, t_philo **philos);
int				create_threads(t_data *data, t_philo **philos);
void			clean_up_threads(t_data *data, t_philo **philos);
void			*philo_thread(void *arg);
bool			philo_is_dead(t_data *data, t_philo *philo);
bool			philo_monitor(t_data *data, t_philo **philos);
bool			philo_thread_is_finished(t_philo *philo);
unsigned long	get_timestamp_m(void);
int				larger_of_two(int a, int b);
unsigned long	get_sim_time(t_data *data);
void			nap_timer(unsigned long wake_time);
void			print_log_message(t_philo *philo, char *message);

#endif
