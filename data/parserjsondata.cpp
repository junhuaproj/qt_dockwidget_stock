#include "parserjsondata.h"
#include <QFile>
ParserJsonData::ParserJsonData() {}

bool ParserJsonData::getFileContent(const QString& path,QByteArray& content)
{
    QFile f(path);
    if(f.open(QFile::ReadOnly))
    {
        content=f.readAll();
        f.close();
        return true;
    }
    return false;
}
bool ParserJsonData::getJsonArray(const QByteArray& content,QJsonArray& arr)
{
    QJsonDocument doc=QJsonDocument::fromJson(content);
    if(doc.isArray())
    {
        arr=doc.array();
        return true;
    }
    return false;
}

bool ParserJsonData::getJsonObj(const QByteArray& content,QJsonObject& obj)
{
    QJsonDocument doc=QJsonDocument::fromJson(content);
    if(doc.isObject())
    {
        obj=doc.object();
        return true;
    }
    return false;
}
void ParserJsonData::parserTableStockProp(const QString& path,QStringList& titles)
{
    QByteArray content;
    if(!getFileContent(path,content))
        return ;
    QJsonArray arr;
    if(!getJsonArray(content,arr))
        return ;
    int count=arr.size();
    for(int i=0;i<count;i++)
    {
        titles.push_back(arr.at(i).toString());
    }
}
bool ParserJsonData::parserStockInfo(const QString& path,StockInformation& info)
{
    QByteArray content;
    if(!getFileContent(path,content))
        return false;
    QJsonObject obj;
    if(!getJsonObj(content,obj))
        return false ;
    if(obj.contains("code"))
        info.setCode(obj["code"].toString());
    //if(obj.contains("name"))
    //    info.setName(obj["name"].toString());
    if(obj.contains("summary"))
        info.setSummary(obj["summary"].toString());
    if(obj.contains("profile"))
        info.setProfile(obj["profile"].toString());
    if(obj.contains("internel"))
        info.setInternel(obj["internel"].toString());
    if(obj.contains("remind"))
        info.setRemind(obj["remind"].toString());
    if(obj.contains("scope"))
        info.setScope(obj["scope"].toString());
    if(obj.contains("keywords"))
        info.setKeywords(obj["keywords"].toString());
    return true;
}
