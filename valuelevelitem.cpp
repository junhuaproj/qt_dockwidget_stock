#include "valuelevelitem.h"

#include <QProgressBar>

ValueLevelItem::ValueLevelItem()
    :count(0),children(NULL)
{

}
ValueLevelItem::ValueLevelItem(float l,int c)
    :count(0),level(l),compare(c),children(NULL)
{

}
void ValueLevelItem::getTitle(QString& title)
{
    if(compare<0)
        title="<";
    else if(compare>0)
        title=">";
    else
        title="=";
    title=title+QString("%2").arg(level);
}
void ValueLevelItem::resetData()
{
    count=0;
    indexes.clear();
    if(children==NULL)return ;
    QList<ValueLevelItem*>::iterator it= children->begin()
        ,end=children->end();
    for(;it!=end;it++)
    {
        (*it)->resetData();
    }
}

ValueLevelItem::~ValueLevelItem()
{
    clear();
}

bool ValueLevelItem::testValue(int index,float v)
{
    if(compare==0&&v>-0.01&&v<0.01)
    {
        count++;
        indexes.append(index);
        return true;
    }
    else if(compare<0&&v<level)
    {
        count++;
        indexes.append(index);

        if(children)
        {
            QList<ValueLevelItem*>::iterator it= children->begin()
                ,end=children->end();
            for(;it!=end;it++)
            {
                if((*it)->testValue(index,v))
                    break;
            }
        }
        return true;
    }
    else if(compare>0&&v>level)
    {
        count++;
        indexes.append(index);

        if(children)
        {
            QList<ValueLevelItem*>::reverse_iterator it= children->rbegin()
                ,end=children->rend();
            for(;it!=end;it++)
            {
                if((*it)->testValue(index,v))
                    break;
            }
        }
        return true;
    }
    return false;
}
void ValueLevelItem::setRange(int count,float* range,int compare)
{
    level=0;
    this->compare=compare;
    //if(compare==0)
    //{
    //level=0;
    //this->compare=0;
    //}else
    if(compare>0)
    {

        clear();
        children=new QList<ValueLevelItem*>();
        for(int i=0;i<count;i++)
        {
            children->push_back(new ValueLevelItem(range[i],compare));
        }
    }
    else if(compare<0)
    {
        clear();
        children=new QList<ValueLevelItem*>();
        for(int i=count-1;i>=0;i--)
        {
            children->push_back(new ValueLevelItem(range[i]*compare,compare));
        }
    }
}

void ValueLevelItem::clear()
{
    if(children==NULL)return ;
    while(!children->isEmpty())
    {
        delete children->back();
        children->pop_back();
    }
}
ValueItemLevelThree::ValueItemLevelThree(float center)
    :levelCenter(center)
{
    levelCount=3;
    items=new ValueLevelItem[levelCount]();
}

ValueItemLevelThree::~ValueItemLevelThree()
{
    delete []items;
}

void ValueItemLevelThree::resetGroupData()
{
    dataCount=0;
    for(int i=0;i<levelCount;i++)
        items[i].resetData();
}

void ValueItemLevelThree::setDataToTree(QTreeWidget* tree)
{
    QTreeWidgetItem* item;
    //tree->setColumnCount(2);
    QString title;
    QProgressBar* bar;
    for(int i=0;i<levelCount;i++)
    {
        item=new QTreeWidgetItem(tree);
        //item->setText(0,QString("%1  %2").arg(levels[i].title).arg(levels[i].count));
        items[i].getTitle(title);
        item->setText(0,title);
        item->setText(1,QString("%1").arg(items[i].count));
        if(items[i].children)
            addChildTreeItem(tree,item,items[i].children);
        if(dataCount)
        {
            bar=new QProgressBar(tree);
            bar->setRange(0,100);
            bar->setValue(items[i].count*100/dataCount);
            tree->setItemWidget(item,2,bar);
        }
        tree->addTopLevelItem(item);
    }
}
void ValueItemLevelThree::addChildTreeItem(QTreeWidget* tree,QTreeWidgetItem* parent,
                                            QList<ValueLevelItem*>* children )
{
    QString title;
    QTreeWidgetItem* item;
    QProgressBar* bar;
    QList<ValueLevelItem*>::iterator it=children->begin(),end=children->end();
    for(;it!=end;it++)
    {
        item=new QTreeWidgetItem(parent);
        (*it)->getTitle(title);
        item->setText(0,title);
        item->setText(1,QString("%1").arg((*it)->count));
        parent->addChild(item);
        if(dataCount)
        {
            bar=new QProgressBar(tree);
            bar->setRange(0,100);
            bar->setValue((*it)->count*100/dataCount);
            tree->setItemWidget(item,2,bar);
        }
    }
}
void ValueItemLevelThree::setRange(int count,float* range)
{
    items[0].setRange(count,range,-1);
    items[1].setRange(count,range,0);
    items[2].setRange(count,range,1);
}

void ValueItemLevelThree::group(const float* values,int count)
{
    for(int i=0;i<count;i++)
    {
        if(items[1].testValue(i,values[i]))
        {}
        else if(items[0].testValue(i,values[i]))
        {

        }
        else
            items[2].testValue(i,values[i]);
    }
    dataCount+=count;
}
void ValueItemLevelThree::group(const double* values,int count)
{
    for(int i=0;i<count;i++)
    {
        if(items[1].testValue(i,values[i]))
        {}
        else if(items[0].testValue(i,values[i]))
        {

        }
        else
            items[2].testValue(i,values[i]);
    }
    dataCount+=count;
}
void ValueItemLevelThree::setPie(QPieSeries* pie)
{
    QString title;
    for(int i=0;i<levelCount;i++)
    {
        if(items[i].children)
        {

            //uint8_t cbyte=255-colorStep*(index-zero_index);
            //slice->setColor(QColor(red,0,0));

            QList<ValueLevelItem*>::iterator it=items[i].children->begin(),
                end=items[i].children->end();
            uint8_t start=0x3f;
            int colorStep=(0xff-start)/items[i].children->size();
            uint8_t clrByte=0x3f;
            uint32_t colr;
            for(;it!=end;it++)
            {
                (*it)->getTitle(title);
                QPieSlice* slice=new QPieSlice(title,(*it)->count*100/dataCount);
                slice->setLabelVisible();

                if(items[i].getCompare()<0)
                {
                    //colr+=colorStep;
                    colr=(start+clrByte)<<8;
                }
                else
                {
                    colr=(0xff-clrByte)<<16;
                }
                slice->setColor(colr);
                slice->setLabelPosition(QPieSlice::LabelOutside);
                //slice->setColor(QColor(red,0,0));
                pie->append(slice);

                clrByte+=colorStep;
                //text=QString("%1:%2 =%3%").arg((*it)->title).arg((*it)->percent)
                //           .arg((*it)->percent*100.0/((float)stockCount),0,'f',2);
                //listWidget->addItem(text);
            }
        }
        else
        {
            QPieSlice* slice=new QPieSlice(title,items[i].count*100/dataCount);
            slice->setLabelVisible();

            //slice->setColor(QColor(red,0,0));
            slice->setColor(Qt::gray);
            pie->append(slice);
        }
    }
}
