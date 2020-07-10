#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>

// Prototype Functions
int menu();

// Global Variables


int main() {

    // TO IMPLEMENT {NEW = 1, SORT, COPY, RENAMEALL, FIND, DELETE, END, INFO }
    enum class CHOICE { NEW = 1, COPY, END };

    CHOICE action{ static_cast<CHOICE>(menu()) };

    while (action != CHOICE::END) {

        switch (action) {
        case CHOICE::NEW:
            std::cout << "TEMP DISFUNC" << std::endl;
            break;

        case CHOICE::COPY:
            std::string fromPathStringForm;
            std::string toPathStringForm;

            // Get a directory to copy from
            std::cout << "Enter a directory to copy from: " << std::endl;
            std::cin >> fromPathStringForm;
            std::filesystem::path fromPath{fromPathStringForm};

            // Get a directory to copy to
            std::cout << "Enter a directory to copy to: " << std::endl;
            std::cin >> toPathStringForm;
            std::filesystem::path toPath{toPathStringForm};

            std::filesystem::copy(fromPath, toPath);
            // TODO: Add Options to copy function (make user chose between rewrite and other types of copy if the file already exists)
            break;
        }

            //Get the next input
            action = static_cast<CHOICE>(menu());
        
    }

}

// The function prints out the menu and takes in an its
// @Input: (int) the selected function
// @Output: (int) the selected function to be inputted into the enum class CHOICE
int menu() {
    std::cout << std::endl << std::left << std::setw(50) << "1 - N/A" << std::endl;
    std::cout << std::setw(50) << "2 - COPY" << std::endl;
    std::cout << std::setw(50) << "3 - QUIT" << std::endl;
    //std::cout << std::setw(50) << "4 -" << std::endl;
    //std::cout << std::setw(50) << "5 -" << std::endl << std::endl;

    int choice;
    std::cin >> choice;

    while (choice < 1 || choice > 3) {
        std::cerr << "Invalid input. Enter a number 1-3." << std::endl;
        std::cin >> choice;
    }

    return choice;
}