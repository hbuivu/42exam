#ifndef TARGETGENERATOR_HPP
# define TARGETGENERATOR_HPP

# include <string>
# include <iostream>
# include "ATarget.hpp"
# include <map>

class TargetGenerator
{
private:
	std::map<std::string, ATarget*>	_targetBook;

	TargetGenerator(const TargetGenerator& src);
	TargetGenerator&	operator=(const TargetGenerator& src);
public:
	TargetGenerator();
	~TargetGenerator();

	void		learnTargetType(ATarget *target);
	void		forgetTargetType(const std::string& target);
	ATarget*	createTarget(const std::string& target);
};

#endif