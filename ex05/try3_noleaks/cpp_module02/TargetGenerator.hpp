#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

# include <iostream>
# include <string>
# include <map>
# include "ATarget.hpp"

class TargetGenerator
{
private:
	std::map<std::string, ATarget*>	_targetbook;

	TargetGenerator(const TargetGenerator& src);
	TargetGenerator& operator=(const TargetGenerator&src);
public:
	TargetGenerator();
	~TargetGenerator();

	void		learnTargetType(ATarget* target);
	void		forgetTargetType(const std::string& target);
	ATarget*	createTarget(const std::string& target);
	
};

#endif