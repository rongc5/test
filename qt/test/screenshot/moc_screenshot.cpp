/****************************************************************************
** Meta object code from reading C++ file 'screenshot.h'
**
** Created: Tue Feb 11 17:01:30 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "screenshot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'screenshot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Screenshot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      28,   11,   11,   11, 0x08,
      45,   11,   11,   11, 0x08,
      59,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Screenshot[] = {
    "Screenshot\0\0newScreenshot()\0"
    "saveScreenshot()\0shootScreen()\0"
    "updateCheckBox()\0"
};

void Screenshot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Screenshot *_t = static_cast<Screenshot *>(_o);
        switch (_id) {
        case 0: _t->newScreenshot(); break;
        case 1: _t->saveScreenshot(); break;
        case 2: _t->shootScreen(); break;
        case 3: _t->updateCheckBox(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Screenshot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Screenshot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Screenshot,
      qt_meta_data_Screenshot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Screenshot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Screenshot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Screenshot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Screenshot))
        return static_cast<void*>(const_cast< Screenshot*>(this));
    return QWidget::qt_metacast(_clname);
}

int Screenshot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
