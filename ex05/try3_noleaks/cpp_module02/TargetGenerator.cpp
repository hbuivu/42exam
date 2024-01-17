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
	for (std::map<std::string, ATarget*>::iterator it = _targetbook.begin(); it != _targetbook.end(); it++)
		delete it->second;
}

TargetGenerator& TargetGenerator::operator=(const TargetGenerator& src)
{
	(void)src;
	return *this;
}

void		TargetGenerator::learnTargetType(ATarget* target)
{
	if (target)
		_targetbook[target->getType()] = target->clone();
}

void		TargetGenerator::forgetTargetType(const std::string& target)
{
	std::map<std::string, ATarget*>::iterator delIt = _targetbook.find(target);
	if (delIt != _targetbook.end())
	{
		delete delIt->second;
		_targetbook.erase(delIt);
	}
}

ATarget*	TargetGenerator::createTarget(const std::string& target)
{
	if (_targetbook.find(target) != _targetbook.end())
		return (_targetbook[target]->clone());
	return NULL;
}