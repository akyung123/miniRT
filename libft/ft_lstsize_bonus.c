/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akkim <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 04:24:25 by akkim             #+#    #+#             */
/*   Updated: 2025/04/14 04:24:26 by akkim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int			size;
	t_list		*temp;

	size = 0;
	temp = lst;
	if (!lst)
		return (0);
	while (temp->next)
	{
		size++;
		temp = temp->next;
	}
	if (temp->next == NULL)
		size++;
	return (size);
}
