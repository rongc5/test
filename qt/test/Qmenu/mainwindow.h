#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void show();
    ~MainWindow();
public slots:
    slotLocalTestViewContextMenu(const QPoint &position);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
