

#ifndef  _START_PAGE_H_
#define  _START_PAGE_H_
#include <QObject>
class CStartButton;
class QGraphicsView;
class QGraphicsScene;
class QWidget;
class QPaintEvent;
#include "../forms/ui_dir/ui_start_page.h"
class CStartPage : public QWidget
{
    Q_OBJECT
public:
    CStartPage (QWidget * parent = 0);
protected:
    virtual  void paintEvent(QPaintEvent * event);
signals:
    void start ();
private:
    Ui_start_page m_ui;
    CStartButton * m_start_button;
    QGraphicsView * m_gw;
    QGraphicsScene * m_scene;

};

#endif