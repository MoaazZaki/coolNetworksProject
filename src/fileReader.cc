//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "fileReader.h"

fileReader::fileReader() {

}

fileReader::~fileReader() {
}


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



