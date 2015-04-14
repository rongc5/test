#include <QStandardItemModel>

#include "dns_info_model.h"

CDnsInfoModel::CDnsInfoModel (QObject * parent)
    : QStandardItemModel (parent)
{
    QStringList head_list;
    head_list <<  QString::fromLocal8Bit("域名");
    head_list << QString::fromLocal8Bit("ip");
    setHorizontalHeaderLabels(head_list);
}