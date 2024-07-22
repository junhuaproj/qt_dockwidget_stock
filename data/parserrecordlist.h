/*
 * 解析由Python计算分析后的得到的记录
*/
#ifndef PARSERRECORDLIST_H
#define PARSERRECORDLIST_H

#include <QByteArray>
#include <QJsonArray>
#include <QJsonObject>
#include "stockrecorditem.h"
namespace data{
class DataLoader;
}
class ParserRecordList
{
public:
    ParserRecordList(data::DataLoader* dataLoader);
    bool openAndParseFile(QList<data::StockRecordItem*>& recorditems,QWidget *parent=NULL);
    bool openFile(const QString& filePath,QByteArray& content);
    bool parser(const QByteArray& content,QList<data::StockRecordItem*>&);
    bool parser(const QJsonArray& items,QList<data::StockRecordItem*>& );
    data::StockRecordItem* parser(const QJsonObject& obj);
protected:
    data::DataLoader* pDataLoader;
};

#endif // PARSERRECORDLIST_H
