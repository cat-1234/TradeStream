#pragma once


#include "../ZMQHandle/TCPZMQHandle.hpp"
#include <memory>


class ZMQHandleFactory
{
public:
    virtual std::unique_ptr<ZMQHandle> make_push() = 0;
    virtual std::unique_ptr<ZMQHandle> make_pull() = 0;
    virtual std::unique_ptr<ZMQHandle> make_pair(ZMQHandle::Type type) = 0;
    virtual std::unique_ptr<ZMQHandle> make_pub() = 0;
    virtual std::unique_ptr<ZMQHandle> make_sub() = 0;
    virtual std::unique_ptr<ZMQHandle> make_push_multi() = 0;
    virtual std::unique_ptr<ZMQHandle> make_pull_multi() = 0;
    virtual ~ZMQHandleFactory() = default;
};

class TCPZMQFactory : public ZMQHandleFactory
{
public:
    std::unique_ptr<ZMQHandle> make_push() override
    {
        return std::make_unique<TCPZMQPush>();
    }
    std::unique_ptr<ZMQHandle> make_pull() override
    {
        return std::make_unique<TCPZMQPull>();
    }
    std::unique_ptr<ZMQHandle> make_pair(ZMQHandle::Type type) override
    {
        return std::make_unique<TCPZMQPair>(type);
    }
    std::unique_ptr<ZMQHandle> make_pub() override
    {
        return std::make_unique<TCPZMQPub>();
    }
    std::unique_ptr<ZMQHandle> make_sub() override
    {
        return std::make_unique<TCPZMQSub>();
    }
    std::unique_ptr<ZMQHandle> make_push_multi() override
    {
        return std::make_unique<TCPZMQPushMulti>();
    }
    std::unique_ptr<ZMQHandle> make_pull_multi() override
    {
        return std::make_unique<TCPZMQPullMulti>();
    }
};
