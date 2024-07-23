#include "daystockchartcfg.h"
#include "appconfig.h"

namespace chart {
enum ChartCfgVal
{
    ChartCfgVal_Mode,
    ChartCfgVal_ItemSep,
    ChartCfgVal_ItemWidth,
    ChartCfgVal_Grid,
    ChartCfgVal_ReferLine,
    ChartCfgVal_FontFace,
    ChartCfgVal_FontSize,
    ChartCfgVal_BgColor,
    ChartCfgVal_ForeColor,
    ChartCfgVal_Up,
    ChartCfgVal_Down,
    ChartCfgVal_ViewTopPadding,
    //ChartCfgVal_ViewDownPadding,
    ChartCfgVal_MarginLeft,
    ChartCfgVal_MarginTop,
    ChartCfgVal_MarginRight,
    ChartCfgVal_MarginBottom,
    ChartCfgVal_CurCrossColor,
    ChartCfgVal_CurCrossWidth,
    ChartCfgVal_Count,
};

DayStockChartCfg::DayStockChartCfg(DayChartParam* cfg,ChartCfg* chartCfg,QObject *parent)
    : QObject(parent),pCfg(cfg),pChartCfg(chartCfg)
{}

const char* DayStockChartCfg::cfgName()const
{
    return "chart cfg";
}
const char* DayStockChartCfg::getIndexId()const
{
    return "chartcfg";
}
bool DayStockChartCfg::loadCfg(QJsonObject* obj)
{
    pCfg->mode=obj->value("mode").toInt();
    pChartCfg->itemSep=obj->value("itemsep").toDouble();
    pChartCfg->itemWidth=obj->value("itemwidth").toDouble();
    pCfg->grid=obj->value("grid").toInt();
    pChartCfg->bg=obj->value("bg").toInt();
    pChartCfg->fore=obj->value("fore").toInt();
    pCfg->referline=obj->value("referline").toInt();

    pChartCfg->fontSize=obj->value("fontSize").toInt();
    strcpy(pChartCfg->fontFace,obj->value("fontFace").toString().toStdString().c_str());
    pCfg->down=obj->value("decrease").toInt();
    pCfg->up=obj->value("increase").toInt();
    pCfg->viewTopPadding=obj->value("viewTopPadding").toInt();
    pChartCfg->marginLeft=obj->value("marginLeft").toInt();
    pChartCfg->marginTop=obj->value("marginTop").toInt();
    pChartCfg->marginRight=obj->value("marginRight").toInt();
    pChartCfg->marginBottom=obj->value("marginBottom").toInt();
    pCfg->curCrossColor=obj->value("curCrossColor").toInt();
    pCfg->curCrossWidth= JsonValueToFloat(obj->value("curCrossWidth"));
    return true;
}

bool DayStockChartCfg::saveCfg(QJsonObject* obj)
{
    obj->insert("mode",QJsonValue(pCfg->mode));
    obj->insert("itemsep",QJsonValue(pChartCfg->itemSep));
    obj->insert("itemwidth",QJsonValue(pChartCfg->itemWidth));
    obj->insert("grid",QJsonValue((int)pCfg->grid));
    obj->insert("bg",QJsonValue((int)pChartCfg->bg));
    obj->insert("fore",QJsonValue((int)pChartCfg->fore));
    obj->insert("referline",QJsonValue((int)pCfg->referline));
    obj->insert("fontSize",QJsonValue(pChartCfg->fontSize));
    obj->insert("fontFace",QJsonValue(pChartCfg->fontFace));
    obj->insert("increase",QJsonValue((int)pCfg->up));

    obj->insert("decrease",QJsonValue((int)pCfg->down));
    obj->insert("viewTopPadding",QJsonValue(pCfg->viewTopPadding));
    obj->insert("marginLeft",QJsonValue((int)pChartCfg->marginLeft));
    obj->insert("marginTop",QJsonValue((int)pChartCfg->marginTop));
    obj->insert("marginRight",QJsonValue((int)pChartCfg->marginRight));
    obj->insert("marginBottom",QJsonValue((int)pChartCfg->marginBottom));
    InsertColorToJson(obj,"curCrossColor",pCfg->curCrossColor);
    InsertFloatToJson(obj,"curCrossWidth",pCfg->curCrossWidth);
    return true;
}
bool DayStockChartCfg::loadDefault()
{
    pCfg->mode=0;
    pChartCfg->itemSep=2;
    pChartCfg->itemWidth=7;
    pCfg->grid=0x2f2f2f;
    pChartCfg->bg=0x0;
    pChartCfg->fore=0xffffff;
    pCfg->referline=0xffff00;
    pChartCfg->fontSize=10;
    QString chartFont;
    strcpy(pChartCfg->fontFace,chartFont.toStdString().c_str());// "宋体");
    pCfg->up=0xff0000;
    pCfg->down=0x00ff00;
    pCfg->viewTopPadding=12;
    //param->viewDateHeight=12;
    pChartCfg->marginLeft=5;
    pChartCfg->marginTop=20;
    pChartCfg->marginRight=40;
    pChartCfg->marginBottom=5;
    pCfg->curCrossColor=0xffffff;
    pCfg->curCrossWidth=0;
    return true;
}

int DayStockChartCfg::getCfgCount()
{
    return ChartCfgVal_Count;
}

bool DayStockChartCfg::getCfgValue(int index,IndexCfgValue* cfgValue)const
{
    switch(index)
    {
    case ChartCfgVal_Mode:
        cfgValue->setInt(tr("mode"),pCfg->mode);
        break;
    case ChartCfgVal_ItemSep:
        cfgValue->setFloat(tr("item sep"),pChartCfg->itemSep);
        break;
    case ChartCfgVal_ItemWidth:
        cfgValue->setFloat(tr("item width"),pChartCfg->itemWidth);
        break;
    case ChartCfgVal_Grid:
        cfgValue->setColor(tr("grid color"),pCfg->grid);
        break;
    case ChartCfgVal_ReferLine:
        cfgValue->setColor(tr("reference line"),pCfg->referline);
        break;
    case ChartCfgVal_FontFace:
        cfgValue->setStr(tr("font face"),pChartCfg->fontFace);
        break;
    case ChartCfgVal_FontSize:
        cfgValue->setInt(tr("font size"),pChartCfg->fontSize);
        break;
    case ChartCfgVal_BgColor:
        cfgValue->setColor(tr("background color"),pChartCfg->bg);
        break;
    case ChartCfgVal_ForeColor:
        cfgValue->setColor(tr("fore color"),pChartCfg->fore);
        break;
    case ChartCfgVal_Up:
        cfgValue->setColor(tr("increase color"),pCfg->up);
        break;
    case ChartCfgVal_Down:
        cfgValue->setColor(tr("decrease color"),pCfg->down);
        break;
    case ChartCfgVal_ViewTopPadding:
        cfgValue->setInt(tr("view top paddiing"),pCfg->viewTopPadding,"chart top padding");
        break;
    //case ChartCfgVal_ViewDownPadding:
    //    cfgValue->setInt(tr("decrease color"),pCfg->down);
        break;
    case ChartCfgVal_MarginLeft:
        cfgValue->setInt(tr("left margin"),pChartCfg->marginLeft);
        break;
    case ChartCfgVal_MarginTop:
        cfgValue->setInt(tr("top margin"),pChartCfg->marginTop);
        break;
    case ChartCfgVal_MarginRight:
        cfgValue->setInt(tr("right margin"),pChartCfg->marginRight);
        break;
    case ChartCfgVal_MarginBottom:
        cfgValue->setInt(tr("bottom margin"),pChartCfg->marginBottom);
        break;
    case ChartCfgVal_CurCrossColor:
        cfgValue->setColor(tr("cur cross color"),pCfg->curCrossColor);
        break;
    case ChartCfgVal_CurCrossWidth:
        cfgValue->setFloat(tr("cur cross width"),pCfg->curCrossWidth);
        break;
    default:
        return false;
    }
    return true;
}
bool DayStockChartCfg::setCfgValue(int index,const IndexCfgValue* cfgValue)
{
    switch(index)
    {
    case ChartCfgVal_Mode:
        pCfg->mode=cfgValue->vInt;
        break;
    case ChartCfgVal_ItemSep:
        pChartCfg->itemSep=cfgValue->vFlt;
        break;
    case ChartCfgVal_ItemWidth:
        pChartCfg->itemWidth=cfgValue->vFlt;
        break;
    case ChartCfgVal_Grid:
        pCfg->grid=cfgValue->vClr;
        break;
    case ChartCfgVal_ReferLine:
        pCfg->referline=cfgValue->vClr;
        break;
    case ChartCfgVal_FontFace:
        strcpy(pChartCfg->fontFace,cfgValue->vStr.toStdString().c_str());
        //cfgValue->setStr(tr("font face"),pCfg->fontFace);
        break;
    case ChartCfgVal_FontSize:
        //cfgValue->setInt(tr("font size"),pCfg->fontSize);
        pCfg->referline=cfgValue->vInt;
        break;
    case ChartCfgVal_BgColor:
        pChartCfg->bg=cfgValue->vClr;
        break;
    case ChartCfgVal_ForeColor:
        pChartCfg->fore=cfgValue->vClr;
        break;
    case ChartCfgVal_Up:
        pCfg->up=cfgValue->vClr;
        break;
    case ChartCfgVal_Down:
        pCfg->down=cfgValue->vClr;
        break;
    case ChartCfgVal_ViewTopPadding:
        pCfg->viewTopPadding=cfgValue->vInt;
        break;
    case ChartCfgVal_MarginLeft:
        pChartCfg->marginLeft=cfgValue->vInt;
        break;
    case ChartCfgVal_MarginTop:
        pChartCfg->marginTop=cfgValue->vInt;
        break;
    case ChartCfgVal_MarginRight:
        pChartCfg->marginRight=cfgValue->vInt;
        break;
    case ChartCfgVal_MarginBottom:
        pChartCfg->marginBottom=cfgValue->vInt;
        break;
    case ChartCfgVal_CurCrossColor:
        pCfg->curCrossColor=cfgValue->vClr;
        break;
    case ChartCfgVal_CurCrossWidth:
        pCfg->curCrossWidth=cfgValue->vFlt;
        break;
    default:
        return false;
    }
    return true;
}
}
