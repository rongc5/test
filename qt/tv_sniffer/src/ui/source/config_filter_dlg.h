/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             config_filter_dlg.h
 * @brief            过滤器配置
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _CONFIG_FILTER_DLG_H_
#define _CONFIG_FILTER_DLG_H_
#include <QList>
#include "../forms/ui_dir/ui_config_filter.h"
class QDialog;
class QWidget;
class CConfigFilterModel;
class CFilterDelegate;
struct FilterRecord;
class IView;

class CConfigFilterDlg : public QDialog
{
    Q_OBJECT
public:
    CConfigFilterDlg (IView * view, QWidget * parent = 0);
    /** 判断是否有空数据
     * 
     * @return bool
     * @see 
     * @note 
     */
    bool has_empty_data ();
    /** 获取过滤器数据
     * 
     * @param  recorder_list 列表中显示的数据
     * @return 
     * @see 
     * @note 
     */
    int get_filter_data (QList <FilterRecord> & recorder_list);
    /** 设置过滤器数据
     * 
     * @param  recorder_list 列表中显示的数据
     * @return 
     * @see 
     * @note 
     */
    int set_filter_data (const QList <FilterRecord> & recorder_list);

    void set_view (IView * view);

    void save_filter_data ();
protected:
private:
    Ui_config_filter_dlg m_ui_config_filter_dlg;
private:
    
private:
    CConfigFilterModel * m_filter_model;
    CFilterDelegate * m_filter_delegate;
    IView * m_view;
private slots:
    /** 添加按钮响应
     * 
     * @return 
     * @see 
     * @note 
     */
    void on_add_btn_clicked ();
    /** 删除按钮响应
     * 
     * @return 
     * @see 
     * @note 
     */
    void on_delete_btn_clicked ();

};


#endif