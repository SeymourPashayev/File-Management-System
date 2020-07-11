#include "Copy.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <regex>

Copy::Copy(std::filesystem::path& pathFrom) {
    this->pathFrom = pathFrom;
    this->copyItems = determineCopyItems();
    this->pathTo = determineTargetDirectory();
    this->nameConflictResolution = determineNameConflictResolution();

    if (copyItems == COPY_ITEMS::ALL) {
        this->copySelectivity = COPY_SELECTIVITY::ALL;
    }

    else {
        this->copySelectivity = determineCopySelectivity();
    }
}

void Copy::copy() {
    std::filesystem::copy_options cpyOpt{ std::filesystem::copy_options::skip_existing };
    cpyOpt = (nameConflictResolution == NAME_CONFLICT_RESOLUTION::KEEP ? cpyOpt : std::filesystem::copy_options::overwrite_existing);

    switch (copyItems) {

    case Copy::COPY_ITEMS::ALL: {
        std::filesystem::copy(pathFrom, pathTo, cpyOpt | std::filesystem::copy_options::recursive);
        break;
    }

    case Copy::COPY_ITEMS::FILES: {
        copyFiles(cpyOpt);
        break;
    }

    case Copy::COPY_ITEMS::DIRECTORIES: {
        copyDirectories(cpyOpt);
        break;
    }
    }
}

void Copy::copyFiles(std::filesystem::copy_options& cpyOpt) {
    switch (copySelectivity) {

    case Copy::COPY_SELECTIVITY::ALL: {
        for (auto& dirEntry : std::filesystem::directory_iterator{ pathFrom }) {
            if (dirEntry.is_regular_file()) {
                std::filesystem::copy_file(dirEntry.path(), pathTo / dirEntry.path().filename(), cpyOpt);
            }
        }
        break;
    }

    case Copy::COPY_SELECTIVITY::CUSTOM: {
        std::deque<int> selectedFiles{ selectItems() };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ pathFrom }) {
            if (!selectedFiles.empty() && count == selectedFiles.front()) {
                std::filesystem::copy_file(dirEntry.path(), pathTo / (dirEntry.path().filename()), cpyOpt);
                selectedFiles.pop_front();
            }

            count++;
        }
        break;
    }

    }
}

void Copy::copyDirectories(std::filesystem::copy_options& cpyOpt) {
    switch (copySelectivity) {

    case Copy::COPY_SELECTIVITY::ALL: {
        std::filesystem::copy(pathFrom, pathTo, cpyOpt | std::filesystem::copy_options::recursive | std::filesystem::copy_options::directories_only);
        break;
    }

    case Copy::COPY_SELECTIVITY::CUSTOM: { 
        std::deque<int> selectedDirectories{ selectItems() };
        long long count{ 1 };

        for (std::filesystem::recursive_directory_iterator next{ pathFrom }, end; next != end; ++next) {
            if (next.depth() == 0) {

                if (!selectedDirectories.empty() && count == selectedDirectories.front()) {
                    std::filesystem::create_directory(pathTo / (*next).path().filename());

                    if (selectedDirectories.size() > 1) {
                        selectedDirectories.pop_front();
                    }
                }

                count++;
            }

            else {
                std::string parentDirectoryStr{ (*next).path().parent_path().string() };
                std::regex parentReg{ regexStrParser(pathFrom.string()) };
                std::filesystem::path targetParentDirectory{ std::regex_replace(parentDirectoryStr, parentReg, pathTo.string()) };

                if (std::filesystem::exists(targetParentDirectory) && (*next).is_directory()) { 
                    std::string outPath{ (*next).path().string() };

                    std::string regStr{ pathFrom.string() };
                    std::regex reg{ regexStrParser(regStr) };

                    std::string parsedPath{ std::regex_replace(outPath, reg, pathTo.string()) };
                    std::filesystem::create_directory(parsedPath);
                }
            }
        }
        break;
    }
    }
}

std::deque<int> Copy::selectItems() {
    std::deque<int> selectedFiles;
    std::vector<int> allFiles;

    std::ostringstream itemLstOutput;
    long long count{ 1 };

    for (auto& dirEntry : std::filesystem::directory_iterator{ pathFrom }) {
        std::filesystem::path filePath{ dirEntry.path() };

        if ((copyItems == Copy::COPY_ITEMS::FILES && std::filesystem::is_regular_file(filePath)) ||
            (copyItems == Copy::COPY_ITEMS::DIRECTORIES && std::filesystem::is_directory(filePath))) { //only regular files recorded into stream

            itemLstOutput << "ID# " << count << " . File name: " << filePath.filename() << std::endl;
            allFiles.push_back(count);
        }

        if (count % 15 == 0) { //display stream contents every 15 files, ask user which to copy
            displayItems(allFiles, selectedFiles, itemLstOutput);
        }

        count++;
    }

    if (itemLstOutput.str() != "") {
        displayItems(allFiles, selectedFiles, itemLstOutput);
    }


    return selectedFiles;
}

void Copy::displayItems(std::vector<int>& allFiles, std::deque<int>& selectedFiles, std::ostringstream& itemLstOutput) {
    std::cout << std::endl << itemLstOutput.str();
    std::cout << std::endl << "From the list above, enter the IDs of the items you would like to copy." << std::endl;
    std::cout << "To terminate, CTRL + z" << std::endl;

    int selectedID;
    int selectionCount{ 0 };

    while (std::cin >> selectedID && selectionCount < allFiles.size()) {
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

std::string Copy::determineTargetDirectory() {
    std::cout << std::endl << "Enter the target directory:" << std::endl;

    std::cin.ignore();

    std::string pathToStr;
    std::getline(std::cin, pathToStr);

    return pathToStr;
}

Copy::COPY_ITEMS Copy::determineCopyItems() {
    std::cout << std::endl << std::setw(50) << "Type of the items to be copied:" << std::endl;
    std::cout << std::setw(50) << "1 - All files and directories." << std::endl;
    std::cout << std::setw(50) << "2 - Selected files." << std::endl;
    std::cout << std::setw(50) << "3 - Selected directories." << std::endl;

    int copyItemType;
    std::cin >> copyItemType;

    while (copyItemType < 1 || copyItemType > 3) {
        std::cerr << "Invalid input. Enter an integer 1-3." << std::endl;
        std::cin >> copyItemType;
    }

    return static_cast<Copy::COPY_ITEMS>(copyItemType);
}

Copy::COPY_SELECTIVITY Copy::determineCopySelectivity() {
    std::cout << std::endl << std::setw(50) << "Selectivity:" << std::endl;
    std::cout << std::setw(50) << "1 - All items." << std::endl;
    std::cout << std::setw(50) << "2 - Custom selection." << std::endl;

    int copySelectivity;
    std::cin >> copySelectivity;

    while (copySelectivity < 1 || copySelectivity > 2) {
        std::cerr << "Invalid input. Enter an integer 1 - 2 ." << std::endl << std::endl;
        std::cin >> copySelectivity;
    }

    return static_cast<Copy::COPY_SELECTIVITY>(copySelectivity);
}

Copy::NAME_CONFLICT_RESOLUTION Copy::determineNameConflictResolution() {
    std::cout << std::endl << std::setw(50) << "Resolution of file name conflicts:" << std::endl;
    std::cout << std::setw(50) << "1 - Keep the file in target directory." << std::endl;
    std::cout << std::setw(50) << "2 - Overwrite the file in target directory." << std::endl;

    int nameConflictResolution;
    std::cin >> nameConflictResolution;

    while (nameConflictResolution < 1 || nameConflictResolution > 2) {
        std::cerr << "Invalid input. Enter an integer 1-2 ." << std::endl;
        std::cin >> nameConflictResolution;
    }

    return static_cast<Copy::NAME_CONFLICT_RESOLUTION>(nameConflictResolution);
}

std::string Copy::regexStrParser(std::string regStr) {
    std::string parsedRegStr;

    for (auto& letter : regStr) {
        parsedRegStr += letter;

        if (letter == '\\') {
            parsedRegStr.append("\\");
        }

    }

    return parsedRegStr;
}
