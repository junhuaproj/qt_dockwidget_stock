/**
 * 用于请求模拟交易的，HTTP地址
 */
#ifndef HTTPADDRESSITEM_H
#define HTTPADDRESSITEM_H
#include <QString>
class HttpAddressItem
{
public:
    HttpAddressItem();
    HttpAddressItem(int id,const QString& name,const QString& address);
    int id;
    QString name;
    QString address;
};

#endif // HTTPADDRESSITEM_H
