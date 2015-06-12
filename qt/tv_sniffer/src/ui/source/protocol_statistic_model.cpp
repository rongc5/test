
#include <QStandardItemModel>
#include "protocol_statistic_model.h"


CProtocolStatisticModel::CProtocolStatisticModel (QObject * parent)
    : QStandardItemModel (parent)
{
    QStringList head_list;
    head_list <<  QString::fromLocal8Bit("协议列表");
    head_list << QString::fromLocal8Bit("次数");
    setHorizontalHeaderLabels(head_list);
};