#pragma once


#include "ZMQHandle.hpp"


class TCPZMQHandle : public ZMQHandle
{
public:
    TCPZMQHandle(int type, Type snd_rcv_type) : ZMQHandle("tcp://", type, snd_rcv_type) {}

    virtual int connect(const std::string& hostname, const std::string& port) override
    {
        switch (type) {
            case ZMQHandle::SEND:
                return zmq_bind(socket, (protocol + hostname + ":" + port).c_str());
            case ZMQHandle::RECEIVE:
                return zmq_connect(socket, (protocol + hostname + ":" + port).c_str());
        }
    }
    virtual int disconnect(const std::string& hostname, const std::string& port) override
    {
        switch (type) {
            case SEND:
                return zmq_unbind(socket, (protocol + hostname + ":" + port).c_str());
            case RECEIVE:
            default:
                return zmq_disconnect(socket, (protocol + hostname + ":" + port).c_str());
        }
    }
    virtual ~TCPZMQHandle() = default;
};

class TCPZMQPush: public TCPZMQHandle
{
public:
    TCPZMQPush() : TCPZMQHandle(ZMQ_PUSH, SEND) {}

    int send(void* data, int size, int flags = 0) override
    {
        return zmq_send(socket, data, size, flags);
    }
};

class TCPZMQPull : public TCPZMQHandle
{
public:
    TCPZMQPull() : TCPZMQHandle(ZMQ_PULL, RECEIVE) {}

    int recv(void* data, int size, int flags = 0) override
    {
        return zmq_recv(socket, data, size, flags);
    }
};

class TCPZMQPushMulti : public TCPZMQHandle
{
public:
    TCPZMQPushMulti() : TCPZMQHandle(ZMQ_PUSH, RECEIVE) {}

    int send(void* data, int size, int flags = 0) override
    {
        return zmq_send(socket, data, size, flags);
    }

};

class TCPZMQPullMulti : public TCPZMQHandle
{
public:
    TCPZMQPullMulti() : TCPZMQHandle(ZMQ_PULL, SEND) {}

    int recv(void* data, int size, int flags = 0) override
    {
        return zmq_recv(socket, data, size, flags);
    }
};

class TCPZMQPair : public TCPZMQHandle
{
public:
    TCPZMQPair(Type type) : TCPZMQHandle(ZMQ_PAIR, type) {}

    int send(void* data, int size, int flags = 0) override
    {
        return zmq_send(socket, data, size, flags);
    }
    int recv(void* data, int size, int flags = 0) override
    {
        return zmq_recv(socket, data, size, flags);
    }
};

class TCPZMQPub : public TCPZMQHandle
{
public:
    TCPZMQPub() : TCPZMQHandle(ZMQ_PUB, SEND) {}

    int send(void* data, int size, int flags = 0) override
    {
        return zmq_send(socket, data, size, flags);
    }
};

class TCPZMQSub : public TCPZMQHandle
{
public:
    TCPZMQSub() : TCPZMQHandle(ZMQ_SUB, RECEIVE) {}

    int connect(const std::string& hostname, const std::string& port) override
    {
        auto res = TCPZMQHandle::connect(hostname, port);
        zmq_setsockopt(socket, ZMQ_SUBSCRIBE, "", 0);
        return res;
    }

    int recv(void* data, int size, int flags = 0) override
    {
        return zmq_recv(socket, data, size, flags);
    }
};
