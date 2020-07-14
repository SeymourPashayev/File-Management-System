#ifndef DELETE_H
#define DELETE_H

#include <filesystem>
#include <string>
#include <deque>

class Delete
{
private:
	enum class DELETE_ITEMS { ALL = 1, FILES, DIRECTORIES };
	enum class DELETE_SELECTIVITY { ALL = 1, CUSTOM };

	DELETE_ITEMS deleteItems;
	DELETE_SELECTIVITY deleteSelectivity;
	std::filesystem::path path;

	DELETE_ITEMS  determineDeleteItems();
	DELETE_SELECTIVITY determineDeleteSelectivity();

	std::deque<long long> selectItems();
	void displayItems(std::vector<long long>&, std::deque<long long>&, std::ostringstream&);

	void deleteFiles();
	void deleteDirectories();

public:
	Delete(std::filesystem::path& path);
	void deletion();
};
#endif

