/*
 * 股票信息
*/
#ifndef STOCKINFORMATION_H
#define STOCKINFORMATION_H
#include <QString>
#include "data/stockprop.h"
class StockInformation
{
public:
    StockInformation();
    // int loadFromFile(const QString& path);
    const QString& getCode();
    const QString& getName();
    void setName(const QString& name);
    const QString& getKeywords();
    const QString& getScope();
    const QString& getRemind();
    const QString& getProfile();
    const QString& getInternel();
    const QString& getSummary();
    const QString&  getIndex() const;
    void setIndex(const QList<StockProp*>& index);
    void setNotions(const QList<StockProp*>& notions);
    void setBussiness(const QList<StockProp*>& bussiness);
    void setIndex(const QString &newIndex);

    const QString&  getNotions() const;
    void setNotions(const QString &newNotions);

    const QString& getBussiness() const;
    void setBussiness(const QString &newBussiness);

    void toHtmlText(QString& out);
    void setCode(const QString &newCode);

    void setKeywords(const QString &newKeywords);

    void setScope(const QString &newScope);

    void setRemind(const QString &newRemind);

    void setProfile(const QString &newProfile);

    void setInternel(const QString &newInternel);

    void setSummary(const QString &newSummary);

protected:
    QString code;
    QString name;
    QString keywords;
    QString scope;
    QString remind;
    QString profile;
    QString internel;
    QString summary;

    QString index;//指数
    QString notions;
    QString bussiness;
};

#endif // STOCKINFORMATION_H
