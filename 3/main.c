/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:50:26 by sjennett          #+#    #+#             */
/*   Updated: 2021/03/02 22:37:32 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int		init_input(int argc, char **argv, t_init_data *init_data)
{
	if (argc < 5 || argc > 6)
	{
		write(2, "Неверное количество аргументов",
		ft_strlen("Неверное количество аргументов"));
		return (1);
	}
	if ((init_data->kol = ft_atoi(argv[1])) < 2)
		write(2, "Неверное количество философов",
		ft_strlen("Неверное количество философов"));
	else if ((init_data->die = ft_atoi(argv[2])) < 60)
		write(2, "Неверное время смерти", ft_strlen("Неверное время смерти"));
	else if ((init_data->eat = ft_atoi(argv[3])) < 60)
		write(2, "Неверное время еды", ft_strlen("Неверное время еды"));
	else if ((init_data->sleep = ft_atoi(argv[4])) < 60)
		write(2, "Неверное время сна", ft_strlen("Неверное время сна"));
	else if (argc == 6 && (init_data->count_eat = ft_atoi(argv[5])) < 0)
		write(2, "Неверное количество блюд",
		ft_strlen("Неверное количество блюд"));
	else
		return (init_philo(init_data));
	return (1);
}

int		main(int argc, char **argv)
{
	t_init_data	init_data;
	int			i;

	memset(&init_data, 0, sizeof(t_init_data));
	if ((i = init_input(argc, argv, &init_data)))
		return (error(&init_data, i));
	if ((i = init_process(&init_data)))
		return (error(&init_data, 3));
	clean_all(&init_data, -1);
	return (0);
}

void	*enderf(void *args)
{
	t_init_data	*init_data;

	init_data = (t_init_data*)args;
	sem_wait(init_data->fin);
	init_data->finish = 0;
	return (NULL);
}

void	*checkerf(void *args)
{
	int			j;
	t_init_data	*init_data;

	init_data = (t_init_data*)args;
	j = 0;
	while (1)
	{
		sem_wait(init_data->sem);
		j++;
		if (j == init_data->kol)
			break ;
	}
	if (init_data->finish == 1)
		show(init_data->philo, 6);
	init_data->finish = 0;
	return (NULL);
}

int		init_process(t_init_data *init_data)
{
	int			i;
	pthread_t	ender;
	pthread_t	checker;

	init_data->time = timer();
	i = -1;
	init_data->finish = 1;
	if (pthread_create(&ender, NULL, enderf, init_data))
		return (1);
	if (pthread_create(&checker, NULL, checkerf, init_data))
		return (1);
	pthread_detach(ender);
	pthread_detach(checker);
	while (++i < init_data->kol)
	{
		init_data->philo[i].time = timer();
		if (!(init_data->philo[i].pid_p = fork()))
			start_process(&init_data->philo[i]);
		else if (init_data->philo[i].pid_p < 0)
			return (1);
		usleep(100);
	}
	while (init_data->finish == 1)
		usleep(100);
	return (0);
}
