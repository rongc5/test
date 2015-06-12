
#ifndef  _DNS_INFO_MODEL_H_
#define  _DNS_INFO_MODEL_H_

class QStandardItemModel;
class QObject;

class CDnsInfoModel : public QStandardItemModel
{
public:
    CDnsInfoModel (QObject * parent = 0);
};

#endif