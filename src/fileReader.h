/* A class responsible for reading from input file from /simulation path*/

#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class fileReader {
public:
    fileReader();
    virtual ~fileReader();

    int countLines(std::string fileName);
    std::vector<std::string> readFile(std::string fileName);
};

#endif /* FILEREADER_H_ */
