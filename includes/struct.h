/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ji-kim <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 15:23:06 by ji-kim            #+#    #+#             */
/*   Updated: 2021/07/05 15:23:08 by ji-kim           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

typedef struct		s_cmd
{
	char			*cmd;
	char			**arg;				// command + options
	int				flag;					// represent |(1) or ;(2)
	int				redin;				// <
	int				redout;				// >
	int				append;				// >>
	int				delimit;			// <<
	char			**parse;			// tmp from parse_tmp
	int				ret;					// skip executing cmd
	struct s_cmd	*next;
}					t_cmd;

#endif
