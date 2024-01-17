#ifndef BRICKWALL_HPP
# define BRICKWALL_HPP

# include <iostream>
# include <string>
# include "ATarget.hpp"

class BrickWall : public ATarget
{
private:
public:
	BrickWall();
	BrickWall(const BrickWall& src);
	~BrickWall();
	
	BrickWall& operator=(const BrickWall&src);

	ATarget*	clone() const;
};

#endif