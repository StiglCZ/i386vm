#include <fstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
vector<unsigned char> readFileBytes(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<unsigned char> buffer;
    if (file) {
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        buffer.resize(fileSize);
        file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    } else std::cout << "Failed to open the file." << std::endl;
    file.close();
    return buffer;
}