/********************************************************************************
** Form generated from reading UI file 'pid_test.ui'
**
** Created: Sat Aug 3 16:42:55 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PID_TEST_H
#define UI_PID_TEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QProgressBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qwt_plot.h"

QT_BEGIN_NAMESPACE

class Ui_PID_test
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QwtPlot *plot;
    QwtPlot *plot_val;
    QProgressBar *value;
    QProgressBar *diff;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_Kp;
    QDoubleSpinBox *Kp;
    QLabel *label_Ki;
    QDoubleSpinBox *Ki;
    QLabel *label_Kd;
    QDoubleSpinBox *Kd;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_Kp_2;
    QDoubleSpinBox *Kp_2;
    QLabel *label_Ki_2;
    QDoubleSpinBox *Ki_2;
    QLabel *label_Kd_2;
    QDoubleSpinBox *Kd_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_x;
    QSlider *x;
    QFormLayout *formLayout;
    QLabel *label_dt;
    QSpinBox *dt;
    QLabel *label_type;
    QComboBox *comboBox_type;
    QLabel *label_dt_3;
    QSpinBox *scale;
    QFormLayout *formLayout_5;
    QLabel *label_2;
    QSlider *mincorr;
    QFormLayout *formLayout_2;
    QPushButton *pushButton_wind;
    QSpinBox *spinBox_wind;
    QLabel *label_friction;
    QDoubleSpinBox *doubleSpinBox_friction;
    QLabel *label;
    QSlider *payload;
    QVBoxLayout *verticalLayout_3;
    QPushButton *pushButton_reset;
    QPushButton *pushButton_pause;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *PID_test)
    {
        if (PID_test->objectName().isEmpty())
            PID_test->setObjectName(QString::fromUtf8("PID_test"));
        PID_test->resize(875, 550);
        centralWidget = new QWidget(PID_test);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget_2 = new QWidget(centralWidget);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(10, 0, 751, 361));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        plot = new QwtPlot(verticalLayoutWidget_2);
        plot->setObjectName(QString::fromUtf8("plot"));

        verticalLayout_2->addWidget(plot);

        plot_val = new QwtPlot(verticalLayoutWidget_2);
        plot_val->setObjectName(QString::fromUtf8("plot_val"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plot_val->sizePolicy().hasHeightForWidth());
        plot_val->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(plot_val);

        value = new QProgressBar(centralWidget);
        value->setObjectName(QString::fromUtf8("value"));
        value->setGeometry(QRect(760, 0, 51, 481));
        value->setMinimum(-1000);
        value->setMaximum(1000);
        value->setValue(0);
        value->setOrientation(Qt::Vertical);
        diff = new QProgressBar(centralWidget);
        diff->setObjectName(QString::fromUtf8("diff"));
        diff->setGeometry(QRect(820, 0, 51, 481));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(diff->sizePolicy().hasHeightForWidth());
        diff->setSizePolicy(sizePolicy1);
        diff->setMinimum(-1000);
        diff->setMaximum(1000);
        diff->setValue(0);
        diff->setOrientation(Qt::Vertical);
        horizontalLayoutWidget = new QWidget(centralWidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 370, 761, 110));
        horizontalLayout_4 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_Kp = new QLabel(horizontalLayoutWidget);
        label_Kp->setObjectName(QString::fromUtf8("label_Kp"));

        horizontalLayout->addWidget(label_Kp);

        Kp = new QDoubleSpinBox(horizontalLayoutWidget);
        Kp->setObjectName(QString::fromUtf8("Kp"));
        Kp->setDecimals(3);
        Kp->setMinimum(-100);
        Kp->setMaximum(100);
        Kp->setSingleStep(0.1);

        horizontalLayout->addWidget(Kp);

        label_Ki = new QLabel(horizontalLayoutWidget);
        label_Ki->setObjectName(QString::fromUtf8("label_Ki"));

        horizontalLayout->addWidget(label_Ki);

        Ki = new QDoubleSpinBox(horizontalLayoutWidget);
        Ki->setObjectName(QString::fromUtf8("Ki"));
        Ki->setDecimals(3);
        Ki->setMinimum(-100);
        Ki->setMaximum(100);
        Ki->setSingleStep(0.1);

        horizontalLayout->addWidget(Ki);

        label_Kd = new QLabel(horizontalLayoutWidget);
        label_Kd->setObjectName(QString::fromUtf8("label_Kd"));

        horizontalLayout->addWidget(label_Kd);

        Kd = new QDoubleSpinBox(horizontalLayoutWidget);
        Kd->setObjectName(QString::fromUtf8("Kd"));
        Kd->setDecimals(3);
        Kd->setMinimum(-100);
        Kd->setMaximum(100);
        Kd->setSingleStep(0.1);

        horizontalLayout->addWidget(Kd);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_Kp_2 = new QLabel(horizontalLayoutWidget);
        label_Kp_2->setObjectName(QString::fromUtf8("label_Kp_2"));

        horizontalLayout_3->addWidget(label_Kp_2);

        Kp_2 = new QDoubleSpinBox(horizontalLayoutWidget);
        Kp_2->setObjectName(QString::fromUtf8("Kp_2"));
        Kp_2->setDecimals(3);
        Kp_2->setMinimum(-100);
        Kp_2->setMaximum(100);
        Kp_2->setSingleStep(0.1);

        horizontalLayout_3->addWidget(Kp_2);

        label_Ki_2 = new QLabel(horizontalLayoutWidget);
        label_Ki_2->setObjectName(QString::fromUtf8("label_Ki_2"));

        horizontalLayout_3->addWidget(label_Ki_2);

        Ki_2 = new QDoubleSpinBox(horizontalLayoutWidget);
        Ki_2->setObjectName(QString::fromUtf8("Ki_2"));
        Ki_2->setDecimals(3);
        Ki_2->setMinimum(-100);
        Ki_2->setMaximum(100);
        Ki_2->setSingleStep(0.1);

        horizontalLayout_3->addWidget(Ki_2);

        label_Kd_2 = new QLabel(horizontalLayoutWidget);
        label_Kd_2->setObjectName(QString::fromUtf8("label_Kd_2"));

        horizontalLayout_3->addWidget(label_Kd_2);

        Kd_2 = new QDoubleSpinBox(horizontalLayoutWidget);
        Kd_2->setObjectName(QString::fromUtf8("Kd_2"));
        Kd_2->setDecimals(3);
        Kd_2->setMinimum(-100);
        Kd_2->setMaximum(100);
        Kd_2->setSingleStep(0.1);

        horizontalLayout_3->addWidget(Kd_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_x = new QLabel(horizontalLayoutWidget);
        label_x->setObjectName(QString::fromUtf8("label_x"));

        horizontalLayout_2->addWidget(label_x);

        x = new QSlider(horizontalLayoutWidget);
        x->setObjectName(QString::fromUtf8("x"));
        x->setMinimum(-1000);
        x->setMaximum(1000);
        x->setSingleStep(1);
        x->setPageStep(100);
        x->setTracking(true);
        x->setOrientation(Qt::Horizontal);
        x->setInvertedAppearance(false);
        x->setTickPosition(QSlider::TicksBelow);
        x->setTickInterval(100);

        horizontalLayout_2->addWidget(x);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_4->addLayout(verticalLayout);

        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        label_dt = new QLabel(horizontalLayoutWidget);
        label_dt->setObjectName(QString::fromUtf8("label_dt"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_dt);

        dt = new QSpinBox(horizontalLayoutWidget);
        dt->setObjectName(QString::fromUtf8("dt"));
        dt->setMinimum(1);
        dt->setMaximum(100);
        dt->setValue(10);

        formLayout->setWidget(0, QFormLayout::FieldRole, dt);

        label_type = new QLabel(horizontalLayoutWidget);
        label_type->setObjectName(QString::fromUtf8("label_type"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_type);

        comboBox_type = new QComboBox(horizontalLayoutWidget);
        comboBox_type->setObjectName(QString::fromUtf8("comboBox_type"));

        formLayout->setWidget(2, QFormLayout::FieldRole, comboBox_type);

        label_dt_3 = new QLabel(horizontalLayoutWidget);
        label_dt_3->setObjectName(QString::fromUtf8("label_dt_3"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_dt_3);

        scale = new QSpinBox(horizontalLayoutWidget);
        scale->setObjectName(QString::fromUtf8("scale"));
        scale->setMinimum(1);
        scale->setMaximum(100);
        scale->setValue(10);

        formLayout->setWidget(1, QFormLayout::FieldRole, scale);


        horizontalLayout_4->addLayout(formLayout);

        formLayout_5 = new QFormLayout();
        formLayout_5->setSpacing(6);
        formLayout_5->setObjectName(QString::fromUtf8("formLayout_5"));
        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout_5->setWidget(0, QFormLayout::LabelRole, label_2);

        mincorr = new QSlider(horizontalLayoutWidget);
        mincorr->setObjectName(QString::fromUtf8("mincorr"));
        mincorr->setMinimum(0);
        mincorr->setMaximum(10);
        mincorr->setOrientation(Qt::Horizontal);
        mincorr->setTickPosition(QSlider::TicksAbove);
        mincorr->setTickInterval(1);

        formLayout_5->setWidget(0, QFormLayout::FieldRole, mincorr);


        horizontalLayout_4->addLayout(formLayout_5);

        formLayout_2 = new QFormLayout();
        formLayout_2->setSpacing(6);
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        pushButton_wind = new QPushButton(horizontalLayoutWidget);
        pushButton_wind->setObjectName(QString::fromUtf8("pushButton_wind"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, pushButton_wind);

        spinBox_wind = new QSpinBox(horizontalLayoutWidget);
        spinBox_wind->setObjectName(QString::fromUtf8("spinBox_wind"));
        spinBox_wind->setMinimum(-50);
        spinBox_wind->setMaximum(50);
        spinBox_wind->setValue(1);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, spinBox_wind);

        label_friction = new QLabel(horizontalLayoutWidget);
        label_friction->setObjectName(QString::fromUtf8("label_friction"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_friction);

        doubleSpinBox_friction = new QDoubleSpinBox(horizontalLayoutWidget);
        doubleSpinBox_friction->setObjectName(QString::fromUtf8("doubleSpinBox_friction"));
        doubleSpinBox_friction->setMinimum(-10);
        doubleSpinBox_friction->setMaximum(10);
        doubleSpinBox_friction->setValue(0);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, doubleSpinBox_friction);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label);

        payload = new QSlider(horizontalLayoutWidget);
        payload->setObjectName(QString::fromUtf8("payload"));
        payload->setMinimum(-10);
        payload->setMaximum(10);
        payload->setOrientation(Qt::Horizontal);
        payload->setTickPosition(QSlider::TicksAbove);
        payload->setTickInterval(5);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, payload);


        horizontalLayout_4->addLayout(formLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        pushButton_reset = new QPushButton(horizontalLayoutWidget);
        pushButton_reset->setObjectName(QString::fromUtf8("pushButton_reset"));

        verticalLayout_3->addWidget(pushButton_reset);

        pushButton_pause = new QPushButton(horizontalLayoutWidget);
        pushButton_pause->setObjectName(QString::fromUtf8("pushButton_pause"));

        verticalLayout_3->addWidget(pushButton_pause);


        horizontalLayout_4->addLayout(verticalLayout_3);

        PID_test->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(PID_test);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 875, 25));
        PID_test->setMenuBar(menuBar);
        mainToolBar = new QToolBar(PID_test);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        PID_test->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PID_test);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        PID_test->setStatusBar(statusBar);

        retranslateUi(PID_test);

        comboBox_type->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(PID_test);
    } // setupUi

    void retranslateUi(QMainWindow *PID_test)
    {
        PID_test->setWindowTitle(QApplication::translate("PID_test", "PID_test", 0, QApplication::UnicodeUTF8));
        value->setFormat(QApplication::translate("PID_test", "%v", 0, QApplication::UnicodeUTF8));
        diff->setFormat(QApplication::translate("PID_test", "%v", 0, QApplication::UnicodeUTF8));
        label_Kp->setText(QApplication::translate("PID_test", "Kp", 0, QApplication::UnicodeUTF8));
        label_Ki->setText(QApplication::translate("PID_test", "Ki", 0, QApplication::UnicodeUTF8));
        label_Kd->setText(QApplication::translate("PID_test", "Kd", 0, QApplication::UnicodeUTF8));
        label_Kp_2->setText(QApplication::translate("PID_test", "maxP", 0, QApplication::UnicodeUTF8));
        label_Ki_2->setText(QApplication::translate("PID_test", "maxI", 0, QApplication::UnicodeUTF8));
        label_Kd_2->setText(QApplication::translate("PID_test", "maxD", 0, QApplication::UnicodeUTF8));
        label_x->setText(QApplication::translate("PID_test", "Request", 0, QApplication::UnicodeUTF8));
        label_dt->setText(QApplication::translate("PID_test", "dt", 0, QApplication::UnicodeUTF8));
        label_type->setText(QApplication::translate("PID_test", "type", 0, QApplication::UnicodeUTF8));
        comboBox_type->clear();
        comboBox_type->insertItems(0, QStringList()
         << QApplication::translate("PID_test", "angular velocity", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("PID_test", "angle", 0, QApplication::UnicodeUTF8)
        );
        label_dt_3->setText(QApplication::translate("PID_test", "sc", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("PID_test", "Min Corr", 0, QApplication::UnicodeUTF8));
        pushButton_wind->setText(QApplication::translate("PID_test", "Wind", 0, QApplication::UnicodeUTF8));
        label_friction->setText(QApplication::translate("PID_test", "Friction", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PID_test", "Payload", 0, QApplication::UnicodeUTF8));
        pushButton_reset->setText(QApplication::translate("PID_test", "Reset", 0, QApplication::UnicodeUTF8));
        pushButton_pause->setText(QApplication::translate("PID_test", "Start", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PID_test: public Ui_PID_test {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PID_TEST_H
