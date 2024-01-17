# include "Fwoosh.hpp"

Fwoosh::Fwoosh()
	:	ASpell("Fwoosh", "fwooshed")
{	
}

Fwoosh::Fwoosh(const Fwoosh& src)
	:	ASpell(src)
{
	(void)src;
}

Fwoosh::~Fwoosh()
{
}

Fwoosh& Fwoosh::operator=(const Fwoosh&src)
{
	(void)src;
	return *this;
}

ASpell*	Fwoosh::clone() const
{
	return (new Fwoosh);
}