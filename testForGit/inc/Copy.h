#ifndef COPY_H
#define COPY_H

#include <filesystem>
#include <string>
#include <deque>

class Copy
{
private:
	enum class COPY_ITEMS { ALL = 1, FILES, DIRECTORIES };
	enum class COPY_SELECTIVITY { ALL = 1, CUSTOM };
	enum class NAME_CONFLICT_RESOLUTION { KEEP = 1, OVERWRITE };

	COPY_ITEMS copyItems;
	COPY_SELECTIVITY copySelectivity;
	NAME_CONFLICT_RESOLUTION nameConflictResolution;
	std::filesystem::path pathFrom;
	std::filesystem::path pathTo;

	std::filesystem::path determineTargetDirectory();
	COPY_ITEMS determineCopyItems();
	COPY_SELECTIVITY determineCopySelectivity();
	NAME_CONFLICT_RESOLUTION determineNameConflictResolution();

	std::deque<int> selectItems();
	void displayItems(std::vector<int>&, std::deque<int>&, std::ostringstream&);

	void copyFiles(std::filesystem::copy_options&);
	void copyDirectories(std::filesystem::copy_options&);

	std::string regexStrParser(std::string);

public:
	Copy(std::filesystem::path&);
	void copy();
};
#endif

