/****************************************************************************
** Meta object code from reading C++ file 'AGClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../TextClient/AGClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AGClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AGClient_t {
    QByteArrayData data[11];
    char stringdata[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_AGClient_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_AGClient_t qt_meta_stringdata_AGClient = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 11),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 10),
QT_MOC_LITERAL(4, 33, 2),
QT_MOC_LITERAL(5, 36, 12),
QT_MOC_LITERAL(6, 49, 16),
QT_MOC_LITERAL(7, 66, 7),
QT_MOC_LITERAL(8, 74, 11),
QT_MOC_LITERAL(9, 86, 5),
QT_MOC_LITERAL(10, 92, 14)
    },
    "AGClient\0socketError\0\0idReceived\0id\0"
    "readFinished\0std::vector<int>\0message\0"
    "readMessage\0getId\0onDisconnected\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AGClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x05,
       3,    1,   45,    2, 0x05,
       5,    1,   48,    2, 0x05,

 // slots: name, argc, parameters, tag, flags
       8,    0,   51,    2, 0x08,
       9,    0,   52,    2, 0x08,
      10,    0,   53,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AGClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AGClient *_t = static_cast<AGClient *>(_o);
        switch (_id) {
        case 0: _t->socketError(); break;
        case 1: _t->idReceived((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->readFinished((*reinterpret_cast< std::vector<int>(*)>(_a[1]))); break;
        case 3: _t->readMessage(); break;
        case 4: _t->getId(); break;
        case 5: _t->onDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AGClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGClient::socketError)) {
                *result = 0;
            }
        }
        {
            typedef void (AGClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGClient::idReceived)) {
                *result = 1;
            }
        }
        {
            typedef void (AGClient::*_t)(std::vector<int> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AGClient::readFinished)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject AGClient::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_AGClient.data,
      qt_meta_data_AGClient,  qt_static_metacall, 0, 0}
};


const QMetaObject *AGClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AGClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AGClient.stringdata))
        return static_cast<void*>(const_cast< AGClient*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int AGClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void AGClient::socketError()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void AGClient::idReceived(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AGClient::readFinished(std::vector<int> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
