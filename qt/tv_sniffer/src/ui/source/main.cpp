



#include <QApplication>
#include <QTextCodec>
#include <qglobal.h>
#include <QFile>
#include <iostream>


#include "main_window.h"
#include "login.h"
#include "ictrl.h"
#include "imodel.h"
#include "iview.h"



int main (int argc, char * argv [])
{
    
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(resource);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    /*QString qss; 
    QFile qssFile("../src/ui/resource/css.qss");  
    qssFile.open(QFile::ReadOnly);  
    if(qssFile.isOpen())  

    {  

    qss = QLatin1String(qssFile.readAll());  

    std::cout << qss.toStdString().c_str () << "\n";
    qApp->setStyleSheet(qss);  

    qssFile.close();  

    }  */
    //qApp->setStyleSheet(qss);
    //std::string q = qss.toStdString();
    IView * view = get_view ();
    IModel * model = get_model();
    ICtrl * ctrl = get_ctrl_ptr();
    view->init(ctrl);
    model->init(ctrl);
    ctrl->init(model, view);

    CMainWin mai_win;
    mai_win.set_view (view);
    view->set_widget(&mai_win);
    mai_win.showMaximized();

    CLoginDlg login_dlg(&mai_win);
    //login_dlg.setWindowFlags (Qt::FramelessWindowHint);
    login_dlg.run ();
    return a.exec();
}