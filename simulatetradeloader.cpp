#include "simulatetradeloader.h"
#include "QNetworkRequest"
namespace simulate{
const char* simulate_url_root="http://localhost:5000";
SimulateTradeLoader::SimulateTradeLoader(QObject *parent)
    : QObject(parent),replyTrade(NULL)
{

}
SimulateTradeLoader::~SimulateTradeLoader()
{
    if(replyTrade)
        delete replyTrade;
}

const QByteArray* SimulateTradeLoader::getData()const
{
    return &data;
}

bool SimulateTradeLoader::requestUrl(const QString& url)
{
    if(replyTrade)return false;
    QString fullUrl=QString("%1//%2").arg(simulate_url_root,url);
    replyTrade=networkManager.get(QNetworkRequest(fullUrl));
    connect(replyTrade,SIGNAL(finished()),this,SLOT(onRequestFinished()));
    connect(replyTrade,SIGNAL(readyRead()),this,SLOT(onRequestReady()));
    return true;
}

void SimulateTradeLoader::onRequestReady()
{
    data=replyTrade->readAll();
    emit requestResult(this,true);
}
void SimulateTradeLoader::onRequestFinished()
{
    if(replyTrade)
    {
        delete replyTrade;
        replyTrade=NULL;
    }
}
}
