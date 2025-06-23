/****************************************************************************
** Meta object code from reading C++ file 'MyWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../MyWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MyWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN8MyWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN8MyWidgetE = QtMocHelpers::stringData(
    "MyWidget",
    "AddClicked",
    "",
    "EditClicked",
    "LoadClicked",
    "SaveClicked",
    "DelClicked",
    "UpdAfterSort",
    "Qt::SortOrder",
    "order",
    "LEUpdate",
    "text",
    "SearchClicked",
    "NextClicked",
    "PrevClicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN8MyWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   74,    2, 0x0a,    1 /* Public */,
       3,    0,   75,    2, 0x0a,    2 /* Public */,
       4,    0,   76,    2, 0x0a,    3 /* Public */,
       5,    0,   77,    2, 0x0a,    4 /* Public */,
       6,    0,   78,    2, 0x0a,    5 /* Public */,
       7,    2,   79,    2, 0x0a,    6 /* Public */,
      10,    1,   84,    2, 0x0a,    9 /* Public */,
      12,    0,   87,    2, 0x0a,   11 /* Public */,
      13,    0,   88,    2, 0x0a,   12 /* Public */,
      14,    0,   89,    2, 0x0a,   13 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 8,    2,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MyWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN8MyWidgetE.offsetsAndSizes,
    qt_meta_data_ZN8MyWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN8MyWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MyWidget, std::true_type>,
        // method 'AddClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'EditClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'LoadClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SaveClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'DelClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'UpdAfterSort'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<Qt::SortOrder, std::false_type>,
        // method 'LEUpdate'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'SearchClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'NextClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'PrevClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MyWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MyWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->AddClicked(); break;
        case 1: _t->EditClicked(); break;
        case 2: _t->LoadClicked(); break;
        case 3: _t->SaveClicked(); break;
        case 4: _t->DelClicked(); break;
        case 5: _t->UpdAfterSort((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<Qt::SortOrder>>(_a[2]))); break;
        case 6: _t->LEUpdate((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 7: _t->SearchClicked(); break;
        case 8: _t->NextClicked(); break;
        case 9: _t->PrevClicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MyWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN8MyWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MyWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
