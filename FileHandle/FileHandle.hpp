#pragma once

#include <fstream>


class FileHandle
{};

class FileReader : public FileHandle
{
protected:
    std::ifstream fstream;

public:
    FileReader(const std::string& filename)
    {
        fstream = std::ifstream(filename);
        if (!fstream.is_open())
            throw std::runtime_error("cant open filename: " + filename);
    }

    FileReader(const FileReader&) = delete;
    FileReader& operator=(const FileReader&) = delete;
    FileReader(FileReader&& rdr) noexcept
    {
        fstream = std::move(rdr.fstream);
    }
    FileReader& operator=(FileReader&& rdr) noexcept
    {
        fstream = std::move(rdr.fstream);
        return *this;
    }

    virtual bool readline(std::string& s) = 0;

    virtual ~FileReader()
    {
        fstream.close();
    }
};

class FileWriter : public FileHandle
{
protected:
    std::ofstream fstream;
public:
    FileWriter(const std::string& filename)
    {
        fstream = std::ofstream(filename);
    }

    FileWriter(const FileWriter&) = delete;
    FileWriter& operator=(const FileWriter&) = delete;
    FileWriter(FileWriter&& writr) noexcept
    {
        fstream = std::move(writr.fstream);
    }
    FileWriter& operator=(FileWriter&& writr) noexcept
    {
        fstream = std::move(writr.fstream);
        return *this;
    }

    virtual bool writeline(char* s, int size) = 0;

    virtual ~FileWriter()
    {
        fstream.close();
    }
};
