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

	std::string determineTargetDirectory();
	COPY_ITEMS determineCopyItems();
	COPY_SELECTIVITY determineCopySelectivity();
	NAME_CONFLICT_RESOLUTION determineNameConflictResolution();

	std::deque<int> selectFiles();
	void displayFiles(std::vector<int>&, std::deque<int>&, std::ostringstream&);

public:
	Copy(std::filesystem::path&);
	void copy();
};
#endif

