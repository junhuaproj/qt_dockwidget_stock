#include "netrequest.h"

NetRequest::NetRequest(INetResponse* response,QObject *parent)
    : NetBaseRequest(parent),pResponse(response)
{}

NetRequest::~NetRequest()
{

}

bool NetRequest::requestUrl(const QString& url,uint64_t data1,uint64_t data2)
{
    this->data1=data1;
    this->data2=data2;
    if(reply)return false;
    reply=networkManager.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(onRequestFinished()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onRequestReady()));
    return true;
}


void NetRequest::onRequestReady()
{
    pResponse->dataRecieved(this,reply->readAll());
}
void NetRequest::onRequestFinished()
{
    if(reply)
    {
        delete reply;
        reply=NULL;

        emit requestFinished(this,true);

    }
}
