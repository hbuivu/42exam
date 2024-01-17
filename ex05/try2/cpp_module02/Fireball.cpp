#include "Fireball.hpp"

Fireball::Fireball()
	:	ASpell("Fireball", "burnt to a crisp")
{	
}

Fireball::Fireball(const Fireball& src)
	:	ASpell(src)
{
	(void)src;
}

Fireball::~Fireball()
{
}

Fireball& Fireball::operator=(const Fireball&src)
{
	(void)src;
	return *this;
}

ASpell*	Fireball::clone() const
{
	return (new Fireball);
}