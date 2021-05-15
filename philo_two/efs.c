/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   efs.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:46:44 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 16:59:26 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_process(void *data)
{
	t_philo	*philo;

	philo = data;
	philo->time = timer();
	while (!philo->init_data->finish)
	{
		take_fork_on(philo);
		if (philo->init_data->finish)
			break ;
		eat(philo);
		if (philo->init_data->finish)
			break ;
		take_fork_off(philo);
		if (philo->init_data->count_eat > 0 && count_eat(philo))
			break ;
		if (philo->init_data->finish)
			break ;
		ft_sleep(philo);
		show(philo, 4);
	}
	sem_post(philo->init_data->fork);
	sem_post(philo->init_data->fork);
	sem_post(philo->init_data->sem);
	return (NULL);
}

void	take_fork_on(t_philo *philo)
{
	sem_wait(philo->init_data->fork);
	sem_wait(philo->init_data->fork);
	show(philo, 1);
	show(philo, 1);
}

void	take_fork_off(t_philo *philo)
{
	sem_post(philo->init_data->fork);
	sem_post(philo->init_data->fork);
}

void	eat(t_philo *philo)
{
	show(philo, 2);
	philo->time = timer();
	sem_wait(philo->sem2);
	usleep(philo->init_data->eat * 1000);
	philo->count_eat++;
	sem_post(philo->sem2);
}

void	ft_sleep(t_philo *philo)
{
	show(philo, 3);
	usleep(philo->init_data->sleep * 1000);
}
