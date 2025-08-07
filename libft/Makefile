# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/04 12:00:00 by mohchams          #+#    #+#              #
#    Updated: 2025/04/04 12:00:00 by mohchams         ###   .fr                #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
    ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c \
    ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c \
    ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c \
    ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c \
    ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c \
    ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c \
    ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
    ft_lstlast.c ft_lstnew.c ft_lstsize.c ft_strspn.c ft_strtok.c\
    get_next_line.c get_next_line_bonus.c \
    ft_printf.c ft_putchar_printf.c ft_putstr_printf.c \
    ft_putnbr_hexa_printf.c ft_putnbr_unsigned.c ft_putnbr_printf.c \
    ft_percent_printf.c ft_putptr_printf.c ft_isspace.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all