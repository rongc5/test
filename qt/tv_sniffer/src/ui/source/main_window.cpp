#include <QTabWidget>
#include <QFileDialog>
#include <QScrollArea>
#include <QPixmap>
#include <QMessageBox>
#include <QTableView>
#include <iostream>
#include <QKeyEvent>
#include <QProcess>
#include <QStandardItemModel>
#include <QProgressDialog>


#include "packet_list_view.h"
#include "histogram_widget.h"
#include "iview.h"
#include "packet_list_model.h"
#include "ip_statistics_model.h"
#include "config_filter_dlg.h"
#include "report_widget.h"
#include "start_page.h"
#include "main_window.h"
#include <stdio.h>
#include <errno.h>



//CMainWin * CMainWin::m_instance = NULL;
void CMainWin::set_view (IView * view)
{
    m_view = view;
    m_config_filter_dlg->set_view(m_view);
    QList <FilterRecord> list_record;
    m_config_filter_dlg->get_filter_data(list_record);
    std::list <IpFilterInfo> ip_filter_info_list;;
    QList <FilterRecord>::const_iterator cit
        = list_record.begin();
    for ( ; cit != list_record.end  (); ++cit)
    {
        IpFilterInfo ip_info;
        ip_info.name = cit->name;
        ip_info.ip = cit->ip;
        ip_info.port = cit->port;
        ip_filter_info_list.push_back(ip_info);
    }
    m_view->set_filter(ip_filter_info_list);
}
CMainWin::CMainWin () : 
m_view (NULL),
    m_start (false),
    m_config_filter_dlg (NULL)
{
    std::cout << "CMainWin\n";
    m_mw_ui.setupUi(this);
    
    m_mw_ui.action_static_data->setEnabled(false);
    m_mw_ui.actionHost_package->setEnabled(false);
    m_mw_ui.action_export_file->setEnabled(false);
     std::cout << "CStartPage\n";
    m_start_page = new CStartPage (this);
    std::cout << "setAutoFillBackground\n";
    m_start_page->setAutoFillBackground(true);
    connect(m_start_page, SIGNAL(start ()), this, SLOT(on_actionStartCapture_triggered()));


    
    m_pie_chart_dlg = new QDialog (this);
    m_pie_chart_ui.setupUi (m_pie_chart_dlg);

    m_host_package_dlg = new QDialog (this);
    m_host_package_dlg_ui.setupUi(m_host_package_dlg);
    m_ip_static_model = new CIpStatisticsModel ();
    m_host_package_dlg_ui.host_packages->setModel(m_ip_static_model);

    m_stack_widget = new QStackedWidget (this);
    m_splitter = new QSplitter (Qt::Vertical, this);
    m_packet_list_view = new CPacketListView (m_splitter);
    m_packet_list_view->setAlternatingRowColors(true);
    m_packet_list_view->setAutoScroll(true);
    m_packet_list_model = new CPacketListModel (m_splitter);

    m_config_filter_dlg = new CConfigFilterDlg (NULL, this);
    
    m_histogram_widget = new CHistogram_Widget ();

    m_packet_list_view->setModel(m_packet_list_model);
    m_splitter->addWidget(m_histogram_widget);
    m_splitter->addWidget(m_packet_list_view);
    m_splitter->setStretchFactor(0, 1);
    m_splitter->setStretchFactor(1, 5);

    m_main_tab_widget = new QTabWidget (this);
    m_report_widget = new CReportWidget;
    m_report_scroll_area = new QScrollArea (this);
    m_report_scroll_area->setWidget(m_report_widget);
    m_report_scroll_area->setWidgetResizable (true);

   m_main_tab_widget->addTab(m_splitter, QString::fromLocal8Bit("抓包显示"));
   //m_main_tab_widget->addTab(m_report_scroll_area, QString::fromLocal8Bit("报告"));
   m_report_scroll_area->hide();

    m_stack_widget->addWidget(m_start_page);
    m_stack_widget->addWidget(m_main_tab_widget);



    setCentralWidget (m_stack_widget);
    m_stack_widget->setCurrentWidget(m_start_page);
    setWindowFlags (Qt::FramelessWindowHint);
    setWindowModality (Qt::WindowModal);
    
}
void CMainWin::on_actionStartCapture_triggered ()
{
    std::cout << "on_actionStartCapture_triggered\n";
    m_mw_ui.actionStartCapture->setChecked(true);
    m_mw_ui.actionStopCapture->setChecked(false);
    if (m_start)
    {

        return;
    }
   
    m_start = true;
    m_stack_widget->setCurrentWidget(m_main_tab_widget);
    m_main_tab_widget->removeTab(0);
    m_main_tab_widget->addTab(m_splitter, QString::fromLocal8Bit("抓包显示"));
    m_report_scroll_area->hide ();
    std::cout << "setCurrentWidget\n";
    m_main_tab_widget->setCurrentWidget(m_splitter);

   m_mw_ui.action_config->setEnabled(false);
   m_view->clear_packet_list();
   QProgressDialog progress(QString::fromLocal8Bit("系统初始化中..."), "cancel", 0, 100, this);
   progress.setWindowModality(Qt::WindowModal);
   progress.setValue(30);
   std::cout << "start_capture begin\n";
   m_view->start_capture();
    std::cout << "start_capture end\n";
   progress.setValue(100);


}
void CMainWin::on_actionStopCapture_triggered ()
{
    QProgressDialog progress(this, Qt::Dialog);
    progress.setLabelText(QString::fromLocal8Bit("正在生成报告......"));
    progress.setCancelButton(NULL);
    progress.setMaximum(100);
    progress.setWindowModality(Qt::WindowModal);
    progress.setValue(30);
    m_mw_ui.actionStartCapture->setChecked(false);
    m_mw_ui.actionStopCapture->setChecked(true);
    if (!m_start)
    {
        return;
    }
    m_start = false;
    m_main_tab_widget->removeTab(0);
    m_main_tab_widget->addTab(m_report_scroll_area, QString::fromLocal8Bit("报告"));
    m_splitter->hide();

    m_report_scroll_area->show();
   
    m_mw_ui.action_config->setEnabled(true);
    
    
    progress.setValue(50);
    m_view->stop_capture();
    progress.setValue(100);
    //QPixmap pixmap(m_report_scroll_area->viewport()->width(), m_report_scroll_area->viewport()->height());
    //QPainter painter(&pixmap);
    //QRect rect = painter.viewport();
    //QSize size = pixmap.size();
    //size.scale(rect.size(), Qt::KeepAspectRatio);
    //painter.setViewport(rect.x(), rect.y(),
    //    size.width(), size.height());
    //painter.setWindow(pixmap.rect());
    //painter.drawImage(0, 0, pixmap);
    
    //m_report_scroll_area->render(&pixmap);
    //pixmap.save("./image", "JPG");
}
//CMainWin * CMainWin::instance  ()
//{
//    if (m_instance != NULL)
//    {
//        return m_instance;
//    }
//    return m_instance = new CMainWin ();
//}
CPacketListModel * CMainWin::get_packet_list_model ()
{
    return m_packet_list_model;
}
 CPacketListView * CMainWin::get_packet_list_view()
 {
     return m_packet_list_view;
 }
CHistogram_Widget * CMainWin::get_histogram_widget ()
{
    return m_histogram_widget;
}
CReportWidget * CMainWin::get_report_widget ()
{
    return m_report_widget;
}
void CMainWin::keyPressEvent(QKeyEvent * event)
{
    hide ();
    show ();
    switch(event->key())
    {
    case Qt::Key_Alt:
        {
            //QMessageBox::information(this, "", "alt");
        }
        
        break;
    }
    return;
}
void CMainWin::keyReleaseEvent(QKeyEvent * event)
{
    return;
}
//void CMainWin::on_action_static_data_triggered ()
//{
//    m_pie_chart_dlg->exec();
//}
//
//void CMainWin::on_actionHost_package_triggered ()
//{
//    m_host_package_dlg->exec();
//}
//void CMainWin::on_action_exit_triggered ()
//{
//    exit (0);
//}
//void CMainWin::on_action_export_file_triggered ()
//{
//    QString fileName(tr("ok.txt")) ;   
//    QString filter;
//    QString dir = QFileDialog::getSaveFileName(this, tr("Save File"), "", "txt \n pdf", &filter);
//    std::string path = dir.toStdString();
//    std::string sfilter = filter.toStdString();
//    int i;
//    //QDir d;
//    //d.mkpath(dir);//可以不用，因为路径已经有了，就不用mk了
//    //QFile file(dir+"/"+fileName);
//    //file.open(QFile::WriteOnly);
//}
void CMainWin::on_action_config_triggered ()
{
    QList <FilterRecord> list_record;
    m_config_filter_dlg->get_filter_data(list_record);
 
    while (1)
    {
        if (m_config_filter_dlg->exec () == QDialog::Accepted)
        {
            if (m_config_filter_dlg->has_empty_data())
            {
                QMessageBox::warning (this, 
                    QString::fromLocal8Bit("提示"), 
                    QString::fromLocal8Bit("名字为空或者ip和端口号同时为空，请重新输入"));

            }
            else
            {
                std::cout << "m_config_filter_dlg->save_filter_data()\n";
                m_config_filter_dlg->save_filter_data();
                break;
            }
        }
        else
        {
            m_config_filter_dlg->set_filter_data(list_record);
            break;
        }
        
       
    }

    list_record.clear();
    m_config_filter_dlg->get_filter_data(list_record);
    
    std::list <IpFilterInfo> ip_filter_info_list;;
    QList <FilterRecord>::const_iterator cit
        = list_record.begin();
    for ( ; cit != list_record.end  (); ++cit)
    {
        IpFilterInfo ip_info;
        ip_info.name = cit->name;
        ip_info.ip = cit->ip;
        ip_info.port = cit->port;
        ip_filter_info_list.push_back(ip_info);
    }
    m_view->set_filter(ip_filter_info_list);
}
void CMainWin::on_action_shutdown_triggered ()
{
    std::cout << "on_action_exit_triggered\n";
    QStringList options;
    options << "-c" << "halt";
    QProcess* process = new QProcess(this);
    process->start("/bin/sh", options);
    /*FILE * fp;
    int fd;
    fp = popen("halt", "r");
    if (!fp)
    {
    perror("popen:");
    std::cout << "failed \n";
    }

    pclose(fp);*/
}
void CMainWin::on_action_about_triggered ()
{
    QMessageBox::about(this, "about", "tv-sniffer 0.1 alpha version");
}