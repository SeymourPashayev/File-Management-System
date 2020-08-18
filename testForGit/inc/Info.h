#ifndef INFO_H
#define INFO_H

#include "FileManagementUnit.h"
#include <filesystem>
#include <deque>

class Info : public FileManagementUnit
{
private:
	std::string continueDisplay{ "yes" };
public:
	Info(std::filesystem::path path);
	void info();
};
#endif

