/*
 * HTTP请求
*/
#ifndef NETBASEREQUEST_H
#define NETBASEREQUEST_H

#include <QNetworkReply>
#include <QNetworkAccessManager>

class NetBaseRequest:public QObject
{
public:
    explicit NetBaseRequest(QObject *parent = nullptr);
    virtual ~NetBaseRequest();

    uint64_t getData1() const;

    uint64_t getData2() const;

    void setData2(uint64_t newData2);

    void setData1(uint64_t newData1);

    bool isBusy();

protected:
    uint64_t data1;
    uint64_t data2;

    QNetworkReply *reply;
    QNetworkAccessManager networkManager;

};

#endif // NETBASEREQUEST_H
