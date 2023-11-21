/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frrusso <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 13:01:23 by frrusso           #+#    #+#             */
/*   Updated: 2023/11/21 13:01:25 by frrusso          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ve_window.hpp>

int	main(void) {
	VeWindow	veWindow;

	try {
		veWindow.run();
	} catch (const std::exception& e) {
		std::cerr << RED << e.what() << ENDL;
		return (1);
	}
	return (0);
}
