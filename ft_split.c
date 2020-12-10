/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfabri <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 07:54:48 by mfabri            #+#    #+#             */
/*   Updated: 2020/01/13 11:27:16 by mfabri           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>

static int	ft_ischar(char a, char b)
{
	return (a == b ? 1 : 0);
}

int			get_nbr(char *s, char c)
{
	int i;
	int word_nbr;

	i = 0;
	word_nbr = 0;
	while (s[i])
	{
		if (!ft_ischar(c, s[i]) &&
				(ft_ischar(c, s[i + 1]) || s[i + 1] == '\0'))
			word_nbr++;
		i++;
	}
	return (word_nbr);
}

char		*malloc_word(char *s, char c)
{
	int		i;
	char	*word;

	i = 0;
	while (s[i] && !ft_ischar(c, s[i]))
		i++;
	if (!(word = (char *)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	i = 0;
	while (s[i] && !ft_ischar(c, s[i]))
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

char		**ft_split(char const *s, char c)
{
	int		i;
	char	**array;

	if (!s || !c)
		return (NULL);
	if (!(array = (char **)malloc(sizeof(char*) * (get_nbr((char *)s, c) + 1))))
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s && ft_ischar(c, *s))
			s++;
		if (*s && !ft_ischar(c, *s))
		{
			array[i] = malloc_word((char *)s, c);
			i++;
			while (*s && !ft_ischar(c, *s))
				s++;
		}
	}
	array[i] = NULL;
	return (array);
}
