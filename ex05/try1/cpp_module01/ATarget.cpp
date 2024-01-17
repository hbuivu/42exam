#include "ATarget.hpp"

ATarget::ATarget()
{
}

ATarget::ATarget(const std::string& type)
	:	_type(type)
{
}

ATarget::ATarget(const ATarget& src)
{
	(void)src;
}

ATarget::~ATarget()
{
}

ATarget&	ATarget::operator=(const ATarget& src)
{
	(void)src;
	return *this;
}

void	ATarget::getHitBySpell(const ASpell& spell) const
{
	std::cout << _type << " has been " << spell.getEffects() << "!\n";
}