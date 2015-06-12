
/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             packet_list_model.h
 * @brief            packetlist model
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef  _PACKET_LIST_MODEL_H_
#define  _PACKET_LIST_MODEL_H_
#include "data_struct.h"
#include <QAbstractTableModel>
#include <QStringList>
#include <QList>


class CPacketListModel : public  QAbstractTableModel
{
public:
    CPacketListModel (QObject *parent = 0);
    /** 行数
     * 
     * @param  parent 父索引
     * @return 行数
     * @see 
     * @note 
     */
    int rowCount(const QModelIndex &parent) const;  
    /** 列数
     * 
     * @param  parent 父索引
     * @return 列数
     * @see 
     * @note 
     */
    int columnCount(const QModelIndex &parent) const;  
    /** 数据
     * 
     * @param  index 索引
     * @param  role  角色
     * @return 数据
     * @see 
     * @note 
     */
    QVariant data(const QModelIndex &index, int role) const;  
    /** 标题数据
     * 
     * @param  section     位置
     * @param  orientation 方向
     * @param  role        角色
     * @return 数据
     * @see 
     * @note 
     */
    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;
    /** 添加数据包
     * 
     * @param  packet_record 数据包
     * @return void
     * @see 
     * @note 
     */
    void add_packet (const Packet_Record & packet_record);
    /** 清空
     * 
     * @return void
     * @see 
     * @note 
     */
    void clear ();
private:
    QStringList m_head_list;
    QList <Packet_Record>  m_recorder_list;
};


#endif