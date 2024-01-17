#ifndef FWOOSH_HPP
# define FWOOSH_HPP

# include <iostream>
# include <string>
# include "ASpell.hpp"

class Fwoosh : public ASpell
{
private:
public:
	Fwoosh();
	Fwoosh(const Fwoosh& src);
	~Fwoosh();
	
	Fwoosh& operator=(const Fwoosh&src);

	ASpell*	clone() const;
};

#endif