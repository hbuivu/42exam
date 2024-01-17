#include "BrickWall.hpp"

BrickWall::BrickWall()
	:	ATarget("Inconspicuous Red-brick Wall")
{
}

BrickWall::BrickWall(const BrickWall& src)
	:	ATarget(src)
{
	(void)src;
}

BrickWall::~BrickWall()
{
}

BrickWall&	BrickWall::operator=(const BrickWall& src)
{
	(void)src;
	return *this;
}

ATarget*	BrickWall::clone() const
{
	BrickWall* newWall = new BrickWall;
	return newWall;
}

