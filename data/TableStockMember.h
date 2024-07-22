#ifndef TABLESTOCKMEMBER_H
#define TABLESTOCKMEMBER_H

/**
 * 股票列表数据属性
 * */
enum TableStockMember
{
    TableStockMember_Code,//编号
    TableStockMember_Name,//名称
    TableStockMember_Level,//
    TableStockMember_Comment,
    TableStockMember_Bussiness,
    TableStockMember_Indexex,//指数
    TableStockMember_Notions,
    TableStockMember_Price,
    TableStockMember_Chg,//涨跌额
    TableStockMember_PChg,//涨跌百分比
    TableStockMember_Volume,//成交量
    TableStockMember_TradeRate,//换手
    TableStockMember_VolumeRate,//量比
    TableStockMember_High,//最高价
    TableStockMember_Low,//最低价
    TableStockMember_PreClose,//昨收
    TableStockMember_Open,//今开
    TableStockMember_DownDay,
    TableStockMember_LastKeyVal,
    TableStockMember_DownPercent,
    TableStockMember_Count,
};

#endif // TABLESTOCKMEMBER_H
