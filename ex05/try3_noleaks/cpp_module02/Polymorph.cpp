#include "Polymorph.hpp"

Polymorph::Polymorph()
	:	ASpell("Polymorph", "turned into a critter")
{
}

Polymorph::Polymorph(const Polymorph& src)
	:	ASpell(src)
{
	(void)src;
}

Polymorph::~Polymorph()
{
}

Polymorph& Polymorph::operator=(const Polymorph& src)
{
	(void)src;
	return *this;
}

ASpell*	Polymorph::clone() const
{
	return (new Polymorph);
}