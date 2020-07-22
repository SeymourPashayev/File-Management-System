
#include <iostream>
#include <filesystem>
#include <string>
#include <limits>
#include "Copy.h"
#include "Delete.h"
#include "Rename.h"

enum class CHOICE { NEW = 1, RENAME, DELETE, INFO, COPY, SORT, FIND, END };

int menu();
std::filesystem::path determineSourceDirectory(CHOICE choice);

int main() { 

    CHOICE choice{ static_cast<CHOICE>(menu()) };

    while (choice != CHOICE::END) {
        std::filesystem::path pathFrom{ determineSourceDirectory(choice) };

        switch (choice) {

        case CHOICE::NEW: {
            break;
        }

        case CHOICE::RENAME: {
            Rename renameObj{ pathFrom };
            renameObj.rename();
            break;
        }

        case CHOICE::DELETE: {
            Delete deleteObj{ pathFrom };
            deleteObj.deletion();
            break;
        }

        case CHOICE::INFO: {
            break;
        }

        case CHOICE::COPY: {
            Copy copyObj{ pathFrom };
            copyObj.copy();
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

    int choice{};
    std::cin >> choice;

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (choice < 1 || choice > 8) {
        std::cerr << std::endl << "Invalid input. Enter an integer 1-8 ." << std::endl;

        std::cin >> choice;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return choice;
}

std::filesystem::path determineSourceDirectory(CHOICE choice) {
    std::cout << std::endl << "Enter the" << (choice == CHOICE::COPY ? " original" : "") << " directory:" << std::endl;

    if (std::cin.peek() == '\n') {
        std::cin.ignore();
    }

    std::string pathFromStr;
    std::getline(std::cin, pathFromStr);
    std::filesystem::path tempPath{ pathFromStr }; 

    if (!std::cin.good()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    while (!std::filesystem::exists(tempPath)) {
        std::cerr << std::endl << "Path does not exist." << std::endl;

        std::cout << "Enter the" << (choice == CHOICE::COPY ? " original" : "") << " directory:" << std::endl;

        if (std::cin.peek() == '\n') {
            std::cin.ignore();
        }

        std::getline(std::cin, pathFromStr);
        tempPath = pathFromStr;

        if (!std::cin.good()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return tempPath;
}





