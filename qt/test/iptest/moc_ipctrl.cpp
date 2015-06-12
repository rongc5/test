/****************************************************************************
** Meta object code from reading C++ file 'ipctrl.h'
**
** Created: Tue Jan 7 14:27:17 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ipctrl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ipctrl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IPCtrl[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      44,    8,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_IPCtrl[] = {
    "IPCtrl\0\0pEdit\0signalTextChanged(QLineEdit*)\0"
    "slotTextChanged(QLineEdit*)\0"
};

void IPCtrl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IPCtrl *_t = static_cast<IPCtrl *>(_o);
        switch (_id) {
        case 0: _t->signalTextChanged((*reinterpret_cast< QLineEdit*(*)>(_a[1]))); break;
        case 1: _t->slotTextChanged((*reinterpret_cast< QLineEdit*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData IPCtrl::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IPCtrl::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_IPCtrl,
      qt_meta_data_IPCtrl, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IPCtrl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IPCtrl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IPCtrl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IPCtrl))
        return static_cast<void*>(const_cast< IPCtrl*>(this));
    return QFrame::qt_metacast(_clname);
}

int IPCtrl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IPCtrl::signalTextChanged(QLineEdit * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
