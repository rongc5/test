/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             config_filter_model.h
 * @brief            过滤器model
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef  _CONFIG_FILTER_MIDEL_H_
#define  _CONFIG_FILTER_MIDEL_H_

#include "data_struct.h"
#include <QAbstractTableModel>
#include <QStringList>
#include <QList>

class CConfigFilterModel : public  QAbstractTableModel
{
    Q_OBJECT

public:
    
    CConfigFilterModel (QObject *parent = 0);
    /** 获取行数
     * 
     * @param  parent
     * @return 函数
     * @see 
     * @note 
     */
    int rowCount(const QModelIndex &parent) const;  
    /** 获取列数
     * 
     * @param  parent
     * @return 列数
     * @see 
     * @note 
     */
    int columnCount(const QModelIndex &parent) const;  
    /** 获取数据
     * 
     * @param  index
     * @param  role 角色
     * @return 数据
     * @see 
     * @note 
     */
    QVariant data(const QModelIndex &index, int role) const;  
    /** 设置数据
     * 
     * @param  index 索引
     * @param  value 值
     * @param  role  角色
     * @return  bool
     * @see 
     * @note 
     */
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    /** 获取表格头
     * 
     * @param  section
     * @param  orientation 方向
     * @param  role        角色
     * @return 
     * @see 
     * @note 
     */
    QVariant headerData(int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;
    /** 标识
     * 
     * @param  index 索引
     * @return 
     * @see 
     * @note 
     */
    virtual Qt::ItemFlags flags(const QModelIndex & index) const;
    /** 添加记录
     * 
     * @param  record 记录
     * @return 
     * @see 
     * @note 
     */
    void add_record (const FilterRecord & record);
    /** 删除记录
     * 
     * @param  row
     * @return 
     * @see 
     * @note 
     */
    void delete_record (int row);
    /** 清空
     * 
     * @return 
     * @see 
     * @note 
     */
    void clear ();
    /** 是否有空数据
     * 
     * @return 
     * @see 
     * @note 
     */
    bool has_empty_data ();
    /** 获取过滤器的数据
     * 
     * @param  recorder_list
     * @return 
     * @see 
     * @note 
     */
    int get_filter_data (QList <FilterRecord> & recorder_list);
    /** 设置过滤器的数据
     * 
     * @param  recorder_list
     * @return 
     * @see 
     * @note 
     */
    int set_filter_data (const QList <FilterRecord> & recorder_list);
private:
    QStringList m_head_list;
    QList <FilterRecord>  m_recorder_list;

private slots:

    /** 数据发生变化的响应
     * 
     * @param  topLeft
     * @param  bottomRight
     * @param  roles
     * @return 
     * @see 
     * @note 
     */
    void slot_data_changed(const QModelIndex & topLeft, 
        const QModelIndex & bottomRight, 
        const QVector<int> & roles);
};

#endif