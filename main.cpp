#include <thread>
#include <boost/program_options.hpp>
#include <gtest/gtest.h>

#include "ZMQHandleFactory/ZMQHandleFactory.hpp" // includes memory and TCPZMQHandle (and ZMQHandle) ! double !
#include "TradeStreamer/TradeStreamer.hpp"  // includes ChunkLenSerializer (and Serializer) and CSVFileHandle (and FileHandle) and TCPZMQHandle (and ZMQHandle)
//#include "DataStore/DataStore.hpp"
//#include "AggStreamer/AggStreamer.hpp"
#include "Agg/Agg.hpp"
//#include <sstream>
#include <iostream>

//void run_tradestream(const std::string& s)
//{
//    auto reader = std::make_unique<CSVFileReader>(s);
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    auto fac = std::make_shared<TCPZMQFactory>();
//    ZMQTradeStreamer td{ std::move(reader), std::move(szr), fac };
//    std::cout << "connecting streamer on *:5555" << std::endl;
//    td.connect("127.0.0.1", "5555");
//    td.stream();
//    std::this_thread::sleep_for(std::chrono::minutes(10));
//}


//void run_wtavagg(const std::string& s)
//{
//    int idx1 = std::stoi(std::string(1, s.c_str()[0]));
//    int idx2 = std::stoi(std::string(1, s.c_str()[1]));
//    auto fac = std::make_shared<TCPZMQFactory>();
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    WeightedAverageAgg agg{"wtavagg", {idx1, idx2}, fac, std::move(szr) };
//    agg.connect_sub("127.0.0.1", "5555"); // tradestreamer channel
//    agg.connect_push("127.0.0.1", "5557"); // db channel
//    agg.connect_pair("localhost", "5575"); // private agg channel
//    agg.connect_to_db();
//    std::cout << "wtavagg preparing to recv" << std::endl;
//    agg.recv();
//}

//void run_sumagg(const int& n)
//{
//    auto fac = std::make_shared<TCPZMQFactory>();
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    SumAgg agg{"sumagg", {n}, fac, std::move(szr) };
//    agg.connect_sub("127.0.0.1", "5555");
//    agg.connect_push("127.0.0.1", "5557");
//    agg.connect_pair("localhost", "5585");
//    agg.connect_to_db();
//    std::cout << "sumagg preparing to recv" << std::endl;
//    agg.recv();
//}

//void run_maxrangeagg(const int& n)
//{
//    auto fac = std::make_shared<TCPZMQFactory>();
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    MaxRangeAgg agg{ "maxrangeagg", {n}, fac, std::move(szr) };
//    agg.connect_sub("127.0.0.1", "5555"); // tradestreamer channel
//    agg.connect_push("127.0.0.1", "5557"); // db channel
//    agg.connect_pair("localhost", "5595"); // private agg channel
//    agg.connect_to_db();
//    std::cout << "maxrangeagg preparing to recv" << std::endl;
//    agg.recv();
//}

//void run_maxagg(const int& n)
//{
//    auto fac = std::make_shared<TCPZMQFactory>();
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    MaxAgg agg{ "maxagg", {n}, fac, std::move(szr) };
//    agg.connect_sub("127.0.0.1", "5555"); // tradestreamer channel
//    agg.connect_push("127.0.0.1", "5557"); // db channel
//    agg.connect_pair("localhost", "5565"); // private agg channel
//    agg.connect_to_db();
//    std::cout << "maxagg preparing to recv" << std::endl;
//    agg.recv();
//}


//void run_ds()
//{
//    auto szr = std::make_shared<VarChunksSerializer>(',');
//    auto fac = std::make_shared<TCPZMQFactory>();
//    DataStore ds{ "127.0.0.1", "5557", szr, fac };
//    std::thread writer_listener{&DataStore::start_writer_thread, &ds, "127.0.0.1", "5558"}; // 5558 is for writer
//    writer_listener.detach();
//    ds.recv();
//}

//void run_aggstream(const std::string& s, const std::vector<std::string> aggnames)
//{
//    auto writer = std::make_unique<CSVFileWriter>(s);
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    auto fac = std::make_shared<TCPZMQFactory>();
//    ZMQAggStreamer astr{std::move(writer), std::move(szr), fac, "127.0.0.1", "5550", aggnames};
//    astr.connect("127.0.0.1", "5558"); // req from db
//    astr.stream();
//}

//void parse_tokens(const std::string& str, std::vector<std::string>& vec, const char& delim)
//{
//    std::stringstream sstrm;
//    for (int i = 0; i < str.size(); ++i)
//    {
//        if (str[i] == ' ')
//        {
//            vec.push_back(sstrm.str());
//            sstrm.str("");
//        }
//        else
//            sstrm << str[i];
//    }
//    vec.push_back(sstrm.str());
//}


int main(int ac, char** av)
{
//    namespace po = boost::program_options;
//    po::options_description desc("options");


//    std::vector<std::string> aggnames;
//
//
//    desc.add_options()
//            ("help", "provide description")
//            ("tradestream", po::value<std::string>(), "run tradestream with preconfigures params from filename provided")
//            ("wtavagg", po::value<std::string>(), "run weighted average agg on index of trade line chosen")
//            ("sumagg", po::value<int>(), "run sum agg on index chosen")
//            ("maxrangeagg", po::value<int>(), "run max range agg on idx provided")
//            ("maxagg", po::value<int>(), "run max agg on idx")
//            ("ds", "run data store node")
//            ("aggstream", po::value<std::string>(), "run aggstreamer to file provided")
//            ("aggnames", po::value<std::string>(), "aggset names for aggstreamer");
//    po::variables_map vm;
//    po::store(po::parse_command_line(ac, av, desc), vm);
//    po::notify(vm);
//    if (vm.count("help"))
//        std::cout << desc << std::endl;
//    if (vm.count("tradestream"))
//        run_tradestream(vm["tradestream"].as<std::string>());
//    if (vm.count("wtavagg"))
//        run_wtavagg(vm["wtavagg"].as<std::string>());
//    if (vm.count("sumagg"))
//        run_sumagg(vm["sumagg"].as<int>());
//    if (vm.count("maxrangeagg"))
//        run_maxrangeagg(vm["maxrangeagg"].as<int>());
//    if (vm.count("maxagg"))
//        run_maxagg(vm["maxagg"].as<int>());
//    if (vm.count("ds"))
//        run_ds();
//    if (vm.count("aggstream"))
//    {
//        if (!vm.count("aggnames"))
//        {
//            std::cout << "please enter aggnames for aggstreamer to request from db" << std::endl;
//            return 1;
//        }
//        std::vector<std::string> names;
//        parse_tokens(vm["aggnames"].as<std::string>(), names, ' ');
//        run_aggstream(vm["aggstream"].as<std::string>(), std::move(names));
//    }
//



    ::testing::InitGoogleTest();
    auto res = RUN_ALL_TESTS();
    std::cout << "tests res: " << res << std::endl;
    return 0;
}

//
//class FileHandlersTest : public ::testing::Test
//{
//protected:
//    void SetUp() override
//    {}
//
//    void TearDown() override
//    {}
//};


//TEST(BasicTests, TestGtest)
//{
//    int n{5};
//    double m{3.01};
//    EXPECT_EQ(n, 5);
//    EXPECT_DOUBLE_EQ(m, 3.01);
//}
//
//
//TEST_F(FileHandlersTest, TestReaderIfstreamHandling)
//{
//
//    auto rdr = CSVFileReader{"file.csv"};
//}
//
//
//TEST_F(FileHandlersTest, TestReaderReadLine)
//{
//    auto rdr = CSVFileReader{"file.csv"};
//    std::string s;
//    rdr.readline(s);
//    EXPECT_STREQ(s.c_str(), "123456,aaa,10,12");
//}
//
//
//TEST_F(FileHandlersTest, TestReaderReadLineStop)
//{
//    auto rdr = CSVFileReader{"file.csv"};
//    std::string s;
//    auto res1 = rdr.readline(s);
//    auto res2 = rdr.readline(s);
//    auto res3 = rdr.readline(s);
//    auto res4 = rdr.readline(s);
//    EXPECT_TRUE(res1);
//    EXPECT_TRUE(res2);
//    EXPECT_TRUE(res3);
//    EXPECT_FALSE(res4);
//}
//
//TEST_F(FileHandlersTest, TestWriteLine)
//{
//    auto wrtr = CSVFileWriter{"file1.csv"};
//    const int size{18};
//    char td1[size]{"1234567,ccc,20,15"};
//    char td2[size]{"1234568,ddd,15,25"};
//    wrtr.writeline(td1, size - 1);
//    wrtr.writeline(td2, size - 1);
//}
//
//
class SerializersTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        szr = std::make_unique<ChunkLenSerializer>(',', 4);
        vcs = std::make_unique<VarChunksSerializer>(',');
        td = "1234567,ccc,20,15";
        td_fp = "1234567,zzz,20.00001,15.953";
        td_fp2 = "abc,12345,2.0543,1.8567,9";
        td_ser = "1234567ccc2015";
    }

    void TearDown() override
    {}

    std::unique_ptr<ChunkLenSerializer> szr;
    std::unique_ptr<VarChunksSerializer> vcs;
    std::string td;
    std::string td_fp;
    std::string td_fp2;
    std::string td_ser;
};



TEST_F(SerializersTest, SerializeTradeString) {
    char data[td.size() + 1];
    auto res = szr->serialize(td, data);

    EXPECT_TRUE(res);
    EXPECT_EQ(data[0], 7);
    EXPECT_EQ(data[1], 10);
    EXPECT_EQ(data[2], 12);
    EXPECT_EQ(data[3], 14);

    for (int i = 4; i < td_ser.size() + 4; ++i)
    {
        EXPECT_EQ(data[i], td_ser[i - 4]);
    }
}

TEST_F(SerializersTest, DeserializeCharArrayToString)
{
    char data[td.size() + 1];
    auto res = szr->serialize(td, data);
    EXPECT_TRUE(res);

    std::string s(td.size(), '\0');
    auto res2 = szr->deserialize(data, s);
    EXPECT_TRUE(res2);
    EXPECT_STREQ(s.c_str(), td.c_str());
}

TEST_F(SerializersTest, DeserializeOneStringIntFloat)
{
    char data[td.size() + 1];
    szr->serialize(td, data);
    std::string s(7, '\0');
    szr->deserialize_one(data, s, 0);
    EXPECT_STREQ(s.c_str(), "1234567");
    std::string s2(2, '\0');
    szr->deserialize_one(data, s2, 3);
    EXPECT_STREQ(s2.c_str(), "15");
    int n;
    szr->deserialize_one(data, n, 0);
    EXPECT_EQ(n, 1234567);
    float f;
    szr->deserialize_one(data, f, 2);
    EXPECT_FLOAT_EQ(f, 20.0f);
}

TEST_F(SerializersTest, VarChunksSerializerTest)
{
    VarChunksSerializer vcs1{','};
    std::string s{"avagg,f,i,15,12,abc,d"};
    char data1[64];
    auto res1 = vcs1.serialize(s, data1);
    EXPECT_FALSE(res1);
    VarChunksSerializer vcs2{',', 7};
    char data2[64];
    auto res2 = vcs2.serialize(s, data2);
    EXPECT_TRUE(res2);
    char expected_meta[]{7, 5, 6, 7, 9, 11, 14, 15};
    std::string expected_data{"avaggfi1512abcd"};
    for (int i = 0; i < 8; ++i)
        EXPECT_EQ(data2[i], expected_meta[i]);
    for (int i = 8; i < s.size() + 3; ++i)
        EXPECT_EQ(data2[i], expected_data[i - 8]);
}

TEST_F(SerializersTest, VarChunksDeserializeTest)
{
    VarChunksSerializer vcs{',', 6};
    std::string s{"avagg,f,i,15,12,abc"};
    char data[64];
    auto res1 = vcs.serialize(s, data);
    EXPECT_TRUE(res1);
    std::string s2(s.size(), '\0');
    auto res2 = vcs.deserialize(data, s2);
    EXPECT_TRUE(res2);
    EXPECT_STREQ(s2.c_str(), s.c_str());
}

TEST_F(SerializersTest, VarChunksDeserializeOne)
{
    VarChunksSerializer vcs{','};
    std::string s{"1234567,aaa,1.2345,10,10"};
    char data[64];
    vcs.serialize(s, data);
    float f;
    vcs.deserialize_one(data, f, 2);
    EXPECT_FLOAT_EQ(f, 1.2345f);
    int n;
    vcs.deserialize_one(data, n, 0);
    EXPECT_EQ(n, 1234567);
    double d;
    vcs.deserialize_one(data, d, 4);
    EXPECT_DOUBLE_EQ(d, 10.0);
}

TEST_F(SerializersTest, VCSDeserializeInt)
{
    char data[64];
    vcs->serialize(td_fp, data);
    std::string s;
    vcs->deserialize_int(data, s);
    EXPECT_STREQ(s.c_str(), "1234567,zzz,20,15");
}

//
//class ZMQHandlersTest : public ::testing::Test
//{
//protected:
//    void SetUp() override
//    {
//        hostname = "localhost";
//        port1 = "5555";
//        port2 = "5556";
//        port3 = "5557";
//    }
//    void TearDown() override
//    {}
//
//    std::string hostname;
//    std::string port1;
//    std::string port2;
//    std::string port3;
//};
//
//
//TEST_F(ZMQHandlersTest, TCPZMQPushAndPull)
//{
//    TCPZMQPull pull;
//    pull.connect("localhost", "5555");
//    char data[1];
//
//    std::thread th{
//        []
//        {
//            TCPZMQPush push;
//            push.connect("*", "5555");
//            char data[1]{'c'};
//            push.send(data, 1);
//        }};
//    th.detach();
//    pull.recv(data, 1);
//    EXPECT_EQ(data[0], 'c');
//}
//
//TEST_F(ZMQHandlersTest, TCPZMQPair)
//{
//    TCPZMQPair p{TCPZMQPair::SEND};
//    p.connect("*", "5555");
//    char data[1]{'c'};
//    std::thread recv_th{[this]
//                        {
//                            TCPZMQPair p{TCPZMQPair::RECEIVE};
//                            p.connect("localhost", "5555");
//                            char data[1];
//                            p.recv(data, 1);
//                            EXPECT_EQ(data[0], 'c');
//                        }};
//    recv_th.detach();
//    p.send(data, 1);
//}
//
//TEST_F(ZMQHandlersTest, ZMQFactory)
//{
//    std::shared_ptr<ZMQHandleFactory> factory = std::make_unique<TCPZMQFactory>();
//    std::shared_ptr<ZMQHandle> push = factory->make_push();
//    std::shared_ptr<ZMQHandle> pull = factory->make_pull();
//    std::shared_ptr<ZMQHandle> pair_snd = factory->make_pair(ZMQHandle::SEND);
//    std::shared_ptr<ZMQHandle> pair_rcv = factory->make_pair(ZMQHandle::RECEIVE);
//    EXPECT_TRUE(std::dynamic_pointer_cast<TCPZMQFactory>(factory));
//    EXPECT_TRUE(std::dynamic_pointer_cast<TCPZMQPush>(push));
//    EXPECT_TRUE(std::dynamic_pointer_cast<TCPZMQPull>(pull));
//    EXPECT_TRUE(std::dynamic_pointer_cast<TCPZMQPair>(pair_snd));
//    EXPECT_TRUE(std::dynamic_pointer_cast<TCPZMQPair>(pair_rcv));
//}
//
//
//class TradeStreamerTest : public ::testing::Test
//{
//protected:
//    void SetUp()
//    {
//        trades.emplace_back("123456,aaa,10,12");
//        trades.emplace_back("123457,ccc,10,12");
//        trades.emplace_back("123458,aab,10,12");
//        fac = std::make_shared<TCPZMQFactory>();
//    }
//
//    void TearDown()
//    {}
//
//    std::vector<std::string> trades;
//    std::shared_ptr<ZMQHandleFactory> fac;
//};
//
//
//TEST_F(TradeStreamerTest, ConstructTradeStreamer)
//{
//    std::unique_ptr<FileReader> rdr = std::make_unique<CSVFileReader>("file.csv");
//    std::unique_ptr<Serializer> szr = std::make_unique<ChunkLenSerializer>(4, ',');
//    ZMQTradeStreamer ts{std::move(rdr), std::move(szr), fac };
//    ts.connect("*", "5555");
//    std::thread pull_th{[this]
//                        {
//                            ChunkLenSerializer szr(4, ',');
//                            std::string s;
//                            TCPZMQPull pull;
//                            pull.connect("localhost", "5555");
//                            char data[64];
//                            int count = 0;
//                            while (count < 3)
//                            {
//                                pull.recv(data, 64);
//                                szr.deserialize(data, s);
//                                EXPECT_STREQ(s.c_str(), trades.at(count).c_str());
//                                count++;
//                            }
//                        }};
//    pull_th.detach();
//    ts.stream();
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//}

// std::cout tests because of nature of this code

//TEST_F(AggsTest, AggConnectPull)
//{
//    auto s = std::make_unique<VarChunksSerializer>(',');
//    AverageAgg agg{"avagg", 2, fac, std::move(s)};
//    agg.connect_sub("localhost", "5555");
//    std::thread th{[] {
//        VarChunksSerializer szr{','};
//        TCPZMQPush p;
//        p.connect("*", "5555");
//        char data[64];
//        std::string s{"aaa,bbb,123"};
//        szr.serialize(s, data);
//        p.send(data, 64);
//    }};
//    th.detach();
//    agg.recv();
//}

//TEST_F(AggsTest, AggReceiveStream)
//{
//    auto szr = std::make_unique<VarChunksSerializer>(',');
//    AverageAgg agg{"avagg", 2, fac, std::move(szr)};
//    agg.connect_sub("localhost", "5555");
//    std::thread th{[this]
//                   {
//                       std::unique_ptr<FileReader> rdr = std::make_unique<CSVFileReader>("file.csv");
//                       std::unique_ptr<Serializer> szr = std::make_unique<VarChunksSerializer>(',');
//                       std::unique_ptr<ZMQHandle> push = std::make_unique<TCPZMQPush>();
//                       ZMQTradeStreamer ts{std::move(rdr), std::move(szr), fac };
//                       ts.connect("*", "5555");
//                       ts.stream();
//                   }};
//    th.detach();
//    agg.recv();
//}

//
class AggsTest : public ::testing::Test
{
protected:
    void SetUp()
    {
        fac = std::make_shared<TCPZMQFactory>();
        serializer = std::make_unique<VarChunksSerializer>(',');
    }
    void TearDown()
    {}
    std::shared_ptr<ZMQHandleFactory> fac{nullptr};
    std::unique_ptr<Serializer> serializer;
};
//
TEST_F(AggsTest, StatefulAggsTest)
{
    auto ts = ZMQTradeStreamer{std::make_unique<CSVFileReader>("input.csv"),
                               std::make_unique<VarChunksSerializer>(','),
                               fac};
    ts.connect("*", "5555");
    std::thread th{
        [this]
        {
            auto avagg = WeightedAverageAgg{
                    {3, 2}, fac, std::make_unique<VarChunksSerializer>(',')
            };
            avagg.connect_sub("127.0.0.1", "5555");
            avagg.recv();
        }
    };
    th.detach();
    ts.stream();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

//
//
//TEST_F(AggsTest, ConnectToDb)
//{
//    TCPZMQPull db_pull;
//    db_pull.connect("localhost", "5555");
//    TCPZMQPair db_pair{ZMQHandle::RECEIVE};
//    char data[64];
//    std::string port;
//    std::string response{"ready"};
//    char res_data[64];
//    serializer->serialize(response, res_data);
//    std::thread agg_thread{[this]
//                           {
//                               AverageAgg agg{"avagg", {2}, fac, std::make_unique<VarChunksSerializer>(',')};
//                               agg.connect_push("*", "5555");
//                               agg.connect_pair("*", "5556");
//                               agg.connect_to_db();
//                           }};
//    agg_thread.detach();
//    db_pull.recv(data, 64);
//    serializer->deserialize_one(data, port, 2);
//    db_pair.connect("localhost", port);
//    db_pair.send(res_data, 64);
//}
//
//TEST_F(AggsTest, AverageAggCalc)
//{
//    float val{5.0};
//    float prev_val{10.0};
//    int N{1};
//    auto avFunc = [&val, &prev_val, &N]
//                    {
//                        val = ((prev_val * N) + val) / (N + 1);
//                        ++N;
//                    };
//    avFunc();
//    EXPECT_EQ(N, 2);
//    EXPECT_FLOAT_EQ(val, 7.5);
//    EXPECT_FLOAT_EQ(prev_val, 10.0);
//}
//
//TEST_F(AggsTest, SumAggTest)
//{
//    int val{15};
//    int prev_val{100};
//    auto sumFunc = [&val, &prev_val]
//                    {
//                        val += prev_val;
//                    };
//    sumFunc();
//    EXPECT_EQ(val, 115);
//    EXPECT_EQ(prev_val, 100);
//}
//
//TEST_F(AggsTest, MaxRangeTest)
//{
//    int val{15};
//    int prev_val{5};
//    int last_recorded_val{9};
//    auto maxRangeFunc = [&val, &prev_val, &last_recorded_val]
//                        {
//                            if (val - last_recorded_val > prev_val)
//                                val = val - last_recorded_val;
//                            else
//                                val = prev_val;
//                        };
//    maxRangeFunc();
//    EXPECT_EQ(val, 6);
//    EXPECT_EQ(prev_val, 5);
//    EXPECT_EQ(last_recorded_val, 9);
//
//    val = 17;
//    prev_val = 6;
//    last_recorded_val = 15;
//    maxRangeFunc();
//    EXPECT_EQ(val, 6);
//    EXPECT_EQ(prev_val, 6);
//    EXPECT_EQ(last_recorded_val, 15);
//}
//
//TEST_F(AggsTest, GetDBData)
//{
//
//}
//
//
//TEST_F(AggsTest, AggConnectPushPair)
//{
//    auto s = std::make_unique<VarChunksSerializer>(',');
//    auto factory = std::make_shared<TCPZMQFactory>();
//    auto agg = AverageAgg{"avagg", {2}, factory, std::move(s)};
//    auto res1 = agg.connect_push("*", "5555");
//    EXPECT_EQ(res1, 0);
//    auto res2 = agg.connect_pair("*", "5556");
//    EXPECT_EQ(res2, 0);
//}
//
//
//class AggStreamerTest : public ::testing::Test
//{
//protected:
//    void SetUp()
//    {
//        fac = std::make_shared<TCPZMQFactory>();
//    }
//
//    void TearDown()
//    {}
//
//    static std::unique_ptr<FileWriter> get_writer(const std::string& filename)
//    {
//        return std::make_unique<CSVFileWriter>(filename);
//    }
//
//    static std::unique_ptr<Serializer> get_szr(const char& delim)
//    {
//        return std::make_unique<VarChunksSerializer>(delim);
//    }
//
//    std::shared_ptr<ZMQHandleFactory> fac{nullptr};
//};
//
//TEST_F(AggStreamerTest, ConstructAggsStreamer)
//{
//    auto writer = get_writer("file4.csv");
//    auto szr = get_szr(',');
//    ZMQAggStreamer streamer{std::move(writer), std::move(szr), fac,
//                            "localhost", "5556", {"avagg"}};
//
//    std::thread pull_pair_th
//    {
//        []
//        {
//            TCPZMQPull pull;
//            TCPZMQPair pair(ZMQHandle::SEND);
//            pull.connect("localhost", "5555");
//            pair.connect("*", "5556");
//            char data[64];
//            while (1)
//            {
//                pull.recv(data, 64);
//                pair.send(data, 64);
//            }
//        }
//    };
//    std::thread tdstrm_th{
//        [this]
//        {
//            ZMQTradeStreamer td{std::make_unique<CSVFileReader>("file.csv"),
//                                std::make_unique<VarChunksSerializer>(','),
//                                fac};
//            td.connect("*", "5555");
//            td.stream();
//        }
//    };
//    pull_pair_th.detach();
//    tdstrm_th.detach();
//
////    streamer.connect("*", "5555");
//    streamer.stream();
//}

//TEST_F(AggStreamerTest, GetReqStr)
//{
//
//}
//
//class DataStoreTest : public ::testing::Test
//{
//protected:
//    void SetUp()
//    {
//        szr = std::make_shared<VarChunksSerializer>(',');
//        fac = std::make_shared<TCPZMQFactory>();
//    }
//
//    void TearDown()
//    {}
//    std::shared_ptr<Serializer> szr;
//    std::shared_ptr<ZMQHandleFactory> fac;
//};
//
//TEST_F(DataStoreTest, ConstructDataStore)
//{
//    DataStore ds{ "localhost", "5555", szr, fac };
//}
//
//TEST_F(DataStoreTest, DSAssignMaps)
//{
//    DataStore ds{ "localhost", "5555", szr, fac };
//
//}

//TEST_F(DataStoreTest, DsWithAggStreamer)
//{
//    DataStore ds{ "localhost", "5555", "127.0.0.1", "5558", szr, fac };
//    ds.start_writer_thread();
//    auto f = [this]
//    {
//        TCPZMQPushMulti push;
//        TCPZMQPair pair_recv{ZMQHandle::RECEIVE};
//        auto push_conn_res = push.connect("127.0.0.1", "5558");
//        std::cout << "push connect result: " << push_conn_res << std::endl;
//        auto pair_conn_result = pair_recv.connect("127.0.0.1", "5559");
//        std::cout << "pair conn res: " << pair_conn_result << std::endl;
//
//        char data[64];
//        std::string response;
//
//        std::string conn_req{"conn,127.0.0.1,5559,avagg"};
//        VarChunksSerializer szr{','};
//        szr.serialize(conn_req, data);
//        auto push_snd_res = push.send(data, 64);
//        std::cout << "push snd res: " << push_snd_res << std::endl;
//        int recv_res = 1;
//        while ((recv_res = pair_recv.recv(data, 64)) > 0) {
//            szr.deserialize(data, response);
//            std::cout << response << std::endl;
//        }
//    };
//    std::thread th{f};
//    th.detach();
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//}


























