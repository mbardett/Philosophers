/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbardett <mbardett@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/16 18:29:18 by mbardett          #+#    #+#             */
/*   Updated: 2022/10/22 18:01:57 by mbardett         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//returns time in MILLIsecs
long long	ft_get_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return ((timeval.tv_sec * 1000) + (timeval.tv_usec / 1000));
}

//wait(usleep or pthreadjoin?)
void	ft_wait(long long wait)
{
	long long	start_time;

	start_time = ft_get_time();
	usleep(wait * 1000 - 20000);
	while (ft_get_time() < start_time + wait)
		continue ;
}

//print time, philo->id and change of philo status
void	ft_logs(t_philo *ph, char *str)
{
	if (ft_check_state(0, ph))
	{
		pthread_mutex_lock(&ph->rules->write);
		printf("%lld %d %s \n", (long long)ft_get_time() - ph->rules->strt_time,
			ph->id, str);
		pthread_mutex_unlock(&ph->rules->write);
	}
}

//check philostate:
//checks whether the philo still needs to eat or has finished, returns 1 or 0
int	ft_check_state(int flag, t_philo *ph)
{
	int	i;

	i = 1;
	if (flag == 0)
	{
		pthread_mutex_lock(&ph->rules->uber_lock);
		i = ph->rules->must_run;
		pthread_mutex_unlock(&ph->rules->uber_lock);
	}
	else if (flag == 1)
	{
		pthread_mutex_lock(&ph->philo_lock);
		i = ph->must_run;
		pthread_mutex_unlock(&ph->philo_lock);
	}
	return (i);
}

//exit(pthread_destroy and free)
void	ft_exit(t_rules *rules)
{
	int		i;
	t_philo	*philo;

	philo = rules->philos;
	i = 0;
	while (i < rules->nbr_philos)
	{
		pthread_join(philo[i].philo_thread, NULL);
		i++;
	}
	i = 0;
	while (i < rules->nbr_philos)
	{
		pthread_mutex_destroy(&rules->forks[i].fork_mutex);
		i++;
	}
	pthread_mutex_destroy(&rules->uber_lock);
	pthread_mutex_destroy(&rules->write);
	free(rules->forks);
	free(philo);
	exit(0);
}
