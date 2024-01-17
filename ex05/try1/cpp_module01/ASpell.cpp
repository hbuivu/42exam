#include "ASpell.hpp"

ASpell::ASpell()
{
}

ASpell::ASpell(const std::string& name, const std::string& effects)
	:	_name(name),
		_effects(effects)
{
}

ASpell::ASpell(const ASpell& src)
{
	(void)src;
}

ASpell::~ASpell()
{
}

ASpell&	ASpell::operator=(const ASpell& src)
{
	(void)src;
	return *this;
}

const std::string&	ASpell::getName() const
{
	return _name;
}

const std::string&	ASpell::getEffects() const
{
	return _effects;
}

void	ASpell::launchSpell(const ATarget& target) const
{
	target.getHitBySpell(*this);
}