#pragma once


#include <unordered_map>


class DataStore
{
protected:
    using MapType = std::unordered_map<std::string, std::unordered_map<std::string, std::vector<long double>>>;
    MapType DB;
    std::vector<std::string> sorted_symbols;
    // not ideal for these purposes; other order (symbol map to aggset name map may be better)
    std::set<std::string> unique_symbols;
    std::vector<std::string> aggset_names;
    int capacity;

    std::unique_ptr<ZMQHandle> pull;
    std::vector<std::unique_ptr<ZMQHandle>> pairs;
    std::shared_ptr<ZMQHandleFactory> factory;
    std::shared_ptr<Serializer> serializer;

public:
    DataStore(const std::string& hostname, const std::string& port,
              const std::shared_ptr<Serializer>& szr,
              const std::shared_ptr<ZMQHandleFactory>& fac, int capacity = 4)
              : serializer(szr), capacity(capacity)
    {
        factory = fac;
        pull = factory->make_pull_multi();
        pull->connect(hostname, port);
    }

    void start_writer_thread(const std::string& hostname, const std::string& port)
    {
        std::cout << "in writer thread" << std::endl;
        auto delim = serializer->get_delim();
        int chunks;
        std::vector<std::string> aggset_names;
        char out_data[64];

        char data[64];
        std::string conn_str;
        std::string rcvr_hostname;
        std::string rcvr_port;

        std::string str;
        std::string out_str;
        std::string curr_val;
        std::stringstream sstrm;

        auto pull = factory->make_pull_multi();
        pull->connect(hostname, port);

        while (true)
        {
            auto maybe_write_req = pull->recv(data, 64);
            if (maybe_write_req <= 0) continue;
            serializer->deserialize_one(data, conn_str, 0);
            if (conn_str[1] != 'o' | conn_str[2] != 'n' | conn_str[3] != 'n') continue;
            serializer->deserialize_one(data, rcvr_hostname, 1);
            serializer->deserialize_one(data, rcvr_port, 2);

            chunks = int(data[0]);

            auto pair = factory->make_pair(ZMQHandle::SEND);
            auto pair_conn_res = pair->connect(rcvr_hostname, rcvr_port);
            for (int i = 3; i < chunks; ++i) {
                serializer->deserialize_one(data, str, i);
                aggset_names.push_back(str);
            }

            for (const auto &symbol: sorted_symbols)
            {
                sstrm.str("");
                sstrm << symbol;
                for (int i = 0; i < aggset_names.size(); ++i) {
                    auto aggnameIt = DB.find(aggset_names[i]);
                    if (aggnameIt != DB.end())
                    {
                        auto smbIt = aggnameIt->second.find(symbol);
                        if (smbIt != aggnameIt->second.end())
                        {
                            sstrm << std::string(1, delim);
                            sstrm << smbIt->second[0];
                        }
                    }
                }
                out_str = sstrm.str();
                std::cout << "ds writer thread to aggstreamer: " << out_str << std::endl;
                serializer->serialize(out_str, data);
                auto snd_res = pair->send(data, 64);
            }
            std::string stop_sig{"stop"};
            serializer->serialize(stop_sig, data);
            auto sig_snd_res = pair->send(data, 64);
        }
    }

    void recv()
    {
        char data[64];
        auto delim = serializer->get_delim();
        std::stringstream req_strm;
        std::string req_str;
        std::string aggset_name;
        std::string key;
        int num_vals;
        int ds_id;
        std::string hostname;
        std::string port;
        std::string ready{"ready"};
        while (capacity > 0)
        {
            std::cout << "waiting for " << capacity << " aggs to connect" << std::endl;
            req_strm.str("");
            pull->recv(data, 64);
            serializer->deserialize_one(data, req_str, 0);
            if (req_str == "conn")
            {
                serializer->deserialize_one(data, aggset_name, 1);
                serializer->deserialize_one(data, hostname, 2);
                serializer->deserialize_one(data, port, 3);
                pairs.push_back(factory->make_pair(ZMQHandle::SEND));
                pairs[pairs.size() - 1]->connect("*", port);
                serializer->serialize("ready" + std::string(1, delim) + std::to_string(pairs.size() - 1), data);
                pairs[pairs.size() - 1]->send(data, 64);
                DB[aggset_name] = std::unordered_map<std::string, std::vector<long double>>{};
                --capacity;
            }
        }
        std::cout << "all aggs connected" << std::endl;
        while (true)
        {
            req_strm.str("");
            pull->recv(data, 64);
            serializer->deserialize_one(data, aggset_name, 0);
            serializer->deserialize_one(data, key, 1);
            serializer->deserialize_one(data, num_vals, 2);
            serializer->deserialize_one(data, ds_id, 3);
            std::cout << "received req in db: "
                      << aggset_name << ", " << key << ", " << num_vals << std::endl;
            auto it = DB.find(aggset_name);
            if (it != DB.end())
            {
                auto keyIt = it->second.find(key);
                if (keyIt == it->second.end())
                {
                    req_strm << 0;
                    for (int i = 1; i < num_vals; ++i)
                    {
                        req_strm << delim;
                        req_strm << 0;
                    }
                    keyIt = it->second.insert({key, std::vector<long double>(10, 0)}).first;
                    if (unique_symbols.count(key) == 0)
                    {
                        unique_symbols.insert(key);
                        sorted_symbols.insert(std::upper_bound(sorted_symbols.begin(), sorted_symbols.end(), key), key);
                    }
                }
                else
                {
                    req_strm << keyIt->second[0];
                    for (int i = 1; i < num_vals; ++i)
                    {
                        req_strm << delim;
                        req_strm << keyIt->second[i];
                    }
                }
                req_str = req_strm.str();
                serializer->serialize(req_str, data);
                pairs[ds_id]->send(data, 64);
                pairs[ds_id]->recv(data, 64);
                for (int i = 0; i < num_vals; ++i)
                {
                    serializer->deserialize_one(data, keyIt->second[i], i);
                }
            }

        }
    }

};
