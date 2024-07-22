/**
 * 后台运行线程的基类，用于HTTP请求返回数据
*/
#ifndef BGRUNNABLE_H
#define BGRUNNABLE_H

#include <QRunnable>
namespace data{
class DataLoader;
}
class NetThreadPool;
class BgRunnable : public QRunnable
{
public:
    BgRunnable(NetThreadPool* pool,data::DataLoader* dataLoader);
protected:
    data::DataLoader* pDataLoader;//全局的数据加载器
    NetThreadPool* pool;//指向线程池
};

#endif // BGRUNNABLE_H
