/*
 * 模拟交易请求与加载
*/
#ifndef SIMULATETRADELOADER_H
#define SIMULATETRADELOADER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
namespace simulate{
class SimulateTradeLoader : public QObject
{
    Q_OBJECT
public:
    explicit SimulateTradeLoader(QObject *parent = nullptr);
    virtual ~SimulateTradeLoader();
    const QByteArray* getData()const;
    bool requestUrl(const QString& url);
protected:
    QByteArray data;
    QNetworkReply *replyTrade;
    QNetworkAccessManager networkManager;
protected slots:
    void onRequestReady();
    void onRequestFinished();
signals:
    void requestResult(SimulateTradeLoader* pLoader,bool result);
};
}
#endif // SIMULATETRADELOADER_H
