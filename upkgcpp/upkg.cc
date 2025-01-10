#include <iostream>
#include <string>
#include <cstring>


void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] input_file.deb\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help        Display this help message\n";
    std::cout << "  -v, --version     Display the program version\n";
    std::cout << "  -f, --file <file> Specify the input file\n";
    // Add more options as needed
}

int main(int argc, char* argv[]) {
    // ... (parse command-line arguments)
    if (argc == 1) {
        printUsage(argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
	char *filename = argv[i];
	char *extension = strrchr(filename, '.');
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (std::string(argv[i]) == "-v" || std::string(argv[i]) == "--version") {
            std::cout << "Program Version 1.0\n";
            return 0;
	} else if (extension != NULL && strcmp(extension, ".deb") == 0) {
	    std::cout << "filename=" << argv[i] << "\n";
        } else if (std::string(argv[i]) == "-f" || std::string(argv[i]) == "--file") {
            if (i + 1 < argc) {
                std::string filename = argv[++i];
                // ... (process the filename)
            } else {
                std::cerr << "Error: Missing filename after -f option\n";
                return 1;
            }
        }
        // ... (handle other options)
    }
    return 0;
}
