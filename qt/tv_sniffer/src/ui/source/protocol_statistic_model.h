/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             protocol_statistic_model.h
 * @brief            协议统计model
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _PROTOCOL_STATISTIC_MODEL_H_
#define _PROTOCOL_STATISTIC_MODEL_H_
class QStandardItemModel;
class QObject ;
class CProtocolStatisticModel : public QStandardItemModel
{
public:
    CProtocolStatisticModel (QObject * parent = 0);
};



#endif