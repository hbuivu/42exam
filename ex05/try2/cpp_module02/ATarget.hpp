#ifndef ATARGET_HPP
# define ATARGET_HPP

# include <string>
# include <iostream>
# include "ASpell.hpp"

class ASpell;

class ATarget
{
protected:
	std::string	_type;
public:
	ATarget();
	ATarget(const std::string& type);
	ATarget(const ATarget& src);
	virtual ~ATarget();

	ATarget&	operator=(const ATarget&src);

	const std::string&	getType() const;
	virtual ATarget*	clone() const = 0;
	void				getHitBySpell(const ASpell& spell) const;
};

#endif