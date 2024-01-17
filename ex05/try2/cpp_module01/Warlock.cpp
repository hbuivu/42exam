# include "Warlock.hpp"

Warlock::Warlock()
{	
}

Warlock::Warlock(const std::string& name, const std::string& title)
	:	_name(name),
		_title(title)
{	
	std::cout << _name << ": This looks like another boring day.\n";
}

Warlock::Warlock(const Warlock& src)
{
	(void)src;
}

Warlock::~Warlock()
{
	for (std::map<std::string, ASpell*>::iterator it = _spellbook.begin(); it != _spellbook.end(); it++)
		delete it->second;
	std::cout << _name << ": My job here is done!\n";
}

Warlock& Warlock::operator=(const Warlock&src)
{
	(void)src;
	return *this;
}

const std::string&	Warlock::getName() const
{
	return _name;
}

const std::string&	Warlock::getTitle() const
{
	return _title;
}

void				Warlock::setTitle(const std::string& title)
{
	_title = title;
}

void				Warlock::introduce() const
{
	std::cout << _name << ": I am " << _name << ", " << _title << "!\n";
}

void	Warlock::learnSpell(ASpell* spell)
{
	if (spell)
		_spellbook[spell->getName()] = spell->clone();
}

void	Warlock::forgetSpell(std::string spell)
{
	std::map<std::string, ASpell*>::iterator delIt = _spellbook.find(spell);
	if (delIt != _spellbook.end())
		_spellbook.erase(delIt);
}

void	Warlock::launchSpell(std::string spell, const ATarget& target) //note if we want to make this const, launchSpell must be const too
{
	if (_spellbook.find(spell) != _spellbook.end())
		_spellbook[spell]->launchSpell(target);
}