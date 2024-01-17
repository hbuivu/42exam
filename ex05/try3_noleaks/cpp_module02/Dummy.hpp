#ifndef DUMMY_HPP
# define DUMMY_HPP

# include <iostream>
# include <string>
# include "ATarget.hpp"

class Dummy : public ATarget
{
private:
public:
	Dummy();
	Dummy(const Dummy& src);
	~Dummy();
	
	Dummy& operator=(const Dummy&src);

	ATarget*	clone() const;
};

#endif