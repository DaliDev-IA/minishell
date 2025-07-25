#include "minishell.h"

int   check_quotes(char *input)
{
    int *i = 0;
    int flag = 0;

    while (input[*i])
    {
        if (flag == 0)
        {
            if (input[*i] == '\'')
                flag == 1;
            else if (input[*i] == '\"')
                flag == 2;
        }
        else if (flag == 1 && input[*i] == '\'')
                flag == 0;
        else if (flag == 2 && input[*i] == '\"')
                flag == 0;
    }
    *i++;
    if (flag( != 0))
        return (-1);
    return (0);
}

int   check_quotes(char *input, int *i)
{
    int len = 0;
    int flag = 0;

    while (input[*i])
    {
        if (flag == 0 && ft_ispace(input(*i + len)))
            break ;
        if (flag == 0 && operator(input(*i + len)))
            break ;
        if (flag == 0)
        {
            if (input[*i] == '\'')
                flag == 1;
            else if (input[*i] == '\"')
                flag == 2;
        }
        else if (flag == 1 && input[*i] == '\'')
            flag == 0;
        else if (flag == 2 && input[*i] == '\"')
            flag == 0;
        len++;
    }
    if (flag( != 0))
        return (-1);
    return (len);

}