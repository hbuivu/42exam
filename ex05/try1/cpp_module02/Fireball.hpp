#ifndef FIREBALL_HPP
# define FIREBALL_HPP

# include <string>
# include <iostream>
# include "ASpell.hpp"

class Fireball : public ASpell
{
public:
	Fireball();
	Fireball(const Fireball& src);
	~Fireball();

	Fireball&	operator=(const Fireball& src);

	ASpell*	clone() const;
};

#endif