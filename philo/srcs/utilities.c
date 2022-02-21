/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fnichola <fnichola@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 11:31:15 by fnichola          #+#    #+#             */
/*   Updated: 2022/02/21 15:06:19 by fnichola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/**
 * @brief Get the current timestamp in milliseconds
 * 
 * @return int timestamp
 */
unsigned long	get_timestamp_m(void)
{
	struct timeval	current_time;
	unsigned long	timestamp;

	gettimeofday(&current_time, NULL);
	timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (timestamp);
}

unsigned long	get_sim_time(t_data *data)
{
	return (get_timestamp_m() - data->start_time);
}

int	larger_of_two(int a, int b)
{
	if (a > b)
		return (a);
	else
		return (b);
}

/**
 * @brief A more precise sleep timer. usleep() is used only when there is extra
 * time remaining to allow other threads a chance to execute.
 * 
 * @param wake_time unix time in milliseconds, function returns at this time.
 */
void	nap_timer(unsigned long wake_time)
{
	while (get_timestamp_m() < wake_time)
	{
		if (get_timestamp_m() < wake_time - 20)
			usleep(10000);
	}
}
