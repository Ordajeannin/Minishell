/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:51:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/29 11:33:00 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	char	*dup;

	if (!s)
		return (ft_strdup(""));
	i = ft_strlen(s) + 1;
	dup = ft_calloc(i, sizeof(char));
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, s, i);
	return (dup);
}
