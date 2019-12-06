#include <iostream>

void doList(const std::string& input)
{

}

void showUsage()
{
    std::cerr << "Usage: asustool\n"
        << "-l <input>\n"
        << "    List files in image <input>";
}

int main(int argc, char** argv) {
    if (argc < 3) {
        showUsage();
        return -1;
    }

    std::string option = std::string(argv[1]);
    if (option == "-l") {
        std::string input = std::string(argv[2]);
        doList(input);
    } else {
        showUsage();
    }

    return 0;
}
