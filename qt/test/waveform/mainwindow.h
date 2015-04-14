#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QPixmap>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void demoPlay();
private slots:
    void shootScreen();

private:
    Ui::MainWindow *ui;
    QCustomPlot *customPlot;
    QTimer *timer;
};

#endif // MAINWINDOW_H
