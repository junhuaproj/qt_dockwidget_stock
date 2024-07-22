#include "stockinformation.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

StockInformation::StockInformation() {}



void StockInformation::setName(const QString& name)
{
    this->name=name;
}

const QString& StockInformation::getCode()
{
    return code;
}
const QString& StockInformation::getName()
{
    return name;
}
const QString& StockInformation::getKeywords()
{
    return keywords;
}
const QString& StockInformation::getScope()
{
    return scope;
}
const QString& StockInformation::getRemind()
{
    return remind;
}
const QString& StockInformation::getProfile()
{
    return profile;
}
const QString& StockInformation::getInternel()
{
    return internel;
}
const QString& StockInformation::getSummary()
{
    return summary;
}

const QString& StockInformation::getIndex() const
{
    return index;
}

void StockInformation::setIndex(const QString &newIndex)
{
    index = newIndex;
}

const QString& StockInformation::getNotions() const
{
    return notions;
}

void StockInformation::setNotions(const QString &newNotions)
{
    notions = newNotions;
}

const QString& StockInformation::getBussiness() const
{
    return bussiness;
}

void StockInformation::setBussiness(const QString &newBussiness)
{
    bussiness = newBussiness;
}
void StockInformation::toHtmlText(QString& out)
{
    out=out.append("<ul>");
    out=out.append("<li>"+code+"</li>");
    out=out.append("<li>"+keywords+"</li>");
    out=out.append("<li>"+index+"</li>");
    out=out.append("<li>"+notions+"</li>");
    out=out.append("<li>"+bussiness+"</li>");
    out=out.append("<li>"+scope+"</li>");
    out=out.append("<li>"+remind+"</li>");
    out=out.append("<li>"+profile+"</li>");
    out=out.append("<li>"+internel+"</li>");
    out=out.append("<li>"+summary+"</li>");
    out=out.append("</ul>");
}

void StockInformation::setCode(const QString &newCode)
{
    code = newCode;
}

void StockInformation::setKeywords(const QString &newKeywords)
{
    keywords = newKeywords;
}

void StockInformation::setScope(const QString &newScope)
{
    scope = newScope;
}

void StockInformation::setRemind(const QString &newRemind)
{
    remind = newRemind;
}

void StockInformation::setProfile(const QString &newProfile)
{
    profile = newProfile;
}

void StockInformation::setInternel(const QString &newInternel)
{
    internel = newInternel;
}

void StockInformation::setSummary(const QString &newSummary)
{
    summary = newSummary;
}
void StockInformation::setIndex(const QList<StockProp*>& index)
{
    propListToString(index,this->index);
}

void StockInformation::setNotions(const QList<StockProp*>& notions)
{
    propListToString(notions,this->notions);
}
void StockInformation::setBussiness(const QList<StockProp*>& bussiness)
{
    propListToString(bussiness,this->bussiness);
}
