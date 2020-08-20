#include "FileManagementUnit.h"
#include <filesystem>
#include <iostream>
#include <vector>
#include <deque>
#include <sstream>

FileManagementUnit::FileManagementUnit(std::filesystem::path path) {
	this->path = path;
	this->items = determineItems();
	this->selectivity = (items == FileManagementUnit::ITEMS::ALL ? FileManagementUnit::SELECTIVITY::ALL : determineSelectivity());
}

std::filesystem::path FileManagementUnit::getPath() {
	return this->path;
}

FileManagementUnit::ITEMS FileManagementUnit::getItems() {
    return this->items;
}

FileManagementUnit::SELECTIVITY FileManagementUnit::getSelectivity() {
    return this->selectivity;
}

FileManagementUnit::ITEMS FileManagementUnit::determineItems() {
    std::cout << std::endl << std::setw(50) << "Type of the items:" << std::endl;
    std::cout << std::setw(50) << "1 - All files and directories." << std::endl;
    std::cout << std::setw(50) << "2 - Files." << std::endl;
    std::cout << std::setw(50) << "3 - Directories." << std::endl;

    int itemType;
    std::cin >> itemType;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (itemType < 1 || itemType > 3) {
        std::cerr << std::endl << "Invalid input. Enter an integer 1-3." << std::endl;
        std::cin >> itemType;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return static_cast<FileManagementUnit::ITEMS>(itemType);
}

FileManagementUnit::SELECTIVITY FileManagementUnit::determineSelectivity() {
    std::cout << std::endl << std::setw(50) << "Selectivity:" << std::endl;
    std::cout << std::setw(50) << "1 - All items." << std::endl;
    std::cout << std::setw(50) << "2 - Custom selection." << std::endl;

    int selectivity;
    std::cin >> selectivity;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (selectivity < 1 || selectivity > 2) {
        std::cerr << "Invalid input. Enter an integer 1 - 2 ." << std::endl;
        std::cin >> selectivity;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return static_cast<FileManagementUnit::SELECTIVITY>(selectivity);
}

std::deque<long long> FileManagementUnit::selectItems() {
    std::deque<long long> selectedFiles;
    std::vector<long long> allFiles;

    std::ostringstream itemLstOutput;
    long long count{ 1 };

    for (auto& dirEntry : std::filesystem::directory_iterator{ this->path }) {
        std::filesystem::path filePath{ dirEntry.path() };

        if ((items == FileManagementUnit::ITEMS::FILES && std::filesystem::is_regular_file(filePath)) ||
            (items == FileManagementUnit::ITEMS::DIRECTORIES && std::filesystem::is_directory(filePath))) {

            itemLstOutput << "ID# " << count << " . File name: " << filePath.filename() << std::endl;
            allFiles.push_back(count);
        }

        if (count % 15 == 0) {
            displayItems(allFiles, selectedFiles, itemLstOutput);
        }

        count++;
    }

    if (itemLstOutput.str() != "") {
        displayItems(allFiles, selectedFiles, itemLstOutput);
    }


    return selectedFiles;
}
void FileManagementUnit::displayItems(std::vector<long long>& allFiles, std::deque<long long>& selectedFiles, std::ostringstream& itemLstOutput) {
    std::cout << std::endl << itemLstOutput.str();
    std::cout << std::endl << "From the list above, enter the IDs of the items you would like to copy." << std::endl;
    std::cout << "To terminate, CTRL + z" << std::endl;

    long long selectedID{ 0 };
    unsigned long long selectionCount{ 0 };

    while ((std::cin >> selectedID || !std::cin.eof()) && selectionCount < allFiles.size()) {
        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (std::binary_search(allFiles.begin(), allFiles.end(), selectedID) && !std::binary_search(selectedFiles.begin(), selectedFiles.end(), selectedID)) {
            selectedFiles.push_back(selectedID);
            std::sort(selectedFiles.begin(), selectedFiles.end());
            selectionCount++;
        }

        else {
            std::cerr << std::endl << "Invalid input. Make sure item ID is on the list and the last entry is not a duplicate." << std::endl << std::endl;
        }
    }

    std::cin.clear();
    allFiles.clear();
    itemLstOutput.str("");
    itemLstOutput.clear();
}
