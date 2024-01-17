#ifndef SPELLBOOK_HPP
# define SPELLBOOK_HPP

# include <iostream>
# include <string>
# include <map>
# include "ASpell.hpp"

class SpellBook
{
private:
	std::map<std::string, ASpell*>	_spellbook;

	SpellBook(const SpellBook& src);
	SpellBook& operator=(const SpellBook&src);
public:
	SpellBook();
	~SpellBook();

	void	learnSpell(ASpell* spell);
	void	forgetSpell(const std::string& spell);
	ASpell*	createSpell(const std::string& spell);	
};

#endif