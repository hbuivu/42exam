#ifndef POLYMORPH_HPP
# define POLYMORPH_HPP

# include <string>
# include <iostream>
# include "ASpell.hpp"

class Polymorph : public ASpell
{
public:
	Polymorph();
	Polymorph(const Polymorph& src);
	~Polymorph();

	Polymorph&	operator=(const Polymorph&src);

	ASpell*	clone() const;
};

#endif