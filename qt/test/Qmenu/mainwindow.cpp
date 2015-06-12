#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPoint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}


void MainWindow::slotLocalTestViewContextMenu(const QPoint &position)
{
    QModelIndex index=ui->local_test_view->indexAt(position);
    if(!index.isValid())
    {
        qDebug()<<"slotLocalTestViewContextMenu: index is not valid";
        return;
    }
    QList<QAction *> actions;

    QAction *action_create_data_set=new QAction(this);
    action_create_data_set->setObjectName(QString::fromLocal8Bit("action_create_data_set"));
    action_create_data_set->setText(tr("Create data set..."));
    action_create_data_set->setEnabled(true);
    actions.append(action_create_data_set);

    QAction *action_show_test_detail=new QAction(this);
    action_show_test_detail->setObjectName(QString::fromLocal8Bit("action_show_test_detail"));
    action_show_test_detail->setText("Show test detail...");
    action_show_test_detail->setEnabled(false);
    actions.append(action_show_test_detail);

    QAction *result_action = QMenu::exec(actions,ui->local_test_view->mapToGlobal(position));
    if(result_action == action_create_data_set)
    {
        QStringList selected_test_names;
        selected_test_names.append(index.data(QFileSystemModel::FilePathRole).toString());
        CreateDataSetDialog dialog(core_app_context,this);
        dialog.exec();
    }
    if(result_action==action_show_test_detail)
    {
        qDebug()<<"Hasn't implemented!";
    }
    foreach(QAction* action, actions)
    {
        action->deleteLater();
    }

    return;
}

void MainWindow::show()
{
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(slotLocalTestViewContextMenu(QPoint)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
