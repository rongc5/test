
#include <QStandardItemModel>
#include "ip_statistics_model.h"

CIpStatisticsModel::CIpStatisticsModel (QObject * parent)
    : QStandardItemModel (parent)
{
    QStringList header;
    header << "Ip";
    header << "Packets";
    header << "Bytes";
    setHorizontalHeaderLabels(header);
}
