#include "netbaserequest.h"

NetBaseRequest::NetBaseRequest(QObject *parent)
    :QObject(parent), reply(NULL),data1(0),data2(0)
{

}

NetBaseRequest::~NetBaseRequest()
{
    if(reply)
        delete reply;
}
bool NetBaseRequest::isBusy()
{
    return reply!=NULL;
}
uint64_t NetBaseRequest::getData1() const
{
    return data1;
}

uint64_t NetBaseRequest::getData2() const
{
    return data2;
}

void NetBaseRequest::setData2(uint64_t newData2)
{
    data2 = newData2;
}

void NetBaseRequest::setData1(uint64_t newData1)
{
    data1 = newData1;
}
