#pragma once


#include <iostream>
#include "../ZMQHandleFactory/ZMQHandleFactory.hpp"
#include "../Serializer/ChunkLenSerializer.hpp"



class Agg
{
protected:
//    FRIEND_TEST(AggsTest, StatefulAggsTest);
    using MapType = std::unordered_map<std::string, std::vector<long double>>;
    MapType map;
    MapType::iterator aggValsIt;
    std::vector<int> idcs;
    std::vector<int> new_vals;
    int size;
    std::unique_ptr<ZMQHandle> sub;
    std::unique_ptr<ZMQHandle> push;
    std::unique_ptr<Serializer> serializer;
    virtual void get_it(const std::string& symbol)
    {
        aggValsIt = map.find(symbol);
        if (aggValsIt == map.end())
            aggValsIt = map.insert(aggValsIt, {symbol, std::vector<long double>(size, 0)});
    }
public:
    Agg(const std::initializer_list<int>& idcs, int size, const std::shared_ptr<ZMQHandleFactory>& fac, std::unique_ptr<Serializer> s)
    : idcs(idcs.begin(), idcs.end()), new_vals(idcs.size()), size(size), serializer(std::move(s))
    {
        sub = fac->make_sub();
        push = fac->make_push_multi();
    }
    virtual int connect_sub(const std::string& hostname, const std::string& port)
    {
        return sub->connect(hostname, port);
    }
    virtual int connect_push(const std::string& hostname, const std::string& port)
    {
        return push->connect(hostname, port);
    }
// only for testing

    int send(const std::string& message)
    {
        char data[64];
        serializer->serialize(message, data);
        return push->send(data, 64);
    }

// ----
    virtual void recv()
    {
        char data[64];
        std::string symbol;
        auto count = 0;
        while (1)
        {
            sub->recv(data, 64);
            if (data[0] == 'e') break;
            serializer->deserialize_one(data, symbol, 1);
            get_it(symbol);
            for (int i = 0; i < idcs.size(); ++i)
                serializer->deserialize_one(data, new_vals[i], idcs[i]);
            agg();
        }
//        send_to_writer();
    }
//    virtual void send_to_writer() = 0;
    virtual void agg() = 0;
    virtual ~Agg() = default;
};

class AverageAgg : public Agg
{
public:
    AverageAgg(const std::initializer_list<int>& idcs, const std::shared_ptr<ZMQHandleFactory>& fac, std::unique_ptr<Serializer> s)
    : Agg(idcs, 2, fac, std::move(s))
    {}

    void agg() override
    {
        aggValsIt->second[0] = ((aggValsIt->second[0] * aggValsIt->second[1]) + new_vals[0]) / (aggValsIt->second[1] + 1);
        aggValsIt->second[1] += 1;
    }
};

class SumAgg : public Agg
{
public:
    SumAgg(const std::initializer_list<int>& idcs, const std::shared_ptr<ZMQHandleFactory>& fac, std::unique_ptr<Serializer> s)
    : Agg(idcs, 1, fac, std::move(s))
    {}
    
    void agg() override
    {
        aggValsIt->second[0] += new_vals[0];
    }
};

class MaxRangeAgg : public Agg
{
public:
    MaxRangeAgg(const std::initializer_list<int>& idcs, const std::shared_ptr<ZMQHandleFactory>& fac, std::unique_ptr<Serializer> s)
    : Agg(idcs, 2, fac, std::move(s))
    {}

    void agg() override
    {
        if ((aggValsIt->second[1] != 0) && (new_vals[0] - aggValsIt->second[1] > aggValsIt->second[0]))
            aggValsIt->second[0] = new_vals[0] - aggValsIt->second[1];
        aggValsIt->second[1] = new_vals[0];
    }
};

class MaxAgg : public Agg
{
public:
    MaxAgg(const std::initializer_list<int>& idcs, const std::shared_ptr<ZMQHandleFactory>& fac, std::unique_ptr<Serializer> s)
    : Agg(idcs, 1, fac, std::move(s))
    {}

    void agg() override
    {
        if (new_vals[0] > aggValsIt->second[0])
            aggValsIt->second[0] = new_vals[0];
    }
};

class WeightedAverageAgg : public Agg
{
public:
    WeightedAverageAgg(const std::initializer_list<int>& idcs, const std::shared_ptr<ZMQHandleFactory>& fac, std::unique_ptr<Serializer> s)
    : Agg(idcs, 2, fac, std::move(s))
    {}

    void agg() override
    {
        aggValsIt->second[0] = (((aggValsIt->second[0] * aggValsIt->second[1]) + (new_vals[0] * new_vals[1])) / (aggValsIt->second[1] + new_vals[1]));
        aggValsIt->second[1] += new_vals[1];
    }
};
