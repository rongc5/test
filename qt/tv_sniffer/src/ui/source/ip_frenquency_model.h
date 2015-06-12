/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             ip_frenquency_model.h
 * @brief            ip频率model
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _IP_FRENQUENCY_MODEL_H_
#define _IP_FRENQUENCY_MODEL_H_
class QStandardItemModel;
class QObject;



class CIpFrenqModel : public QStandardItemModel
{
public:
    CIpFrenqModel (QObject * parent = 0);
};

#endif