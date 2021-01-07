//
 #include "fileReader.h"

/*Empty cctrs*/
fileReader::fileReader() {

}

/*Empty destructor*/
fileReader::~fileReader() {
}

/*Counts the number of lines in a given file*/
int fileReader::countLines(std::string fileName)
{
    std::ifstream file;
    file.open(fileName,std::ios::in);
    int count=0;
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file,line)){count++;}
        file.close();
        return count;
    }
    else
    {
        return -1;
    }

}

/*Reads the content of a file and returns a vector of strings,
 * where each line is an element in the vector.
 */
std::vector<std::string> fileReader::readFile (std::string fileName)
{
        std::vector<std::string> messages;
        std::ifstream file;
        file.open(fileName,std::ios::in);
        std::string line="";
        if(file.is_open())
        {
            while(std::getline(file,line))
            messages.push_back(line);
        }
        file.close();
        return messages;
}



