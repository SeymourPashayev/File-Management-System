#include "Delete.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>

Delete::Delete(std::filesystem::path& path) {
	this->path = path;
	this->deleteItems = determineDeleteItems();
	this->deleteSelectivity = (deleteItems == Delete::DELETE_ITEMS::ALL? Delete::DELETE_SELECTIVITY::ALL : determineDeleteSelectivity());
}

void Delete::deletion() {
	switch (deleteItems) {

	case Delete::DELETE_ITEMS::ALL: {
        std::filesystem::remove_all(path);
		break;
	}

	case Delete::DELETE_ITEMS::FILES: {
        deleteFiles();
		break;
	}

	case Delete::DELETE_ITEMS::DIRECTORIES: {
        deleteDirectories();
		break;
    }

	}
}

void Delete::deleteFiles() {
    switch (deleteSelectivity) {

    case Delete::DELETE_SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (dirEntry.is_regular_file()) {
                std::filesystem::remove(dirEntry.path());
            }
        }
        break;
    }

    case Delete::DELETE_SELECTIVITY::CUSTOM: {
        std::deque<long long> selectedFiles{ selectItems() };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (!selectedFiles.empty() && count == selectedFiles.front()) {
                std::filesystem::remove(dirEntry.path());
                selectedFiles.pop_front();
            }

            count++;
        }
        break;
    }

    }
}

void Delete::deleteDirectories() {
    switch (deleteSelectivity) {

    case Delete::DELETE_SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
            if (dirEntry.is_directory()) {
                std::filesystem::remove_all(dirEntry.path());
            }
        }
        break;
    }

    case Delete::DELETE_SELECTIVITY::CUSTOM: {
        std::deque<long long> selectedDirectories{ selectItems() };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ path } ) {
            if (dirEntry.is_directory() && count == selectedDirectories.front()) {
                std::filesystem::remove_all(dirEntry.path());
                selectedDirectories.pop_front();
            }

            count++;
        }
        break;
    }
    }
}

std::deque<long long> Delete::selectItems() {
    std::deque<long long> selectedFiles;
    std::vector<long long> allFiles;

    std::ostringstream itemLstOutput;
    long long count{ 1 };

    for (auto& dirEntry : std::filesystem::directory_iterator{ path }) {
        std::filesystem::path filePath{ dirEntry.path() };

        if ((deleteItems == Delete::DELETE_ITEMS::FILES && std::filesystem::is_regular_file(filePath)) ||
            (deleteItems == Delete::DELETE_ITEMS::DIRECTORIES && std::filesystem::is_directory(filePath))) {

            itemLstOutput << "ID# " << count << " . File name: " << filePath.filename() << std::endl;
            allFiles.push_back(count);
        }

        if (count % 15 == 0) {
            displayItems(allFiles, selectedFiles, itemLstOutput);
        }

        count++;
    }

    if (!itemLstOutput.str().empty()) {
        displayItems(allFiles, selectedFiles, itemLstOutput);
    }


    return selectedFiles;
}

void Delete::displayItems(std::vector<long long>& allFiles, std::deque<long long>& selectedFiles, std::ostringstream& itemLstOutput) {
    std::cout << std::endl << itemLstOutput.str();
    std::cout << std::endl << "From the list above, enter the IDs of the items you would like to delete." << std::endl;
    std::cout << "To terminate, CTRL + z" << std::endl;

    long long selectedID;
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

Delete::DELETE_ITEMS  Delete::determineDeleteItems() {
    std::cout << std::endl << std::setw(50) << "Type of the items to be deleted:" << std::endl;
    std::cout << std::setw(50) << "1 - All files and directories." << std::endl;
    std::cout << std::setw(50) << "2 - Selected files." << std::endl;
    std::cout << std::setw(50) << "3 - Selected directories." << std::endl;

    int deleteItemType;
    std::cin >> deleteItemType;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (deleteItemType < 1 || deleteItemType > 3) {
        std::cerr << std::endl << "Invalid input. Enter an integer 1-3." << std::endl;
        std::cin >> deleteItemType;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return static_cast<Delete::DELETE_ITEMS>(deleteItemType);
}

Delete::DELETE_SELECTIVITY Delete::determineDeleteSelectivity() {
    std::cout << std::endl << std::setw(50) << "Selectivity:" << std::endl;
    std::cout << std::setw(50) << "1 - All items." << std::endl;
    std::cout << std::setw(50) << "2 - Custom selection." << std::endl;

    int deleteSelectivity;
    std::cin >> deleteSelectivity;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (deleteSelectivity < 1 || deleteSelectivity > 2) {
        std::cerr << "Invalid input. Enter an integer 1 - 2 ." << std::endl;
        std::cin >> deleteSelectivity;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return static_cast<Delete::DELETE_SELECTIVITY>(deleteSelectivity);
}
