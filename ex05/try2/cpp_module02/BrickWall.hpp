#ifndef BRICKWALL_HPP
# define BRICKWALL_HPP

# include <string>
# include <iostream>
# include "ATarget.hpp"

class BrickWall : public ATarget
{
public:
	BrickWall();
	BrickWall(const BrickWall& src);
	~BrickWall();

	BrickWall&	operator=(const BrickWall&src);
	
	ATarget*	clone() const;
};

#endif