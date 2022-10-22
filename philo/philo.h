/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbardett <mbardett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 18:14:22 by mbardett          #+#    #+#             */
/*   Updated: 2022/10/21 19:51:38 by mbardett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_forks
{
	int				fork_status;
	pthread_mutex_t	fork_mutex;
}	t_forks;

typedef struct s_philo
{
	int					id;
	int					nmeals;
	int					must_run;
	long long			last_meal;
	long long			last_action;
	pthread_t			philo_thread;
	pthread_mutex_t		philo_lock;
	pthread_mutex_t		rfork;
	pthread_mutex_t		lfork;
	struct s_rules		*rules;
}	t_philo;

typedef struct s_rules
{
	int					nbr_philos;
	int					nbr_meals;
	int					must_run;
	long long			tm_die;
	long long			tm_eat;
	long long			tm_sleep;
	long long			strt_time;
	t_forks				*forks;
	t_philo				*philos;
	pthread_mutex_t		write;
	pthread_mutex_t		uber_lock;
}	t_rules;

//main.c
int			ft_atoi(char *str);
int			ft_nums_only(int argc, char **argv);
void		ft_init_aux(t_rules *rules);
void		ft_init(t_rules *rules, char **argv);
//threads.c
int			monitor_aux(t_rules *rules, t_philo *philo, int *mealrounds);
void		ft_monitor(t_rules *rules);
void		ft_thrroutine_aux(t_philo *ph);
void		*ft_thrroutine(void *ph);
void		ft_multithreads(t_rules *rules);
//ft_utils.c
long long	ft_get_time(void);

void		ft_logs(t_philo *ph, char *str);
//ft_utils2.c
int			ft_check_state(int flag, t_philo *ph);
void		ft_exit(t_rules *rules);
void		ft_wait(long long wait);

#endif
