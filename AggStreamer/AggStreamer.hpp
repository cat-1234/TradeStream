#pragma once


#include "../FileHandle/CSVFileHandle.hpp"
#include "../Serializer/Serializer.hpp"


class AggStreamer
{
protected:
    std::unique_ptr<FileWriter> writer;
    std::unique_ptr<Serializer> serializer;
public:
    AggStreamer(std::unique_ptr<FileWriter> wrtr, std::unique_ptr<Serializer> szr)
    : writer(std::move(wrtr)), serializer(std::move(szr))
    {}
    virtual void stream() = 0;
    virtual ~AggStreamer() = default;
};

class ZMQAggStreamer : public AggStreamer
{
protected:
    std::unique_ptr<ZMQHandle> pair;
    std::unique_ptr<ZMQHandle> push;
    std::string pair_hostname;
    std::string pair_port;
    bool connected;
    std::vector<std::string> aggsets;
    std::string db_hostname;
    std::string db_port;
public:
    ZMQAggStreamer(std::unique_ptr<FileWriter> wrtr,
                   std::unique_ptr<Serializer> szr,
                   const std::shared_ptr<ZMQHandleFactory>& fac,
                   const std::string& pair_hostname,
                   const std::string& pair_port,
                   const std::vector<std::string>& aggsets)
    : AggStreamer(std::move(wrtr), std::move(szr)),
    pair_hostname(pair_hostname), pair_port(pair_port),
    aggsets(aggsets)
    {
        push = fac->make_push_multi();
        pair = fac->make_pair(ZMQHandle::RECEIVE);
    }

    void get_req_str(std::string& str)
    {
        std::stringstream s;
        auto delim = serializer->get_delim();
        s << "conn";
        s << std::string(1, delim);
        s << pair_hostname;
        s << std::string(1, delim);
        s << pair_port;
        for (const auto& setname: aggsets)
        {
            s << std::string(1, delim);
            s << setname;
        }
        str = s.str();
    }

    int connect(const std::string& hostname, const std::string& port)
    {
        // push to db addr;
        // not ideal
        auto res1 = pair->connect(pair_hostname, pair_port);
        auto res2 = push->connect(hostname, port);
        std::string req;
        char data[64];
        get_req_str(req);
        serializer->serialize(req, data);
        auto res = push->send(data, 64);
        if (res > 0)
        {
            connected = true;
            return 0;
        }
        return -1;
    }

    void stream() override
    {
        if (!connected) return;
        char data[64];
        std::string s;
        auto recv_res = pair->recv(data, 64);
        serializer->deserialize_int(data, s);
        while (s != "stop")
        {
            writer->writeline(s.data(), s.size());
            pair->recv(data, 64);
            serializer->deserialize_int(data, s);
        }
    }
};
