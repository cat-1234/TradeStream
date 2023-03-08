#pragma once

#include "FileHandle.hpp"


class CSVFileReader : public FileReader
{
public:
    CSVFileReader(const std::string& filename) : FileReader(filename) {}

    bool readline(std::string& s) override
    {
        if (fstream.is_open())
            if (std::getline(fstream, s))
                return true;
        return false;
    }
};

class CSVFileWriter : public FileWriter
{
public:
    CSVFileWriter(const std::string& filename) : FileWriter(filename) {}

    bool writeline(char* s, int size) override
    {
        if (fstream.is_open())
        {
            auto initial_pos = fstream.tellp();
            if (fstream.write(s, size))
            {
                if (fstream.write("\n", 1))
                    return true;
                else if (fstream.is_open())
                    fstream.seekp(initial_pos);
            }
        }
        return false;
    }
};
