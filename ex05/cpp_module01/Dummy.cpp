#include "Dummy.hpp"

Dummy::Dummy()
	:	ATarget("Target Practice Dummy")
{
}

Dummy::Dummy(const Dummy& src)
	:	ATarget(src)
{
	(void)src;
}

Dummy::~Dummy()
{
}

Dummy&	Dummy::operator=(const Dummy& src)
{
	(void)src;
	return *this;
}

ATarget*	Dummy::clone() const
{
	Dummy* newDummy = new Dummy;
	return newDummy;
}

