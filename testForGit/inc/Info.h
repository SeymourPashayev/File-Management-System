#ifndef INFO_H
#define INFO_H

#include <filesystem>
#include <deque>

class Info
{

private:
	enum class INFO_ITEMS { ALL = 1, FILES, DIRECTORIES };

	INFO_ITEMS infoItems;
	std::string continueDisplay{ "yes" };
	std::filesystem::path path;

	INFO_ITEMS determineInfoItems();
public:
	Info(std::filesystem::path path);
	void info();
};
#endif

