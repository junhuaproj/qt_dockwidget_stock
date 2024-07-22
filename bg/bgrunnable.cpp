#include "bgrunnable.h"

BgRunnable::BgRunnable(NetThreadPool* newPool,data::DataLoader* dataLoader)
    :pDataLoader(dataLoader),pool(newPool)
{

}
