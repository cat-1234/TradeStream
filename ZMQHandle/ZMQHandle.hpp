#pragma once

#include <string>
#include <zmq.h>

class ZMQHandle
{
public:
    enum Type { SEND, RECEIVE };
protected:
    std::string protocol;
    void* context;
    void* socket;
    Type type;
public:
    ZMQHandle(const std::string& protocol, int type, Type snd_recv_type) : protocol(protocol), type(snd_recv_type)
    {
        context = zmq_ctx_new();
        socket = zmq_socket(context, type);
    }

    ZMQHandle(const ZMQHandle&) = delete;
    ZMQHandle& operator=(const ZMQHandle&) = delete;
    ZMQHandle(ZMQHandle&&) = default;
    ZMQHandle& operator=(ZMQHandle&&) = default;

    virtual int connect(const std::string&, const std::string&) = 0;
    virtual int send(void*, int, int flags = 0) { return -1; }
    virtual int recv(void*, int, int flags = 0) { return -1; }
    virtual int disconnect(const std::string&, const std::string&) { return -1; }

    virtual ~ZMQHandle()
    {
        zmq_close(socket);
        zmq_ctx_destroy(context);
    }
};
