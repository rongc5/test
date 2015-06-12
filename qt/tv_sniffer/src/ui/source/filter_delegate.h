/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             filter_delegate.h
 * @brief            过滤器代理
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef  _FILTER_DELEGATE_H_
#define  _FILTER_DELEGATE_H_

#include <QItemDelegate>

class CFilterDelegate : public QItemDelegate
{
public:
    CFilterDelegate (QObject * parent = 0);

    /** 创建编辑器
     * 
     * @param  parent
     * @param  option
     * @param  index
     * @return 
     * @see 
     * @note 
     */
    virtual QWidget *	createEditor(QWidget * parent, 
        const QStyleOptionViewItem & option,
        const QModelIndex & index) const;

    /** 设置model数据
     * 
     * @param  editor
     * @param  model
     * @param  index
     * @return 
     * @see 
     * @note 
     */
    virtual void	setModelData(QWidget * editor,
        QAbstractItemModel * model, 
        const QModelIndex & index) const;

    /** 设置编辑器数据
     * 
     * @param  editor
     * @param  index
     * @return 
     * @see 
     * @note 
     */
    virtual void	setEditorData(QWidget * editor,
        const QModelIndex & index) const;
};


#endif