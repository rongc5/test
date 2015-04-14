/** 
 * Copyright (C) 2007-2013 Haier
 *
 * @file             main_window.h
 * @brief            主窗口
 * @author           zhengyl
 * @version	         1.0
 * @date	         2014-1-24
 */
#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QStackedWidget>
#include <QTableView>
#include "pie_chart_widget.h"
#include "../forms/ui_dir/ui_main_window.h"
#include "../forms/ui_dir/ui_ip_statistics_dlg.h"
#include "../forms/ui_dir/ui_pie_chart_dlg.h"

class CPacketListModel;
class IView;
class CHistogram_Widget;

class QTabWidget;
class CIpStatisticsModel;
class CPacketListView;
class CConfigFilterDlg;
class CReportWidget;
class QScrollArea;
class CStartPage;

class CMainWin : public QMainWindow
{
    Q_OBJECT
public:
    CMainWin ();
  /*  static CMainWin * instance  ();*/
    /** 获取packetlist model
     * 
     * @return packetlist model
     * @see 
     * @note 
     */
    CPacketListModel * get_packet_list_model ();
    /** 获取packetlist view
     * 
     * @return packetlist view
     * @see 
     * @note 
     */
    CPacketListView * get_packet_list_view();
    /** 获取直方图窗口
     * 
     * @return 直方图窗口
     * @see 
     * @note 
     */
    CHistogram_Widget * get_histogram_widget ();
    /** 获取报告窗口
     * 
     * @return 报告窗口
     * @see 
     * @note 
     */
    CReportWidget * get_report_widget ();
    /** 设置视图
     * 
     * @param  view 视图指针
     * @return void
     * @see 
     * @note 
     */
    void set_view (IView * view);
protected:
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
     
private:
    Ui_MainWindow  m_mw_ui;
    Ui_pie_chart_dlg m_pie_chart_ui;
    Ui_host_packages_dlg m_host_package_dlg_ui;
private:
   /* static CMainWin * m_instance;*/

    QDialog        * m_pie_chart_dlg;
    QDialog        * m_host_package_dlg;
    CConfigFilterDlg * m_config_filter_dlg;

    QSplitter      * m_splitter;
    QWidget * m_background_widget;
    CStartPage   * m_start_page;
    QStackedWidget   * m_stack_widget;
    CPacketListView *  m_packet_list_view;
    
    CPacketListModel  * m_packet_list_model;
    CIpStatisticsModel * m_ip_static_model;

    CHistogram_Widget  * m_histogram_widget;
   
    CReportWidget *    m_report_widget;

    QTabWidget      * m_main_tab_widget;
    QScrollArea     * m_report_scroll_area;
     IView          * m_view;

     bool m_start;
private slots:
    /** 开始按钮触发
     * 
     * @return void
     * @see 
     * @note 
     */
    void on_actionStartCapture_triggered ();
    /** 停止按钮触发
     * 
     * @return void
     * @see 
     * @note 
     */
    void on_actionStopCapture_triggered ();
    //void on_action_static_data_triggered ();
    //void on_actionHost_package_triggered ();
    //void on_action_exit_triggered ();
    //void on_action_export_file_triggered ();
    /** 配置按钮触发
     * 
     * @return 
     * @see 
     * @note 
     */
    void on_action_config_triggered ();
    void on_action_shutdown_triggered ();
    void on_action_about_triggered ();

};



#endif