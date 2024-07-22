/**
 * mysql 数据预处理绑定
*/
#ifndef MYBIND_VALUE_H
#define MYBIND_VALUE_H

#include <mysql.h>
struct BindValue
{
    bool isnull;
    uint32_t buf_len;
    union{
        char* szBuf;
        MYSQL_TIME* pTime;
        uint64_t u64Value;
        int32_t i32Value;
        float fValue;
        double dValue;
    };
    unsigned long length;
};

#endif // MYBIND_VALUE_H
