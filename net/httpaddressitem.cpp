#include "httpaddressitem.h"

HttpAddressItem::HttpAddressItem():id(0) {}

HttpAddressItem::HttpAddressItem(int nid,const QString& nname,const QString& naddress)
{
    id=nid;
    name=nname;
    address=naddress;
}
