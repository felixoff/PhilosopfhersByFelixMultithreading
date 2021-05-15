/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sjennett <sjennett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 16:16:08 by sjennett          #+#    #+#             */
/*   Updated: 2021/04/20 23:52:36 by sjennett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <semaphore.h>
# include <pthread.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <stdio.h>

int	g_flag;

typedef struct s_philo
{
	int					index;
	int					count_eat;
	int					time;
	int					is_death;
	char				*sem;
	pid_t				pid_p;
	struct s_init_data	*init_data;
}					t_philo;

typedef struct s_init_data
{
	int		kol;
	int		sleep;
	int		eat;
	int		die;
	int		count_eat;
	int		time;
	int		finish;
	t_philo	*philo;
	sem_t	*fork;
	sem_t	*sem;
	sem_t	*vivod;
	sem_t	*fin;
	pid_t	pid_i;
}					t_init_data;

void				clean_all(t_init_data *init_data, int i);
int					error(t_init_data *init_data, int i);
void				ft_putchar_fd(int fd, char c);
void				ft_putnbr_fd(int fd, int number);
char				*ft_name(char *str, int j);
int					ft_strlen(char *str);
int					atoi2(char *str);
int					ft_atoi(char *str);
int					init_philo(t_init_data *init_data);
int					init_input(int argc, char **argv, t_init_data *init_data);
int					main(int argc, char **argv);
void				take_fork_on(t_philo *philo);
void				take_fork_off(t_philo *philo);
void				eat(t_philo *philo);
void				ft_sleep(t_philo *philo);
void				*enderf(void *args);
int					timer(void);
int					init_process (t_init_data *init_data);
int					count_eat(void *args);
void				*dead_philo(void *args);
int					start_process(t_philo *data);
char				*output(int i);
void				show(t_philo *philo, int i);

#endif
