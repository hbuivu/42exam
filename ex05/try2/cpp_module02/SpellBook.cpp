# include "SpellBook.hpp"

SpellBook::SpellBook()
{	
}

SpellBook::SpellBook(const SpellBook& src)
{
	(void)src;
}

SpellBook::~SpellBook()
{
	for (std::map<std::string, ASpell*>::iterator it = _spellbook.begin(); it != _spellbook.end(); it++)
		delete it->second;
}

SpellBook& SpellBook::operator=(const SpellBook&src)
{
	(void)src;
	return *this;
}

void	SpellBook::learnSpell(ASpell* spell)
{
	if (spell)
		_spellbook[spell->getName()] = spell->clone();
}

void	SpellBook::forgetSpell(const std::string& spell)
{
	std::map<std::string, ASpell*>::iterator delIt = _spellbook.find(spell);
	if (delIt != _spellbook.end())
		_spellbook.erase(delIt);
}

ASpell*	SpellBook::createSpell(const std::string& spell)
{
	if (_spellbook.find(spell) != _spellbook.end())
		return (_spellbook[spell]->clone());
	return (NULL);
}
