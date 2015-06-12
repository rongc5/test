/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             ip_statistics_model.h
 * @brief            ip统计model
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _IP_STATISTICS_H_
#define _IP_STATISTICS_H_
class QStandardItemModel;
class QObject;

class CIpStatisticsModel : public QStandardItemModel
{

public:
     CIpStatisticsModel (QObject *parent = 0);
private:

};



#endif