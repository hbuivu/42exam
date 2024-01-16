#include "Polymorph.hpp"

Polymorph::Polymorph()
	:	ASpell("Polymorph", "turned into a critter")
{
}

Polymorph::Polymorph(const Polymorph& src)
	:	ASpell(src)
{
}

Polymorph::~Polymorph()
{
}

Polymorph&	Polymorph::operator=(const Polymorph& src)
{
	(void)src;
	return *this;
}

ASpell*	Polymorph::clone() const
{
	Polymorph*	newPolymorph = new Polymorph;
	return newPolymorph;
}