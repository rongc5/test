
#include <QItemDelegate>
#include <QSpinBox>

#include "filter_delegate.h"
#include "ipctrl.h"

 CFilterDelegate::CFilterDelegate (QObject * parent)
     : QItemDelegate (parent)
 {

 }

 QWidget *	CFilterDelegate::createEditor(QWidget * parent, 
    const QStyleOptionViewItem & option,
    const QModelIndex & index) const
 {
     if (index.column() == 2)
     {
         QSpinBox  * child = new QSpinBox (parent);
         child->setRange(0, 65536);
         return child;
     }
     if (index.column() == 1)
     {
         return new IPCtrl (parent);
     }
     return QItemDelegate::createEditor(parent, option, index);
 }

 void	CFilterDelegate::setModelData(QWidget * editor,
    QAbstractItemModel * model, 
    const QModelIndex & index) const
 {
     if (index.column() == 2)
     {
         model->setData(index, ((QSpinBox *)editor)->value ());
     }
     if (index.column() == 1)
     {
         model->setData(index, ((IPCtrl *)editor)->get_ip ());;
     }
     return QItemDelegate::setModelData (editor, model, index);
 }

 void	CFilterDelegate::setEditorData(QWidget * editor,
    const QModelIndex & index) const
 {
     if (index.column() == 2)
     {
         ((QSpinBox *)editor)->setValue (index.data().toInt());
         return ;
     }
     if (index.column() == 1)
     {
         ((IPCtrl *)editor)->set_ip (index.data().toString());
         return;
     }
     QItemDelegate::setEditorData (editor, index);
 }