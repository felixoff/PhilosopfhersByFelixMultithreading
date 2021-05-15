/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 18:47:31 by sjennett          #+#    #+#             */
/*   Updated: 2021/04/20 23:53:02 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	clean_all(t_init_data *init_data)
{
	int	i;

	i = 0;
	if (init_data)
	{
		if (init_data->fork)
		{
			while (i < init_data->kol)
				pthread_mutex_destroy(&init_data->fork[i++]);
			free(init_data->fork);
		}
		pthread_mutex_destroy(&init_data->mutex);
		if (init_data->philo)
		{
			i = 0;
			while (i < init_data->kol)
				pthread_mutex_destroy(&init_data->philo[i++].mutex);
			free(init_data->philo);
			init_data->philo = NULL;
		}
	}
	return (1);
}

int	error(t_init_data *init_data, int i)
{
	if (i == 2)
	{
		write(2, "Ошибка маллока", ft_strlen("Ошибка маллока"));
		clean_all(init_data);
	}
	else if (i == 3)
	{
		write(2, "Ошибка мьютекса", ft_strlen("Ошибка мьютекса"));
		clean_all(init_data);
	}
	return (i);
}

char	*output(int i)
{
	if (i == 1)
		return (" : беру вилку\n");
	else if (i == 2)
		return (" : кушаю\n");
	else if (i == 3)
		return (" : сплю\n");
	else if (i == 4)
		return (" : думаю\n");
	else if (i == 5)
		return (" : умер RIP\n");
	return (" : все наелись\n");
}

void	show(t_philo *philo, int i)
{
	char	*tmp;

	pthread_mutex_lock(&philo->init_data->mutex);
	if (!philo->init_data->finish)
	{
		tmp = output(i);
		ft_putnbr_fd(1, timer() - philo->init_data->time);
		write(1, "\t", 1);
		if (i != 6)
		{
			write(1, "\tPhilosoph number", ft_strlen(" Philosoph number"));
			ft_putnbr_fd(1, philo->index + 1);
		}
		write(1, tmp, ft_strlen(tmp));
		tmp = NULL;
		if (i == 5 || i == 6)
		{
			philo->init_data->finish = 1;
			return ;
		}
	}
	pthread_mutex_unlock(&philo->init_data->mutex);
}

int	init_thread(t_init_data *init_data)
{
	int			i;
	pthread_t	thread;

	init_data->time = timer();
	i = 0;
	while (i < init_data->kol)
	{
		init_data->philo[i].time = timer();
		if (pthread_create(&thread, NULL, \
		start_process, (void *)&init_data->philo[i]))
			return (1);
		pthread_detach(thread);
		if (pthread_create(&init_data->philo[i].thread, NULL, \
		&dead_philo, (void *)&init_data->philo[i]))
			return (1);
		usleep(1000);
		i++;
	}
	i = 0;
	while (i < init_data->kol)
		pthread_join(init_data->philo[i++].thread, NULL);
	return (0);
}
