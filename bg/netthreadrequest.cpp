#include "netthreadrequest.h"
#include <QDebug>
#include "stocklistitem.h"
NetThreadRequest::NetThreadRequest(QObject *parent)
    : NetBaseRequest(parent)
{

}
void NetThreadRequest::request(const QString& url)
{
    // mutex.lock();
    responseData.clear();
    reply=networkManager.get(QNetworkRequest(url));
    //connect(reply,SIGNAL(finished()),this,SLOT(onRequestFinished()));
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));

    connect(reply,SIGNAL(readyRead()),this,SLOT(onRequestReady()));
    //return true;
    loop.exec();
    onRequestFinished();
    // mutex.unlock();
}

void NetThreadRequest::onRequestReady()
{
    requestResult=true;
    responseData=reply->readAll();
    if(data1)
    {
        const StockListItem* item=(const StockListItem*)data1;
        qDebug()<<"code:"<<item->getCode();
    }
    qDebug()<<"receive:"<<responseData.size()<<"line:"<<__LINE__;//reply->readAll();
}

void NetThreadRequest::onRequestFinished()
{
    if(reply)
    {
        delete reply;
        reply=NULL;
    }
    // mutex.unlock();
}
