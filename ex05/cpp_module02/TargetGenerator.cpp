#include "TargetGenerator.hpp"

TargetGenerator::TargetGenerator()
{
}

TargetGenerator::TargetGenerator(const TargetGenerator& src)
{
	(void)src;
}

TargetGenerator::~TargetGenerator()
{
	for(std::map<std::string, ATarget*>::iterator it = _targetBook.begin(); it != _targetBook.end(); it++)
		delete it->second;
}

TargetGenerator&	TargetGenerator::operator=(const TargetGenerator& src)
{
	(void)src;
	return *this;
}

void	TargetGenerator::learnTargetType(ATarget *target)
{
	if (target)
		_targetBook[target->getType()] = target->clone();
}

void	TargetGenerator::forgetTargetType(const std::string& target)
{
	std::map<std::string, ATarget*>::iterator delIt = _targetBook.find(target);
	if (delIt != _targetBook.end())
		_targetBook.erase(delIt);
}

ATarget*	TargetGenerator::createTarget(const std::string& target)
{
	if (_targetBook.find(target) != _targetBook.end())
		return _targetBook[target]->clone();
	return NULL;
}

