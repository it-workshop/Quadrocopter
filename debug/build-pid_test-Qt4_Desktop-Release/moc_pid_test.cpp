/****************************************************************************
** Meta object code from reading C++ file 'pid_test.h'
**
** Created: Sat Aug 3 16:07:38 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pid_test/pid_test.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pid_test.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PID_test[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x08,
      30,    9,    9,    9, 0x08,
      60,    9,    9,    9, 0x08,
      95,   90,    9,    9, 0x08,
     122,   90,    9,    9, 0x08,
     149,   90,    9,    9, 0x08,
     176,   90,    9,    9, 0x08,
     206,  200,    9,    9, 0x08,
     248,    9,    9,    9, 0x08,
     277,   90,    9,    9, 0x08,
     324,   90,    9,    9, 0x08,
     353,   90,    9,    9, 0x08,
     382,   90,    9,    9, 0x08,
     411,   90,    9,    9, 0x08,
     445,   90,    9,    9, 0x08,
     478,  472,    9,    9, 0x08,
     501,  472,    9,    9, 0x08,
     530,  472,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_PID_test[] = {
    "PID_test\0\0timer_auto_update()\0"
    "on_pushButton_reset_clicked()\0"
    "on_pushButton_pause_clicked()\0arg1\0"
    "on_Kp_valueChanged(double)\0"
    "on_Ki_valueChanged(double)\0"
    "on_Kd_valueChanged(double)\0"
    "on_dt_valueChanged(int)\0index\0"
    "on_comboBox_type_currentIndexChanged(int)\0"
    "on_pushButton_wind_clicked()\0"
    "on_doubleSpinBox_friction_valueChanged(double)\0"
    "on_Kp_2_valueChanged(double)\0"
    "on_Ki_2_valueChanged(double)\0"
    "on_Kd_2_valueChanged(double)\0"
    "on_spinBox_wind_valueChanged(int)\0"
    "on_scale_valueChanged(int)\0value\0"
    "on_x_valueChanged(int)\0"
    "on_payload_valueChanged(int)\0"
    "on_mincorr_valueChanged(int)\0"
};

void PID_test::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PID_test *_t = static_cast<PID_test *>(_o);
        switch (_id) {
        case 0: _t->timer_auto_update(); break;
        case 1: _t->on_pushButton_reset_clicked(); break;
        case 2: _t->on_pushButton_pause_clicked(); break;
        case 3: _t->on_Kp_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->on_Ki_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->on_Kd_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->on_dt_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_comboBox_type_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_pushButton_wind_clicked(); break;
        case 9: _t->on_doubleSpinBox_friction_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->on_Kp_2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 11: _t->on_Ki_2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 12: _t->on_Kd_2_valueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 13: _t->on_spinBox_wind_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_scale_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_x_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_payload_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_mincorr_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PID_test::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PID_test::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_PID_test,
      qt_meta_data_PID_test, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PID_test::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PID_test::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PID_test::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PID_test))
        return static_cast<void*>(const_cast< PID_test*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int PID_test::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
