#ifndef POLYMORPH_HPP
# define POLYMORPH_HPP

# include <iostream>
# include <string>
# include "ASpell.hpp"

class Polymorph : public ASpell
{
private:
public:
	Polymorph();
	Polymorph(const Polymorph& src);
	~Polymorph();
	
	Polymorph& operator=(const Polymorph&src);

	ASpell*	clone() const;
};

#endif