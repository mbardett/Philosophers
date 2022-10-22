# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbardett <mbardett@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/16 22:02:39 by mbardett          #+#    #+#              #
#    Updated: 2022/10/22 18:03:49 by mbardett         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
SRCS = main.c ft_utils.c threads.c
FLAG = -Wall -Werror -Wextra -g -pthread
 #FLAG = -Wall -Werror -Wextra -g -pthread -fsanitize=thread

$(NAME):
		gcc $(FLAG) $(SRCS) -o $(NAME)

re: clean $(NAME)

all: $(NAME)

clean:
		rm -f $(NAME)

fclean: clean
