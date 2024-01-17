#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include <string>
# include <iostream>
# include "ASpell.hpp"
# include <map>

class SpellBook
{
private:
	std::map<std::string, ASpell*>	_spellbook;

	SpellBook(const SpellBook& src);
	SpellBook&	operator=(const SpellBook&src);
public:
	SpellBook();
	~SpellBook();

	void	learnSpell(ASpell* spell);
	void	forgetSpell(const std::string& spell);
	ASpell*	createSpell(const std::string& spell);
};

#endif