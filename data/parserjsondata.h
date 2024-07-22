/**
 * 解析JSON数据,包括从网络下载的和本地存放的JSON文件
*/
#ifndef PARSERJSONDATA_H
#define PARSERJSONDATA_H

#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "stockinformation.h"

class ParserJsonData
{
public:
    ParserJsonData();
    bool getFileContent(const QString& path,QByteArray& content);
    bool getJsonArray(const QByteArray& content,QJsonArray& arr);
    bool getJsonObj(const QByteArray& content,QJsonObject& obj);
    void parserTableStockProp(const QString& path,QStringList& titles);
    bool parserStockInfo(const QString& path,StockInformation& info);
};

#endif // PARSERJSONDATA_H
