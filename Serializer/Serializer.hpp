#pragma once

#include <string>


// I'm implementing all of these so if someone uses them where inappropriate there's no crash, just useful res;


class Serializer
{
protected:
    char delim;
public:
    Serializer(char delim) : delim(delim) {}
    virtual bool serialize(const std::string&, char*) = 0;
    virtual bool deserialize(char*, std::string&) = 0;
    virtual bool deserialize_int(char*, std::string&) { return false; }
    virtual bool deserialize_one(char*, std::string&, int idx) { return false; }
    virtual bool deserialize_one(char*, int, int idx) { return false; }
    virtual bool deserialize_one(char*, float, int idx) { return false; }
    virtual bool deserialize_one(char*, double, int idx) { return false; }
    virtual bool deserialize_one(char*, long double, int idx) { return false; }
    virtual char get_delim() { return delim; }
    virtual ~Serializer() = default;
};
