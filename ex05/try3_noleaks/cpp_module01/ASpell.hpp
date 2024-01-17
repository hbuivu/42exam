#ifndef ASPELL_HPP
# define ASPELL_HPP

# include <iostream>
# include <string>
# include "ATarget.hpp"

class ATarget;

class ASpell
{
protected:
	std::string	_name;
	std::string	_effects;

public:
	ASpell();
	ASpell(const std::string& name, const std::string& effects);
	ASpell(const ASpell& src);
	virtual ~ASpell();
	
	ASpell& operator=(const ASpell&src);

	const std::string&	getName() const;
	const std::string&	getEffects() const;
	virtual ASpell*		clone() const = 0;
	void				launchSpell(const ATarget& target) const;
};

#endif