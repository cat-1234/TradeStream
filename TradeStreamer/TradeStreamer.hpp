#pragma once

#include "../FileHandle/CSVFileHandle.hpp"
#include "../Serializer/ChunkLenSerializer.hpp"
#include "../ZMQHandle/TCPZMQHandle.hpp"


class TradeStreamer
{
protected:
    std::unique_ptr<FileReader> reader;
    std::unique_ptr<Serializer> serializer;
public:
    TradeStreamer(std::unique_ptr<FileReader> rdr, std::unique_ptr<Serializer> szr)
     : reader(std::move(rdr)), serializer(std::move(szr))
    {}
    virtual void stream() = 0;
    virtual ~TradeStreamer() = default;
};

class ZMQTradeStreamer : public TradeStreamer
{
protected:
    std::unique_ptr<ZMQHandle> pub;
public:
    ZMQTradeStreamer(std::unique_ptr<FileReader> rdr, std::unique_ptr<Serializer> szr, const std::shared_ptr<ZMQHandleFactory>& fac)
     : TradeStreamer(std::move(rdr), std::move(szr))
    {
        pub = fac->make_pub();
    }
    int connect(const std::string& hostname, const std::string& port)
    {
        return pub->connect(hostname, port);
        // hacky for now
//        std::string s;
//        std::cout << "pub connected, please enter anything when subs are ready" << std::endl;
//        std::cin >> s;
        // --------
//        return 0;
    }
    void stream() override
    {
        std::string line;
        char data[64];
        while (reader->readline(line))
        {
            serializer->serialize(line, data);
            pub->send(&data[0], 64);
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
        if (!line.empty())
        {
            serializer->serialize(line, data);
            pub->send(&data[0], 64);
        }
        data[0] = 'e';
        pub->send(&data[0], 64);
    }
};
