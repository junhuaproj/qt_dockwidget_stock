#include "parserrecordlist.h"
#include <QFileDialog>
#include "dataloader.h"
#include <QJsonDocument>

ParserRecordList::ParserRecordList(data::DataLoader* dataLoader)
    :pDataLoader(dataLoader)
{

}

bool ParserRecordList::openAndParseFile(QList<data::StockRecordItem*>& recorditems,QWidget *parent)
{
    QString filePath=QFileDialog::getOpenFileName(parent);
    if(filePath.length()==0)
        return false;
    QByteArray content;
    if(openFile(filePath,content))
    {
        return parser(content,recorditems);
    }
    return false;
}

bool ParserRecordList::openFile(const QString& filePath,QByteArray& content)
{
    QFile f(filePath);
    if(f.open(QFile::ReadOnly))
    {
        content=f.readAll();
        f.close();
        return true;
    }
    return false;
}
bool ParserRecordList::parser(const QByteArray& content,QList<data::StockRecordItem*>& recorditems)
{
    QJsonDocument doc=QJsonDocument::fromJson(content);
    if(!doc.isArray())
        return false;
    return parser(doc.array(),recorditems);
}

data::StockRecordItem* ParserRecordList::parser(const QJsonObject& obj)
{
    data::StockRecordItem* item=new data::StockRecordItem();
    item->setStock(pDataLoader->findStock(obj.value("code").toString()));

    item->setDownPercent(obj.value("downper").toDouble());
    item->setDownDay(obj.value("downday").toInt());
    item->setLastKeyVal(obj.value("lastj").toDouble());
    return item;
}

bool ParserRecordList::parser(const QJsonArray& items,QList<data::StockRecordItem*>& recorditems)
{
    int count=items.size();
    data::StockRecordItem* item;
    for(int i=0;i<count;i++)
    {
        const QJsonObject obj=items.at(i).toObject();
        item=parser(obj);
        recorditems.push_back(item);
    }
    return true;
}
