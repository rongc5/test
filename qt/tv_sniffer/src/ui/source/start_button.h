

#ifndef  _START_BUTTON_H_
#define  _START_BUTTON_H_
#include <QGraphicsWidget>
class QGraphicsWidget;
class QGraphicsItem;
class QPixmap;
class QWidget;
class QGraphicsSceneMouseEvent;
class QRectF;
class QPainterPath;

class CStartButton : public QGraphicsWidget
{
    Q_OBJECT
public:
    CStartButton (const QPixmap &pixmap, QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *);
signals:
    void pressed();
    void released ();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
private:
    QPixmap _pix;
    
};




#endif