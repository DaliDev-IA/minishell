/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 12:00:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/06 13:01:57 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h> // Pour ft_printf
# include <unistd.h>
# include <stddef.h>
# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>

// Pour get_next_line
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
# define FD_MAX 2000 // Pour get_next_line_bonus

// Structure de libft
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// Prototypes de libft (tous ceux de libft.h)
int		ft_atoi(const char *str);
void	ft_bzero(void *s, size_t n);
int		ft_isalnum(int c);
int		ft_isprint(int c);
int		ft_isdigit(int c);
int		ft_isspace(int c);
int		ft_isascii(int c);
int		ft_isalpha(int c);
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char	*ft_strrchr(const char *s, int c);
char	*ft_strchr(const char *s, int c);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
size_t	ft_strspn(const char *s, const char *accept);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dst, const char *src, size_t siz);
size_t	ft_strlcat(char *dst, const char *src, size_t siz);
char	*ft_strnstr(const char *s1, const char *s2, size_t len);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtok(char *str, const char *delim);
void	ft_putstr_fd(char *s, int fd);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	*ft_calloc(size_t num, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	*ft_strdup(char *src);
char	*ft_itoa(int nb);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));

// Prototypes de get_next_line (sans doublons)
char	*nouv_stat(char *stat);
char	*extract_line(char *stat_temp);
char	*stat_temporaire(int fd, char *stat);
char	*get_next_line(int fd);

// Prototypes de ft_printf (sans doublons)
int		check_base(char *base);
int		ft_percent_printf(void);
int		ft_putnbr_unsigned(unsigned long long n);
int		ft_putstr_printf(char *s);
int		ft_putchar_printf(char c);
int		ft_putnbr_printf(int n);
int		ft_putnbr_hexa_printf(unsigned long long nb, char *base);
int		convert_str(char c, va_list args);
int		ft_printf(const char *str, ...);
int		ft_putptr_printf(void *ptr);

#endif