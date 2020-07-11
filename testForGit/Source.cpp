#include <iostream>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <sstream>
#include <iterator>


int menu();
void copyFiles(std::filesystem::path&);
std::deque<int> selectFiles(std::filesystem::path& outPath);
void displayFiles(std::vector<int>& allFiles, std::deque<int>& selectedFiles, std::ostringstream& itemLstOutput);

int main() { 
	enum class CHOICE{ NEW = 1, RENAME, DELETE, INFO, COPY, SORT, FIND, END};

    CHOICE choice{ static_cast<CHOICE>(menu()) };

    while (choice != CHOICE::END) {
        std::cout << std::endl << "Enter the" << (choice == CHOICE::COPY ? " original" : "") << " directory:" << std::endl;
        std::cin.ignore(); //ignore last crlf

        std::string pathFromStr;
        std::getline(std::cin, pathFromStr);
        std::filesystem::path pathFrom{ pathFromStr };

        switch (choice) {

        case CHOICE::NEW: {
            break;
        }

        case CHOICE::RENAME: {
            break;
        }

        case CHOICE::DELETE: {
            break;
        }

        case CHOICE::INFO: {
            break;
        }

        case CHOICE::COPY: {
            copyFiles(pathFrom);
            break;
        }

        case CHOICE::SORT: {
          
            break;
        }

        case CHOICE::FIND: {
            break;
        }

        }

        choice = static_cast<CHOICE>(menu());
    }
}

int menu() {
    std::cout << std::endl << std::left << std::setw(50) << "1 - Create new files / directories." << std::endl;
    std::cout << std::setw(50) << "2 - Rename an existing file / directory." << std::endl;
    std::cout << std::setw(50) << "3 - Delete an existing file / directory." << std::endl;
    std::cout << std::setw(50) << "4 - Get information on an existing file / directory." << std::endl;
    std::cout << std::setw(50) << "5 - Copy an existing file / directory to another directory." << std::endl;
    std::cout << std::setw(50) << "6 - Sort files in a directory with a key." << std::endl;
    std::cout << std::setw(50) << "7 - Find file(s) in a directory with a key." << std::endl;
    std::cout << std::setw(50) << "8 - Exit program." << std::endl << std::endl;

    int choice;
    std::cin >> choice;

    while (choice < 1 || choice > 8) {
        std::cerr << "Invalid input. Enter an integer 1-8 ." << std::endl;
    }

    return choice;
}

void copyFiles(std::filesystem::path& pathFrom) {
    //Start: target directory determination
    std::cout << std::endl << "Enter the target directory:" << std::endl;

    std::string pathToStr;
    std::getline(std::cin, pathToStr);
    std::filesystem::path pathTo{ pathToStr };
    //Finish: target directory determination

    //Start: type of items to be copied
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
    //Finish: type of items to be copied

    //Start: conflicting file name resolution
    std::cout << std::endl << std::setw(50) << "Resolution of file name conflicts:" << std::endl;
    std::cout << std::setw(50) << "1 - Keep the file in target directory." << std::endl;
    std::cout << std::setw(50) << "2 - Overwrite the file in target directory." << std::endl;

    int fileNameConflict;
    std::cin >> fileNameConflict;

    while (fileNameConflict < 1 || fileNameConflict > 2) {
        std::cerr << "Invalid input. Enter an integer 1-2 ." << std::endl;
        std::cin >> fileNameConflict;
    }

    std::filesystem::copy_options cpyOpt;

    if (fileNameConflict == 1) {
        cpyOpt = std::filesystem::copy_options::skip_existing;
    }

    else {
        cpyOpt = std::filesystem::copy_options::overwrite_existing;
    }
    //Finish: conflicting file name resolution


    //Start: selected copying process
    enum class COPY_CHOICE{ALL = 1, FILES, DIRECTORIES};

    COPY_CHOICE copyChoice{ static_cast<COPY_CHOICE>(copyItemType) };

    switch (copyChoice) {

    case COPY_CHOICE::ALL: {
        std::filesystem::copy(pathFrom, pathTo, cpyOpt | std::filesystem::copy_options::recursive);
        break;
    }

    case COPY_CHOICE::FILES: {
        std::deque<int> selectedFiles{ selectFiles(pathFrom) };
        long long count{ 1 };

        for (auto& dirEntry : std::filesystem::directory_iterator{ pathFrom }) {
            if (!selectedFiles.empty() && count == selectedFiles.front()) {
                std::filesystem::copy_file(dirEntry.path(), pathTo / (dirEntry.path().filename().string()), cpyOpt);
                selectedFiles.pop_front();
            }

            count++;
        }
          
        break;
    }
     
    case COPY_CHOICE::DIRECTORIES: {
        break;
    }
        
    }
    //Finish: selected copying process
}

std::deque<int> selectFiles(std::filesystem::path& outPath) {
    std::deque<int> selectedFiles;
    std::vector<int> allFiles;

    std::ostringstream itemLstOutput;
    long long count{ 1 };

    for (auto& dirEntry : std::filesystem::directory_iterator{ outPath }) {
        std::filesystem::path filePath{ dirEntry.path() }; 

        if (std::filesystem::is_regular_file(filePath)) { //only regular files recorded into stream
            itemLstOutput << "ID# " << count << " . File name:" << filePath.filename() << std::endl;
            allFiles.push_back(count);
        }

        if (count % 15 == 0) { //display stream contents every 15 files, ask user which to copy
            displayFiles(allFiles, selectedFiles, itemLstOutput);
        }

        count++;
    }

    if (itemLstOutput.str() != "") {
        displayFiles(allFiles, selectedFiles, itemLstOutput);
    }


    return selectedFiles;
}

void displayFiles(std::vector<int>& allFiles, std::deque<int>& selectedFiles, std::ostringstream& itemLstOutput) {
    std::cout << itemLstOutput.str();
    std::cout << std::endl << "From the list above, enter the IDs of the files you would like to copy." << std::endl;
    std::cout << "To terminate, CTRL + z" << std::endl;

    int selectedID;
    int selectionCount{ 0 };

    while (std::cin >> selectedID && selectionCount < allFiles.size()) {
        if (std::binary_search(allFiles.begin(), allFiles.end(), selectedID) && !std::binary_search(selectedFiles.begin(), selectedFiles.end(), selectedID)) {
            selectedFiles.push_back(selectedID);
            std::sort(selectedFiles.begin(), selectedFiles.end());
            selectionCount++;
        }

        else if (!std::binary_search(allFiles.begin(), allFiles.end(), selectedID)) {
            std::cerr << std::endl << "Invalid input. Make sure file ID is on the list." << std::endl << std::endl;
        }
    }

    std::cin.clear();
    allFiles.clear();
    itemLstOutput.str("");
    itemLstOutput.clear();
}

