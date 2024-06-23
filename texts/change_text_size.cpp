#include <iostream>
#include <fstream>
#include <string>

void adjustFileSize(const std::string& inputFilePath, const std::string& outputFilePath, std::size_t targetSizeMB) {
    std::size_t targetSizeBytes = targetSizeMB * 1024 * 1024;
    std::ifstream infile(inputFilePath, std::ios::in | std::ios::binary);
    if (!infile) {
        std::cerr << "Error opening input file." << std::endl;
        return;
    }
    
    std::ofstream outfile(outputFilePath, std::ios::out | std::ios::binary);
    if (!outfile) {
        std::cerr << "Error opening output file." << std::endl;
        return;
    }
    
    infile.seekg(0, std::ios::end);
    std::size_t currentSize = infile.tellg();
    infile.seekg(0, std::ios::beg);
    
    if (currentSize > targetSizeBytes) {
        char* buffer = new char[targetSizeBytes];
        infile.read(buffer, targetSizeBytes);
        outfile.write(buffer, targetSizeBytes);
        delete[] buffer;
    } else {
        std::string content((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
        outfile.write(content.c_str(), content.size());
        std::size_t paddingSize = targetSizeBytes - content.size();
        std::string padding(paddingSize, ' ');
        outfile.write(padding.c_str(), padding.size());
    }
    
    infile.close();
    outfile.close();
}

int main() {
    std::string inputFile = "proteins50MB.txt";
    std::string outputFile = "proteins1MB.txt";
    std::size_t targetSize = 1; // target size in MB

    adjustFileSize(inputFile, outputFile, targetSize);

    return 0;
}
