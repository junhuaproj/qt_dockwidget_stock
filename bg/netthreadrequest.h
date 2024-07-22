/*
 * 发起HTTP请求，由线程调用
*/
#ifndef NETTHREADREQUEST_H
#define NETTHREADREQUEST_H

#include "net/netbaserequest.h"
#include <QMutex>
#include <QEventLoop>

class NetThreadRequest : public NetBaseRequest
{
    Q_OBJECT
public:
    explicit NetThreadRequest(QObject *parent = nullptr);
    void request(const QString& url);

    inline bool result(){return requestResult;}
    inline QByteArray data(){return responseData;}
protected:
    // QMutex mutex;
    QEventLoop loop;
    bool requestResult;
    QByteArray responseData;
protected slots:
    void onRequestReady();
    void onRequestFinished();
};

#endif // NETTHREADREQUEST_H
