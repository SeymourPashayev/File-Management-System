#include "Find.h"
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <regex>
#include <chrono>
#include "Rename.h"
#include "Delete.h"
#include "Copy.h"

const std::map<std::string, int> Find::monthToInt{ {"Jan", 1}, {"Feb", 2}, {"Mar", 3}, {"Apr", 4}, {"May", 5}, {"Jun", 6},
	{"Jul", 7}, {"Aug", 8}, {"Sep", 9}, {"Oct", 10}, {"Nov", 11}, {"Dec", 12} };

Find::Find(const std::filesystem::path& path) : FileManagementUnit{ path, 1 }, searchParameters{ determineSearchParameters() }{
	if (searchParameters[0]) {
		datePrecision = determineDatePrecision();
		targetDate = determineTargetDate();
		targetHour = determineTargetHour();
	}

	if (searchParameters[1]) {
		targetNameSubstr = determineTargetNameSubstr();
	}

	if (searchParameters[2]) {
		targetExt = determineTargetExtension();
	}
}

void Find::find() {
	long long fileNum{ 1 };

	for (const auto& dirEntry : std::filesystem::directory_iterator{ FileManagementUnit::getPath() }) {
		bool fileMatch{ true };
		std::bitset<3> matches;

		if (searchParameters[0] && dateMatch(dirEntry)) {
			matches[0] = true;
		}

		if (searchParameters[1] && nameMatch(dirEntry)) {
			matches[1] = true;
		}

		if (searchParameters[2] && extensionMatch(dirEntry)) {
			matches[2] = true;
		}

		for (int i{ 0 }; i < matches.size(); ++i) {
			if (searchParameters[i] && !matches[i]) {
				fileMatch = false;
			}
		}

		if (searchParameters.any() && fileMatch) {
			FileManagementUnit::fileLst.push_back(fileNum);
		}

		++fileNum;
	}

	secondaryAction();
}

/*dateMatch() steps:
1) File modification time is extracted from the system
2) Only date is extracted from result of 1)
3) Result of 2) is compared to result of 1)
*/

bool Find::dateMatch(const std::filesystem::directory_entry& dirEntry) {
	//extraction of file modification date from system clock starts here
	std::time_t lastModification{ std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(dirEntry.last_write_time()
				   - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now())) };

	struct tm lastModificationStrct;
	localtime_s(&lastModificationStrct, &lastModification);

	const size_t BUFFER_SIZE{ 32 };
	char timeBuff[BUFFER_SIZE];

	asctime_s(timeBuff, sizeof(timeBuff), &lastModificationStrct);

	std::string timeBuffStr{ timeBuff };
	//extraction of file modification date from system clock ends here

	//extraction of dd/mm/yyyy from timeBuffStr starts here
	std::string fileDate{ timeBuffStr.substr(8, 2) + "/" };
	std::string monthStr{ timeBuffStr.substr(4, 3) };

	for (const auto& monthPair : monthToInt) {
		if (monthPair.first == monthStr) {
			fileDate += (monthPair.second < 10 ? + "0" : "" ) + std::to_string(monthPair.second) + "/";
		}
	}

	fileDate += timeBuffStr.substr(20, 4);
	//extraction of dd/mm/yyyy from timeBuffStr ends here

	//date matching starts here
	int fileHour{ std::stoi(timeBuffStr.substr(11, 2)) };

	std::bitset<2> dateFlags;

	dateFlags[0] = (targetDate == fileDate);
	dateFlags[1] = (targetHour == fileHour);
	//date matching ends here

	return ((datePrecision == Find::DATE_PRECISION::DAY && dateFlags.any()) || (datePrecision == Find::DATE_PRECISION::HOUR && dateFlags.all()));
}

bool Find::nameMatch(const std::filesystem::directory_entry& dirEntry) {
	std::regex targetReg{ "(" + targetNameSubstr + ")" };
	std::string pathStr{ dirEntry.path().string() };
	std::sregex_iterator currentMatch{ pathStr.begin(), pathStr.end(), targetReg };

	return currentMatch != std::sregex_iterator{};
}

bool Find::extensionMatch(const std::filesystem::directory_entry& dirEntry){
	return ((dirEntry.is_directory() && targetExt == "Folder") || (dirEntry.is_regular_file() && targetExt == dirEntry.path().extension()));
}

void Find::secondaryAction() {
	std::cout << "Select an action you would like to perform on the matched files:" << std::endl;
	std::cout << "1 - No action" << std::endl;
	std::cout << "2 - Rename the files." << std::endl;
	std::cout << "3 - Delete the files." << std::endl;
	std::cout << "4 - Copy the files." << std::endl;

	int choice{};
	std::cin >> choice;

	if (!std::cin.good()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while (choice < 1 || choice > 4) {
		std::cerr << std::endl << "Invalid input. Enter an integer 1-4 ." << std::endl;
		std::cin >> choice;

		if (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}

	switch (choice) {
	case 2: {
		//Rename integration here
		break;
	}
	case 3: {
		Delete deleteObj{ FileManagementUnit::path, static_cast<int>(FileManagementUnit::items), 
			static_cast<int>(FileManagementUnit::selectivity), FileManagementUnit::fileLst };

		deleteObj.deletion();
		break;
	}
	case 4: {
		Copy copyObj{ FileManagementUnit::path, static_cast<int>(FileManagementUnit::items), 
			static_cast<int>(FileManagementUnit::selectivity), FileManagementUnit::fileLst };

		copyObj.copy();
		break;
	}

	}
}

std::bitset<3> Find::determineSearchParameters() {
	std::bitset<3> searchParameters;

	std::cout << "Please enter the IDs of all search parameters you prefer (CTRL + Z to finish):" << std::endl;
	std::cout << "1 - Date of last modification" << std::endl;
	std::cout << "2 - Portion of the item name" << std::endl;
	std::cout << "3 - Extension of the item" << std::endl;

	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}

	int count{ 0 };

	while (std::cin && count < 3) {
		int choice{};
		std::cin >> choice;

		if (!std::cin.eof() && std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		if (choice >= 1 && choice <= 3 && !searchParameters[choice - 1]) {
			++count;
			searchParameters.set(choice - 1);
		}

		else if (!std::cin.eof()) {
			std::cerr << std::endl << "Invalid or duplicate ID. Input ignored." << std::endl << std::endl;
		}
	}

	return searchParameters;
}

Find::DATE_PRECISION Find::determineDatePrecision() {
	if (!std::cin.good()) {
		std::cin.clear();
	}

	std::cout << "Would you like to target items up to the precision of a day or an hour?:" << std::endl;
	std::cout << "1 - Day" << std::endl;
	std::cout << "2 - Hour" << std::endl;

	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}

	int datePrecision{};
	std::cin >> datePrecision;

	if (!std::cin.good()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while (datePrecision < 1 || datePrecision > 2) {
		std::cout << "Invalid input. Enter an integer 1 - 2." << std::endl;

		if (std::cin.peek() == '\n') {
			std::cin.ignore();
		}

		std::cin >> datePrecision;

		if (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

	}

	return static_cast<Find::DATE_PRECISION>(datePrecision);
}

std::string Find::determineTargetDate() {
	std::cout << "Enter the target date in dd/mm/yyyy format (e.g. 27/11/2019 ):" << std::endl;

	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}

	std::string targetDate;
	std::getline(std::cin, targetDate);

	return targetDate;
}

int Find::determineTargetHour() {
	int targetHour{ -1 };

	if (datePrecision == Find::DATE_PRECISION::HOUR) {
		std::cout << "Enter the target hour in 24-hour clock format:" << std::endl;
		std::cin >> targetHour;

		if (!std::cin.good()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}

		while (targetHour < 0 || targetHour > 23) {
			std::cout << "Invalid input. Enter an integer 0 - 23" << std::endl;

			std::cin >> targetHour;

			if (!std::cin.good()) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}
	}

	return targetHour;
}

std::string Find::determineTargetNameSubstr() {
	std::cout << "Enter target substring of the item name:" << std::endl;

	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}

	std::string targetSubstr;
	std::getline(std::cin, targetSubstr);

	return targetSubstr;
}

std::string Find::determineTargetExtension() {
	std::cout << "Enter target extension (\"Folder\" if you are targeting directories):" << std::endl;
	std::string targetExt;
	std::cin >> targetExt;

	if (!targetExt.empty() && targetExt[0] != '.') {
		targetExt = "." + targetExt;
	}

	return targetExt;
}