/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   out_err.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:55:07 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 22:36:31 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_all(t_init_data *init_data, int i)
{
	if (init_data->philo)
	{
		while (++i < init_data->kol)
		{
			kill(init_data->philo[i].pid_p, SIGKILL);
			sem_post(init_data->sem);
		}
		sem_post(init_data->fin);
		free(init_data->philo);
	}
	sem_close(init_data->fin);
	sem_close(init_data->sem);
	sem_unlink("end");
	sem_unlink("sem");
	sem_close(init_data->fork);
	sem_close(init_data->vivod);
	sem_unlink("fork");
	sem_unlink("vivod");
}

int		error(t_init_data *init_data, int i)
{
	if (i == 2)
	{
		write(2, "Ошибка маллока", ft_strlen("Ошибка маллока"));
		clean_all(init_data, -1);
	}
	else if (i == 3)
	{
		write(2, "Ошибка семафора", ft_strlen("Ошибка семафора"));
		clean_all(init_data, -1);
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
	char *tmp;

	sem_wait(philo->init_data->vivod);
	if (g_flag != -1)
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
			g_flag = -1;
			return ;
		}
	}
	sem_post(philo->init_data->vivod);
}
