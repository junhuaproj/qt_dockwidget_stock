#ifndef NETREQUEST_H
#define NETREQUEST_H

#include <QObject>

#include "net/netbaserequest.h"

class NetRequest;
struct INetResponse
{
    virtual void dataRecieved(NetRequest* request,const QByteArray& data)=0;
};

class NetRequest : public NetBaseRequest
{
    Q_OBJECT
public:
    explicit NetRequest(INetResponse* response,QObject *parent = nullptr);
    virtual ~NetRequest();

    bool requestUrl(const QString& url,uint64_t data1=0,uint64_t data2=0);

protected:
    INetResponse* pResponse;
protected slots:
    void onRequestReady();
    void onRequestFinished();
signals:
    void requestFinished(NetRequest* request,bool result);
};

#endif // NETREQUEST_H
