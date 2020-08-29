#ifndef FIND_H
#define FIND_H

#include "FileManagementUnit.h"
#include <string>
#include <iostream>
#include <bitset>
#include <map>

class Find : public FileManagementUnit
{
private:
	enum class DATE_PRECISION { DAY = 1, HOUR, MINUTE };
	static const std::map<std::string, int> monthToInt;

	std::bitset<3> searchParameters; //search parameters: date, substring of the name, extension

	DATE_PRECISION datePrecision;
	std::string targetDate;
	int targetHour;

	std::string targetNameSubstr;
	std::string targetExt;


	std::bitset<3> determineSearchParameters();
	DATE_PRECISION determineDatePrecision();
	std::string determineTargetDate();
	int determineTargetHour();
    
	std::string determineTargetNameSubstr();
	std::string determineTargetExtension();

	bool dateMatch(const std::filesystem::directory_entry&);
	bool nameMatch(const std::filesystem::directory_entry&);
	bool extensionMatch(const std::filesystem::directory_entry&);

	void secondaryAction();

public:
	Find(const std::filesystem::path&);
	virtual ~Find() {
		std::cout << "File matches:" << std::endl;

		for (auto fileNum : FileManagementUnit::fileLst) {
			std::cout << fileNum << " ";
		}

		std::cout << std::endl;
	}
	void find();

};
#endif
