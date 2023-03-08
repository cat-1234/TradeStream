#pragma once


#include "Serializer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


class ChunkLenSerializer : public Serializer
{
protected:
    int nchunks;
    std::stringstream ss;
public:
    ChunkLenSerializer(char delim, int nchunks)
    : Serializer(delim), nchunks(nchunks)
    {}

    bool serialize(const std::string& s, char* data) override
    {
        int chunk_idx = 0;
        int count = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            if (s[i] == delim)
                data[chunk_idx++] = count;
            else
            {
                data[nchunks + count++] = s[i];
            }
        }
        data[chunk_idx] = count;
        return true;
    }

    bool deserialize(char* data, std::string& s) override
    {
        int chunk_idx = 0;
        int st_idx = nchunks;
        ss.str("");
        for (int i = 0; i < nchunks; ++i)
        {
            if (i > 0) ss << delim;
            ss << std::string{&data[st_idx], &data[nchunks + data[chunk_idx]]};
            st_idx = nchunks + data[chunk_idx++];
        }
        s = ss.str();
        return true;
    }

// the multiple implementations are not ideal and would be better with template, but I want deserialize_one to be virtual for use by other implementations;

    bool deserialize_one(char* data, std::string& s, int idx) override
    {
        int st_idx = nchunks;
        if (idx > 0) st_idx += data[idx - 1];
        s = std::string{&data[st_idx], &data[nchunks + data[idx]]};
        return true;
    }

    bool deserialize_one(char* data, int n, int idx) override
    {
        int st_idx = nchunks;
        if (idx > 0) st_idx += data[idx - 1];
        n = std::stoi(
                std::string{&data[st_idx], &data[nchunks + data[idx]]}
        );
        return true;
    }
    bool deserialize_one(char* data, float f, int idx) override
    {
        int st_idx = nchunks;
        if (idx > 0) st_idx += data[idx - 1];
        f = std::stof(
                std::string{&data[st_idx], &data[nchunks + data[idx]]}
        );
        return true;
    }
    bool deserialize_one(char* data, double d, int idx) override
    {
        int st_idx = nchunks;
        if (idx > 0) st_idx += data[idx - 1];
        d = std::stod(
                std::string{&data[st_idx], &data[nchunks + data[idx]]}
        );
        return true;
    }
    bool deserialize_one(char* data, long double ld, int idx) override
    {
        int st_idx = nchunks;
        if (idx > 0) st_idx += data[idx - 1];
        ld = std::stold(
                std::string{&data[st_idx], &data[nchunks + data[idx]]}
        );
        return true;
    }
};

class VarChunksSerializer : public Serializer
{
protected:
    int max_chunks;
    std::stringstream ss;
public:
    VarChunksSerializer(char delim, int max_chunks = 6)
    : Serializer(delim), max_chunks(max_chunks) {}

    bool serialize(const std::string& s, char* data) override
    {
        int nchunks = 0;
        int chunk_idx = 1;
        int count = 0;
        for (int i = 0; i < s.size(); ++i)
        {
            if (s[i] == delim)
            {
                ++nchunks;
                data[chunk_idx++] = count;
            }
            else
            {
                data[max_chunks + 1 + count++] = s[i];
            }
        }
        data[chunk_idx] = count;
        data[0] = nchunks + 1;
        return nchunks < max_chunks;
    }
    bool deserialize(char* data, std::string& s) override
    {
        int chunk_idx = 1;
        int nchunks{static_cast<int>(data[0])};
        int st_idx = max_chunks + 1;
        ss.str("");
        for (int i = 0; i < nchunks; ++i)
        {
            if (i > 0) ss << delim;
            ss << std::string{&data[st_idx], &data[max_chunks + 1 + data[chunk_idx]]};
            st_idx = max_chunks + 1 + data[chunk_idx++];
        }
        s = ss.str();
        return true;
    }

    bool deserialize_int(char* data, std::string& s) override
    {
        int nchunks = data[0];
        ss.str("");
        int st_idx = max_chunks + 1;
        for (int i = 0; i < nchunks; ++i)
        {
            if (i > 0) ss << delim;
            if (i == 1)
                ss << std::string{&data[st_idx], &data[max_chunks + 1 + data[i + 1]]};
            else
                ss << std::stoi(std::string{&data[st_idx], &data[max_chunks + 1 + data[i + 1]]});
            st_idx = max_chunks + 1 + data[i + 1];
        }
        s = ss.str();
        return true;
    }

    bool deserialize_one(char* data, std::string& s, int idx) override
    {
        int st_idx = max_chunks + 1;
        if (idx > 0) st_idx += data[idx];
        s = std::string{&data[st_idx], &data[max_chunks + 1 + data[idx + 1]]};
        return true;
    }

    bool deserialize_one(char* data, int n, int idx) override
    {
        int st_idx = max_chunks + 1;
        if (idx > 0) st_idx += data[idx];
        n = std::stoi(
                std::string{&data[st_idx], &data[max_chunks + 1 + data[idx + 1]]}
        );
        return true;
    }

    bool deserialize_one(char* data, float f, int idx) override
    {
        int st_idx = max_chunks + 1;
        if (idx > 0) st_idx += data[idx];
        f = std::stof(
                std::string{&data[st_idx], &data[max_chunks + 1 + data[idx + 1]]}
        );
        return true;
    }

    bool deserialize_one(char* data, double d, int idx) override
    {
        int st_idx = max_chunks + 1;
        if (idx > 0) st_idx += data[idx];
        d = std::stod(
                std::string{&data[st_idx], &data[max_chunks + 1 + data[idx + 1]]}
        );
        return true;
    }
    bool deserialize_one(char* data, long double ld, int idx) override
    {
        int st_idx = max_chunks + 1;
        if (idx > 0) st_idx += data[idx];
        ld = std::stold(
                std::string{&data[st_idx], &data[max_chunks + 1 + data[idx + 1]]}
        );
        return true;
    }
};
