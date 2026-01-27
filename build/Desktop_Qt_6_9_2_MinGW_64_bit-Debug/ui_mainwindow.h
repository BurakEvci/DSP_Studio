/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnUndo;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_12;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QSlider *sliderFilterParam;
    QLabel *lblSliderValue;
    QLabel *lblCutoffBwSlider;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_8;
    QGridLayout *gridLayout_7;
    QLabel *lblFFTScale;
    QComboBox *cmbFFTScale;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QLabel *lblSampleRate;
    QLabel *lblDuration;
    QDoubleSpinBox *dsbSignalAmplitude;
    QLabel *lblSignalType;
    QLabel *lblSignalFreq;
    QLabel *lblSignalType_2;
    QPushButton *btnAddSignal;
    QComboBox *cmbSignalType;
    QLineEdit *txtSignalFreq;
    QLineEdit *txtDuration;
    QLineEdit *txtSampleRate;
    QGroupBox *groupBox_5;
    QGridLayout *gridLayout_11;
    QGridLayout *gridLayout_9;
    QLabel *lblWindowType;
    QComboBox *cmbWindowType;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout_Orta;
    QGridLayout *gridLayout_14;
    QGridLayout *gridLayout_15;
    QGroupBox *groupBox_8;
    QCustomPlot *customPlotTimeOriginal;
    QGroupBox *groupBox_9;
    QCustomPlot *customPlotTimeFiltered;
    QGridLayout *gridLayout_16;
    QGroupBox *groupBox_7;
    QGridLayout *gridLayout_2;
    QPushButton *btnHighPass;
    QPushButton *btnAllPass;
    QPushButton *btnBandPass;
    QPushButton *btnMedian;
    QPushButton *btnMovingAvg;
    QPushButton *btnKalman;
    QPushButton *btnLowPass;
    QPushButton *btnBandStop;
    QPushButton *btnWiener;
    QWidget *gridLayoutWidget_13;
    QGridLayout *gridLayout_22;
    QGridLayout *gridLayout_21;
    QGroupBox *groupBox_11;
    QWidget *widget;
    QGridLayout *gridLayout_18;
    QLabel *lblStatMin_2;
    QLabel *lblStatRMS_2;
    QLabel *lblStatMax_2;
    QLabel *lblStatMax;
    QLabel *lblStatMean;
    QLabel *lblStatMin;
    QLabel *lblStatMean_2;
    QLabel *lblStatRMS;
    QWidget *gridLayoutWidget_11;
    QGridLayout *gridLayout_20;
    QGroupBox *groupBox_12;
    QCustomPlot *customPlotFreqOriginal;
    QWidget *gridLayoutWidget_10;
    QGridLayout *gridLayout_19;
    QGroupBox *groupBox_13;
    QCustomPlot *customPlotFreqFiltered;
    QLabel *label;
    QWidget *widget1;
    QGridLayout *gridLayout_23;
    QComboBox *cmbNoiseType;
    QLabel *lblNoiseType;
    QPushButton *btnAddNoise;
    QDoubleSpinBox *dsbNoiseAmplitude;
    QLabel *lblNoiseAmplitude;
    QPushButton *btnReverb;
    QGridLayout *gridLayout_28;
    QPushButton *btnRecord;
    QWidget *widget2;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_27;
    QSlider *sliderVolume;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_3;
    QGridLayout *gridLayout_29;
    QPushButton *btnSave;
    QPushButton *btnLoad;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *btnClear;
    QSpacerItem *horizontalSpacer_5;
    QGridLayout *gridLayout_26;
    QPushButton *btnPlayOutput;
    QGridLayout *gridLayout_25;
    QPushButton *btnPlayInput;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_7;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1317, 918);
        MainWindow->setStyleSheet(QString::fromUtf8("/* --- 1. ANA PENCERE (Derin Uzay Mavisi) --- */\n"
"QMainWindow {\n"
"    background-color: #10151e; /* Referans g\303\266rseldeki koyu ton */\n"
"}\n"
"\n"
"/* --- 2. YAZILAR (Okunabilir Parlak Gri) --- */\n"
"QLabel, QCheckBox, QRadioButton {\n"
"    color: #cfd8dc; \n"
"    font-family: 'Segoe UI', sans-serif;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* --- 3. GRUP KUTULARI (KIRMIZI YOK! Sadece Neon Mavi) --- */\n"
"QGroupBox {\n"
"    border: 1px solid #2a3b4c; /* \303\207er\303\247eve: \303\207ok silik mavi-gri */\n"
"    border-radius: 10px;       /* Yumu\305\237ak k\303\266\305\237eler */\n"
"    margin-top: 22px; \n"
"    background-color: #161e2b; /* Kutunun i\303\247i: Ana fondan bir t\304\261k a\303\247\304\261k */\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 15px;\n"
"    padding: 0 5px;\n"
"    color: #00e5ff; /* Ba\305\237l\304\261klar: NEON CYAN (Referans g\303\266rseldeki renk) */\n"
"    font-size: 13px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"/* --- 4. "
                        "BUTONLAR (Referanstaki O Parlak Mavi Ge\303\247i\305\237) --- */\n"
"QPushButton {\n"
"    /* Mavi Gradient: \303\234stten alta koyula\305\237an profesyonel ton */\n"
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #0288d1, stop:1 #01579b);\n"
"    color: white;\n"
"    border: 1px solid #0288d1; /* Kendi renginde \303\247er\303\247eve */\n"
"    border-radius: 6px;        /* G\303\266rseldeki gibi hafif yuvarlak */\n"
"    padding: 6px;\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"    /* \303\234zerine gelince parlas\304\261n (Cyan'a kays\304\261n) */\n"
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #29b6f6, stop:1 #0277bd);\n"
"    border: 1px solid #4fc3f7;\n"
"}\n"
"\n"
"QPushButton:pressed {\n"
"    background-color: #01457b; /* T\304\261klay\304\261nca koyula\305\237s\304\261n */\n"
"    padding-top: 8px;\n"
"}\n"
"\n"
"/* --- 5. \303\226ZEL BUTONLAR --- */\n"
"\n"
"/* \"Sinyal Ekle\", \"G\303\274r\303\274lt\303\274"
                        " Ekle\" gibi aksiyon butonlar\304\261 (Turkuaz) */\n"
"/* E\304\237er ObjectName'leri belliyse buraya ekleyebilirsin, yoksa genel buton stili i\305\237 g\303\266r\303\274r */\n"
"\n"
"/* KAYIT BUTONU (Hala K\304\261rm\304\261z\304\261 Kals\304\261n, Dikkat \303\207eksin) */\n"
"#btnRecord {\n"
"    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ff5252, stop:1 #d32f2f);\n"
"    border: 1px solid #ff1744;\n"
"}\n"
"#btnRecord:hover {\n"
"    background-color: #ff867f;\n"
"}\n"
"\n"
"/* --- 6. G\304\260RD\304\260 KUTULARI (Textbox & Combobox) --- */\n"
"QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {\n"
"    background-color: #0d1219; /* \303\207ok koyu zemin */\n"
"    border: 1px solid #37474f;\n"
"    border-radius: 4px;\n"
"    color: #00e5ff; /* \304\260\303\247indeki rakamlar Neon Cyan */\n"
"    padding: 4px;\n"
"}\n"
"\n"
"/* --- 7. SLIDER (Modern \303\207ubuk) --- */\n"
"QSlider::groove:horizontal {\n"
"    border: 1px solid #37474f;\n"
"    height: 6px;\n"
"    background: #0d1219;\n"
""
                        "    margin: 2px 0;\n"
"    border-radius: 3px;\n"
"}\n"
"QSlider::handle:horizontal {\n"
"    background: #00e5ff; /* Tutama\303\247: Neon Cyan */\n"
"    border: 1px solid #00e5ff;\n"
"    width: 14px;\n"
"    height: 14px;\n"
"    margin: -5px 0;\n"
"    border-radius: 7px;\n"
"}"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnUndo = new QPushButton(centralwidget);
        btnUndo->setObjectName("btnUndo");
        btnUndo->setGeometry(QRect(10, 0, 38, 34));
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::EditUndo));
        btnUndo->setIcon(icon);
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 30, 348, 530));
        groupBox->setMaximumSize(QSize(350, 16777215));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"}\n"
""));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName("gridLayout_12");
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setMaximumSize(QSize(320, 16777215));
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        gridLayout_6 = new QGridLayout(groupBox_4);
        gridLayout_6->setObjectName("gridLayout_6");
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName("gridLayout_5");
        sliderFilterParam = new QSlider(groupBox_4);
        sliderFilterParam->setObjectName("sliderFilterParam");
        sliderFilterParam->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_5->addWidget(sliderFilterParam, 2, 0, 1, 1);

        lblSliderValue = new QLabel(groupBox_4);
        lblSliderValue->setObjectName("lblSliderValue");

        gridLayout_5->addWidget(lblSliderValue, 1, 0, 1, 1);

        lblCutoffBwSlider = new QLabel(groupBox_4);
        lblCutoffBwSlider->setObjectName("lblCutoffBwSlider");

        gridLayout_5->addWidget(lblCutoffBwSlider, 0, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_4, 1, 0, 1, 1);

        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setMaximumSize(QSize(320, 16777215));
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        gridLayout_8 = new QGridLayout(groupBox_3);
        gridLayout_8->setObjectName("gridLayout_8");
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName("gridLayout_7");
        lblFFTScale = new QLabel(groupBox_3);
        lblFFTScale->setObjectName("lblFFTScale");

        gridLayout_7->addWidget(lblFFTScale, 0, 0, 1, 1);

        cmbFFTScale = new QComboBox(groupBox_3);
        cmbFFTScale->setObjectName("cmbFFTScale");

        gridLayout_7->addWidget(cmbFFTScale, 0, 1, 1, 1);


        gridLayout_8->addLayout(gridLayout_7, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_3, 2, 0, 1, 1);

        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setMaximumSize(QSize(320, 16777215));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName("gridLayout_4");
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName("gridLayout_3");
        lblSampleRate = new QLabel(groupBox_2);
        lblSampleRate->setObjectName("lblSampleRate");

        gridLayout_3->addWidget(lblSampleRate, 0, 0, 1, 1);

        lblDuration = new QLabel(groupBox_2);
        lblDuration->setObjectName("lblDuration");

        gridLayout_3->addWidget(lblDuration, 1, 0, 1, 1);

        dsbSignalAmplitude = new QDoubleSpinBox(groupBox_2);
        dsbSignalAmplitude->setObjectName("dsbSignalAmplitude");
        dsbSignalAmplitude->setValue(1.000000000000000);

        gridLayout_3->addWidget(dsbSignalAmplitude, 4, 2, 1, 1);

        lblSignalType = new QLabel(groupBox_2);
        lblSignalType->setObjectName("lblSignalType");

        gridLayout_3->addWidget(lblSignalType, 3, 0, 1, 1);

        lblSignalFreq = new QLabel(groupBox_2);
        lblSignalFreq->setObjectName("lblSignalFreq");

        gridLayout_3->addWidget(lblSignalFreq, 2, 0, 1, 1);

        lblSignalType_2 = new QLabel(groupBox_2);
        lblSignalType_2->setObjectName("lblSignalType_2");

        gridLayout_3->addWidget(lblSignalType_2, 4, 0, 1, 1);

        btnAddSignal = new QPushButton(groupBox_2);
        btnAddSignal->setObjectName("btnAddSignal");

        gridLayout_3->addWidget(btnAddSignal, 4, 3, 1, 1);

        cmbSignalType = new QComboBox(groupBox_2);
        cmbSignalType->setObjectName("cmbSignalType");

        gridLayout_3->addWidget(cmbSignalType, 3, 2, 1, 2);

        txtSignalFreq = new QLineEdit(groupBox_2);
        txtSignalFreq->setObjectName("txtSignalFreq");

        gridLayout_3->addWidget(txtSignalFreq, 2, 2, 1, 2);

        txtDuration = new QLineEdit(groupBox_2);
        txtDuration->setObjectName("txtDuration");

        gridLayout_3->addWidget(txtDuration, 1, 2, 1, 2);

        txtSampleRate = new QLineEdit(groupBox_2);
        txtSampleRate->setObjectName("txtSampleRate");

        gridLayout_3->addWidget(txtSampleRate, 0, 2, 1, 2);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setMaximumSize(QSize(320, 16777215));
        groupBox_5->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        gridLayout_11 = new QGridLayout(groupBox_5);
        gridLayout_11->setObjectName("gridLayout_11");
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName("gridLayout_9");
        lblWindowType = new QLabel(groupBox_5);
        lblWindowType->setObjectName("lblWindowType");

        gridLayout_9->addWidget(lblWindowType, 0, 0, 1, 1);

        cmbWindowType = new QComboBox(groupBox_5);
        cmbWindowType->setObjectName("cmbWindowType");

        gridLayout_9->addWidget(cmbWindowType, 0, 1, 1, 1);


        gridLayout_11->addLayout(gridLayout_9, 0, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_5, 3, 0, 1, 1);


        gridLayout->addLayout(gridLayout_12, 0, 0, 1, 1);

        gridLayoutWidget = new QWidget(centralwidget);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(400, 30, 419, 661));
        gridLayout_Orta = new QGridLayout(gridLayoutWidget);
        gridLayout_Orta->setObjectName("gridLayout_Orta");
        gridLayout_Orta->setContentsMargins(0, 0, 0, 0);
        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName("gridLayout_14");
        gridLayout_15 = new QGridLayout();
        gridLayout_15->setObjectName("gridLayout_15");
        groupBox_8 = new QGroupBox(gridLayoutWidget);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setMaximumSize(QSize(16777215, 220));
        groupBox_8->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        customPlotTimeOriginal = new QCustomPlot(groupBox_8);
        customPlotTimeOriginal->setObjectName("customPlotTimeOriginal");
        customPlotTimeOriginal->setGeometry(QRect(10, 20, 391, 191));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(customPlotTimeOriginal->sizePolicy().hasHeightForWidth());
        customPlotTimeOriginal->setSizePolicy(sizePolicy);
        customPlotTimeOriginal->setMinimumSize(QSize(0, 0));
        customPlotTimeOriginal->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));

        gridLayout_15->addWidget(groupBox_8, 1, 0, 1, 1);

        groupBox_9 = new QGroupBox(gridLayoutWidget);
        groupBox_9->setObjectName("groupBox_9");
        groupBox_9->setMaximumSize(QSize(16777215, 220));
        groupBox_9->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        customPlotTimeFiltered = new QCustomPlot(groupBox_9);
        customPlotTimeFiltered->setObjectName("customPlotTimeFiltered");
        customPlotTimeFiltered->setGeometry(QRect(10, 20, 391, 191));
        sizePolicy.setHeightForWidth(customPlotTimeFiltered->sizePolicy().hasHeightForWidth());
        customPlotTimeFiltered->setSizePolicy(sizePolicy);
        customPlotTimeFiltered->setMinimumSize(QSize(0, 0));
        customPlotTimeFiltered->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));

        gridLayout_15->addWidget(groupBox_9, 2, 0, 1, 1);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName("gridLayout_16");
        groupBox_7 = new QGroupBox(gridLayoutWidget);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setMaximumSize(QSize(16777215, 220));
        groupBox_7->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        gridLayout_2 = new QGridLayout(groupBox_7);
        gridLayout_2->setObjectName("gridLayout_2");
        btnHighPass = new QPushButton(groupBox_7);
        btnHighPass->setObjectName("btnHighPass");

        gridLayout_2->addWidget(btnHighPass, 1, 2, 1, 1);

        btnAllPass = new QPushButton(groupBox_7);
        btnAllPass->setObjectName("btnAllPass");

        gridLayout_2->addWidget(btnAllPass, 1, 1, 1, 1);

        btnBandPass = new QPushButton(groupBox_7);
        btnBandPass->setObjectName("btnBandPass");

        gridLayout_2->addWidget(btnBandPass, 2, 2, 1, 1);

        btnMedian = new QPushButton(groupBox_7);
        btnMedian->setObjectName("btnMedian");

        gridLayout_2->addWidget(btnMedian, 0, 0, 1, 1);

        btnMovingAvg = new QPushButton(groupBox_7);
        btnMovingAvg->setObjectName("btnMovingAvg");

        gridLayout_2->addWidget(btnMovingAvg, 2, 1, 1, 1);

        btnKalman = new QPushButton(groupBox_7);
        btnKalman->setObjectName("btnKalman");

        gridLayout_2->addWidget(btnKalman, 1, 0, 1, 1);

        btnLowPass = new QPushButton(groupBox_7);
        btnLowPass->setObjectName("btnLowPass");

        gridLayout_2->addWidget(btnLowPass, 0, 2, 1, 1);

        btnBandStop = new QPushButton(groupBox_7);
        btnBandStop->setObjectName("btnBandStop");

        gridLayout_2->addWidget(btnBandStop, 0, 1, 1, 1);

        btnWiener = new QPushButton(groupBox_7);
        btnWiener->setObjectName("btnWiener");

        gridLayout_2->addWidget(btnWiener, 2, 0, 1, 1);


        gridLayout_16->addWidget(groupBox_7, 1, 0, 1, 1);


        gridLayout_15->addLayout(gridLayout_16, 0, 0, 1, 1);


        gridLayout_14->addLayout(gridLayout_15, 2, 0, 1, 1);


        gridLayout_Orta->addLayout(gridLayout_14, 0, 0, 1, 1);

        gridLayoutWidget_13 = new QWidget(centralwidget);
        gridLayoutWidget_13->setObjectName("gridLayoutWidget_13");
        gridLayoutWidget_13->setGeometry(QRect(840, 30, 461, 661));
        gridLayout_22 = new QGridLayout(gridLayoutWidget_13);
        gridLayout_22->setObjectName("gridLayout_22");
        gridLayout_22->setContentsMargins(0, 0, 0, 0);
        gridLayout_21 = new QGridLayout();
        gridLayout_21->setObjectName("gridLayout_21");
        groupBox_11 = new QGroupBox(gridLayoutWidget_13);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setStyleSheet(QString::fromUtf8(""));
        widget = new QWidget(groupBox_11);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(11, 31, 421, 181));
        gridLayout_18 = new QGridLayout(widget);
        gridLayout_18->setObjectName("gridLayout_18");
        gridLayout_18->setContentsMargins(0, 0, 0, 0);
        lblStatMin_2 = new QLabel(widget);
        lblStatMin_2->setObjectName("lblStatMin_2");

        gridLayout_18->addWidget(lblStatMin_2, 1, 0, 1, 1);

        lblStatRMS_2 = new QLabel(widget);
        lblStatRMS_2->setObjectName("lblStatRMS_2");

        gridLayout_18->addWidget(lblStatRMS_2, 3, 0, 1, 1);

        lblStatMax_2 = new QLabel(widget);
        lblStatMax_2->setObjectName("lblStatMax_2");

        gridLayout_18->addWidget(lblStatMax_2, 0, 0, 1, 1);

        lblStatMax = new QLabel(widget);
        lblStatMax->setObjectName("lblStatMax");
        lblStatMax->setFrameShape(QFrame::Shape::Box);

        gridLayout_18->addWidget(lblStatMax, 0, 2, 1, 1);

        lblStatMean = new QLabel(widget);
        lblStatMean->setObjectName("lblStatMean");
        lblStatMean->setFrameShape(QFrame::Shape::Box);

        gridLayout_18->addWidget(lblStatMean, 2, 2, 1, 1);

        lblStatMin = new QLabel(widget);
        lblStatMin->setObjectName("lblStatMin");
        lblStatMin->setFrameShape(QFrame::Shape::Box);

        gridLayout_18->addWidget(lblStatMin, 1, 2, 1, 1);

        lblStatMean_2 = new QLabel(widget);
        lblStatMean_2->setObjectName("lblStatMean_2");

        gridLayout_18->addWidget(lblStatMean_2, 2, 0, 1, 2);

        lblStatRMS = new QLabel(widget);
        lblStatRMS->setObjectName("lblStatRMS");
        lblStatRMS->setFrameShape(QFrame::Shape::Box);

        gridLayout_18->addWidget(lblStatRMS, 3, 2, 1, 1);

        gridLayoutWidget_11 = new QWidget(groupBox_11);
        gridLayoutWidget_11->setObjectName("gridLayoutWidget_11");
        gridLayoutWidget_11->setGeometry(QRect(11, 220, 421, 211));
        gridLayout_20 = new QGridLayout(gridLayoutWidget_11);
        gridLayout_20->setObjectName("gridLayout_20");
        gridLayout_20->setContentsMargins(0, 0, 0, 0);
        groupBox_12 = new QGroupBox(gridLayoutWidget_11);
        groupBox_12->setObjectName("groupBox_12");
        groupBox_12->setMaximumSize(QSize(16777215, 220));
        groupBox_12->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        customPlotFreqOriginal = new QCustomPlot(groupBox_12);
        customPlotFreqOriginal->setObjectName("customPlotFreqOriginal");
        customPlotFreqOriginal->setGeometry(QRect(7, 23, 401, 181));
        sizePolicy.setHeightForWidth(customPlotFreqOriginal->sizePolicy().hasHeightForWidth());
        customPlotFreqOriginal->setSizePolicy(sizePolicy);
        customPlotFreqOriginal->setMinimumSize(QSize(0, 0));
        customPlotFreqOriginal->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));

        gridLayout_20->addWidget(groupBox_12, 0, 0, 1, 1);

        gridLayoutWidget_10 = new QWidget(groupBox_11);
        gridLayoutWidget_10->setObjectName("gridLayoutWidget_10");
        gridLayoutWidget_10->setGeometry(QRect(11, 440, 421, 211));
        gridLayout_19 = new QGridLayout(gridLayoutWidget_10);
        gridLayout_19->setObjectName("gridLayout_19");
        gridLayout_19->setContentsMargins(0, 0, 0, 0);
        groupBox_13 = new QGroupBox(gridLayoutWidget_10);
        groupBox_13->setObjectName("groupBox_13");
        groupBox_13->setMaximumSize(QSize(16777215, 220));
        groupBox_13->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:white;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:white;\n"
"\n"
"}\n"
""));
        customPlotFreqFiltered = new QCustomPlot(groupBox_13);
        customPlotFreqFiltered->setObjectName("customPlotFreqFiltered");
        customPlotFreqFiltered->setGeometry(QRect(10, 20, 401, 181));
        sizePolicy.setHeightForWidth(customPlotFreqFiltered->sizePolicy().hasHeightForWidth());
        customPlotFreqFiltered->setSizePolicy(sizePolicy);
        customPlotFreqFiltered->setMinimumSize(QSize(0, 0));
        customPlotFreqFiltered->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));

        gridLayout_19->addWidget(groupBox_13, 0, 0, 1, 1);


        gridLayout_21->addWidget(groupBox_11, 0, 0, 1, 1);


        gridLayout_22->addLayout(gridLayout_21, 0, 0, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(640, 0, 111, 20));
        widget1 = new QWidget(centralwidget);
        widget1->setObjectName("widget1");
        widget1->setGeometry(QRect(30, 580, 331, 152));
        gridLayout_23 = new QGridLayout(widget1);
        gridLayout_23->setObjectName("gridLayout_23");
        gridLayout_23->setContentsMargins(0, 0, 0, 0);
        cmbNoiseType = new QComboBox(widget1);
        cmbNoiseType->setObjectName("cmbNoiseType");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmbNoiseType->sizePolicy().hasHeightForWidth());
        cmbNoiseType->setSizePolicy(sizePolicy1);

        gridLayout_23->addWidget(cmbNoiseType, 0, 1, 1, 1);

        lblNoiseType = new QLabel(widget1);
        lblNoiseType->setObjectName("lblNoiseType");

        gridLayout_23->addWidget(lblNoiseType, 0, 0, 1, 1);

        btnAddNoise = new QPushButton(widget1);
        btnAddNoise->setObjectName("btnAddNoise");

        gridLayout_23->addWidget(btnAddNoise, 2, 0, 1, 1);

        dsbNoiseAmplitude = new QDoubleSpinBox(widget1);
        dsbNoiseAmplitude->setObjectName("dsbNoiseAmplitude");
        sizePolicy1.setHeightForWidth(dsbNoiseAmplitude->sizePolicy().hasHeightForWidth());
        dsbNoiseAmplitude->setSizePolicy(sizePolicy1);
        dsbNoiseAmplitude->setValue(1.000000000000000);

        gridLayout_23->addWidget(dsbNoiseAmplitude, 1, 1, 1, 1);

        lblNoiseAmplitude = new QLabel(widget1);
        lblNoiseAmplitude->setObjectName("lblNoiseAmplitude");

        gridLayout_23->addWidget(lblNoiseAmplitude, 1, 0, 1, 1);

        btnReverb = new QPushButton(widget1);
        btnReverb->setObjectName("btnReverb");

        gridLayout_23->addWidget(btnReverb, 3, 0, 1, 1);

        gridLayout_28 = new QGridLayout();
        gridLayout_28->setObjectName("gridLayout_28");
        btnRecord = new QPushButton(widget1);
        btnRecord->setObjectName("btnRecord");
        btnRecord->setStyleSheet(QString::fromUtf8(""));
        btnRecord->setCheckable(true);

        gridLayout_28->addWidget(btnRecord, 0, 0, 1, 1);


        gridLayout_23->addLayout(gridLayout_28, 3, 1, 1, 1);

        widget2 = new QWidget(centralwidget);
        widget2->setObjectName("widget2");
        widget2->setGeometry(QRect(400, 690, 901, 69));
        gridLayout_10 = new QGridLayout(widget2);
        gridLayout_10->setObjectName("gridLayout_10");
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        gridLayout_27 = new QGridLayout();
        gridLayout_27->setObjectName("gridLayout_27");
        sliderVolume = new QSlider(widget2);
        sliderVolume->setObjectName("sliderVolume");
        sliderVolume->setMaximum(100);
        sliderVolume->setValue(50);
        sliderVolume->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_27->addWidget(sliderVolume, 0, 0, 1, 1);


        gridLayout_10->addLayout(gridLayout_27, 1, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_2, 1, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer, 1, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_3, 1, 7, 1, 1);

        gridLayout_29 = new QGridLayout();
        gridLayout_29->setObjectName("gridLayout_29");
        btnSave = new QPushButton(widget2);
        btnSave->setObjectName("btnSave");

        gridLayout_29->addWidget(btnSave, 0, 1, 1, 1);

        btnLoad = new QPushButton(widget2);
        btnLoad->setObjectName("btnLoad");

        gridLayout_29->addWidget(btnLoad, 0, 3, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_29->addItem(horizontalSpacer_4, 0, 0, 1, 1);

        btnClear = new QPushButton(widget2);
        btnClear->setObjectName("btnClear");

        gridLayout_29->addWidget(btnClear, 0, 2, 1, 1);


        gridLayout_10->addLayout(gridLayout_29, 1, 9, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_5, 1, 8, 1, 1);

        gridLayout_26 = new QGridLayout();
        gridLayout_26->setObjectName("gridLayout_26");
        btnPlayOutput = new QPushButton(widget2);
        btnPlayOutput->setObjectName("btnPlayOutput");
        QIcon icon1(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
        btnPlayOutput->setIcon(icon1);
        btnPlayOutput->setIconSize(QSize(24, 24));

        gridLayout_26->addWidget(btnPlayOutput, 0, 0, 1, 1);


        gridLayout_10->addLayout(gridLayout_26, 1, 6, 1, 1);

        gridLayout_25 = new QGridLayout();
        gridLayout_25->setObjectName("gridLayout_25");
        btnPlayInput = new QPushButton(widget2);
        btnPlayInput->setObjectName("btnPlayInput");
        btnPlayInput->setIcon(icon1);

        gridLayout_25->addWidget(btnPlayInput, 0, 0, 1, 1);


        gridLayout_10->addLayout(gridLayout_25, 1, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_6, 1, 4, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_7, 1, 5, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1317, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnUndo->setText(QString());
        groupBox->setTitle(QString());
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "Filtre Parametreleri", nullptr));
        lblSliderValue->setText(QCoreApplication::translate("MainWindow", "De\304\237er: 5", nullptr));
        lblCutoffBwSlider->setText(QCoreApplication::translate("MainWindow", "Cutoff / Bandwith Ayar Slider", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "Analiz Ayarlar\304\261", nullptr));
        lblFFTScale->setText(QCoreApplication::translate("MainWindow", "FFT \303\226l\303\247ekleme Se\303\247imi", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Sinyal \303\234reteci Ayarlar\304\261", nullptr));
        lblSampleRate->setText(QCoreApplication::translate("MainWindow", "\303\226rnekleme Frekan\304\261s\304\261n\304\261 Girin (Hz):", nullptr));
        lblDuration->setText(QCoreApplication::translate("MainWindow", "S\303\274reyi Girin (s):", nullptr));
        lblSignalType->setText(QCoreApplication::translate("MainWindow", "Sinyal T\303\274r\303\274:", nullptr));
        lblSignalFreq->setText(QCoreApplication::translate("MainWindow", "Sinyalin Frekans De\304\237erini Girin(Hz):", nullptr));
        lblSignalType_2->setText(QCoreApplication::translate("MainWindow", "Genlik De\304\237eri:", nullptr));
        btnAddSignal->setText(QCoreApplication::translate("MainWindow", "Sinyal Ekle", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "FFT Pencere Se\303\247imi", nullptr));
        lblWindowType->setText(QCoreApplication::translate("MainWindow", "FFT Penceresi Se\303\247imi", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "Zaman Domeni (Giri\305\237)", nullptr));
        groupBox_9->setTitle(QCoreApplication::translate("MainWindow", "Zaman Domeni (\303\207\304\261k\304\261\305\237/Filtreli)", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "Filtre Kontrol Paneli", nullptr));
        btnHighPass->setText(QCoreApplication::translate("MainWindow", "High-Pass", nullptr));
        btnAllPass->setText(QCoreApplication::translate("MainWindow", "All-Pass", nullptr));
        btnBandPass->setText(QCoreApplication::translate("MainWindow", "Band-Pass", nullptr));
        btnMedian->setText(QCoreApplication::translate("MainWindow", "Median", nullptr));
        btnMovingAvg->setText(QCoreApplication::translate("MainWindow", "Moving Avg ", nullptr));
        btnKalman->setText(QCoreApplication::translate("MainWindow", "Kalman", nullptr));
        btnLowPass->setText(QCoreApplication::translate("MainWindow", "Low-Pass", nullptr));
        btnBandStop->setText(QCoreApplication::translate("MainWindow", "Band-Stop", nullptr));
        btnWiener->setText(QCoreApplication::translate("MainWindow", "Wiener", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("MainWindow", "Canl\304\261 Sinyal \304\260statistikleri", nullptr));
        lblStatMin_2->setText(QCoreApplication::translate("MainWindow", "Minimum:", nullptr));
        lblStatRMS_2->setText(QCoreApplication::translate("MainWindow", "RMS De\304\237eri:", nullptr));
        lblStatMax_2->setText(QCoreApplication::translate("MainWindow", "Maksimum:", nullptr));
        lblStatMax->setText(QString());
        lblStatMean->setText(QString());
        lblStatMin->setText(QString());
        lblStatMean_2->setText(QCoreApplication::translate("MainWindow", "Ortalama (DC):", nullptr));
        lblStatRMS->setText(QString());
        groupBox_12->setTitle(QCoreApplication::translate("MainWindow", "Frekans Spektrumu (Giri\305\237)", nullptr));
        groupBox_13->setTitle(QCoreApplication::translate("MainWindow", "Frekans Spektrumu (\303\207\304\261k\304\261\305\237)", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "DSP ANALYZER", nullptr));
        lblNoiseType->setText(QCoreApplication::translate("MainWindow", "G\303\274r\303\274lt\303\274 T\303\274r\303\274 Se\303\247:", nullptr));
        btnAddNoise->setText(QCoreApplication::translate("MainWindow", "G\303\274r\303\274lt\303\274 Ekle", nullptr));
        lblNoiseAmplitude->setText(QCoreApplication::translate("MainWindow", "G\303\274r\303\274lt\303\274 \305\236iddeti Se\303\247:", nullptr));
        btnReverb->setText(QCoreApplication::translate("MainWindow", "Delay/Reverb", nullptr));
        btnRecord->setText(QCoreApplication::translate("MainWindow", "Ses Kaydet", nullptr));
        btnSave->setText(QCoreApplication::translate("MainWindow", "Kaydet", nullptr));
        btnLoad->setText(QCoreApplication::translate("MainWindow", "Y\303\274kle", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Temizle", nullptr));
        btnPlayOutput->setText(QString());
        btnPlayInput->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
