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
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *lblTitle;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QLabel *lblSampleRate;
    QLineEdit *txtSampleRate;
    QLabel *lblDuration;
    QLineEdit *txtDuration;
    QLabel *lblSignalFreq;
    QLineEdit *txtSignalFreq;
    QPushButton *btnAddSignal;
    QComboBox *cmbSignalType;
    QDoubleSpinBox *dsbSignalAmplitude;
    QLabel *lblSignalType;
    QLabel *lblSignalType_2;
    QGroupBox *groupBox_3;
    QLabel *lblFFTScale;
    QComboBox *cmbFFTScale;
    QGroupBox *groupBox_4;
    QLabel *lblCutoffBwSlider;
    QGroupBox *groupBox_5;
    QLabel *lblWindowType;
    QComboBox *cmbWindowType;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_6;
    QGroupBox *groupBox_7;
    QPushButton *btnLowPass;
    QPushButton *btnHighPass;
    QPushButton *btnBandPass;
    QPushButton *btnMovingAvg;
    QPushButton *btnBandStop;
    QPushButton *btnAllPass;
    QPushButton *btnWiener;
    QPushButton *btnKalman;
    QPushButton *btnMedian;
    QGroupBox *groupBox_8;
    QCustomPlot *customPlotTimeOriginal;
    QGroupBox *groupBox_9;
    QCustomPlot *customPlotTimeFiltered;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_14;
    QPushButton *btnAddNoise;
    QPushButton *btnSave;
    QComboBox *cmbNoiseType;
    QLabel *lblNoiseType;
    QLabel *lblNoiseAmplitude;
    QDoubleSpinBox *dsbNoiseAmplitude;
    QPushButton *btnClear;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_10;
    QGroupBox *groupBox_11;
    QLabel *lblSampleRateValue;
    QLabel *lblDurationValue;
    QLabel *lblSignalFreqValue;
    QLabel *txtSampleValue;
    QLabel *txtDurationValue;
    QLabel *txtSignalFreqValue;
    QGroupBox *groupBox_12;
    QCustomPlot *customPlotFreqOriginal;
    QGroupBox *groupBox_13;
    QCustomPlot *customPlotFreqFiltered;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1317, 820);
        MainWindow->setStyleSheet(QString::fromUtf8("/* Global dark canvas for Designer + runtime */\n"
"QWidget {\n"
"    background-color: #1e1e1e;\n"
"    color: #f0f0f0;\n"
"}\n"
"\n"
"/* Inputs */\n"
"QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {\n"
"    background-color: #2a2a2a;\n"
"    color: #f0f0f0;\n"
"    border: 1px solid #444;\n"
"    padding: 4px;\n"
"    selection-background-color: #3a3a3a;\n"
"}\n"
"\n"
"/* Buttons */\n"
"QPushButton {\n"
"    background-color: #2d2d2d;\n"
"    border: 1px solid #444;\n"
"    padding: 6px 12px;\n"
"    border-radius: 6px;\n"
"}\n"
"QPushButton:hover { background-color: #353535; }\n"
"QPushButton:pressed { background-color: #2a2a2a; }\n"
"\n"
"/* GroupBox */\n"
"QGroupBox {\n"
"    border: 1px solid #444;\n"
"    margin-top: 14px;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 6px;\n"
"}\n"
"\n"
"/* Labels */\n"
"QLabel { color: #f0f0f0; }\n"
""));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        lblTitle = new QLabel(centralwidget);
        lblTitle->setObjectName("lblTitle");
        lblTitle->setGeometry(QRect(630, 0, 211, 51));
        QFont font;
        font.setPointSize(20);
        font.setBold(true);
        lblTitle->setFont(font);
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(20, 60, 331, 631));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(verticalLayoutWidget);
        groupBox->setObjectName("groupBox");
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
        groupBox_2 = new QGroupBox(groupBox);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 20, 311, 201));
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        lblSampleRate = new QLabel(groupBox_2);
        lblSampleRate->setObjectName("lblSampleRate");
        lblSampleRate->setGeometry(QRect(10, 20, 221, 20));
        txtSampleRate = new QLineEdit(groupBox_2);
        txtSampleRate->setObjectName("txtSampleRate");
        txtSampleRate->setGeometry(QRect(230, 20, 71, 26));
        lblDuration = new QLabel(groupBox_2);
        lblDuration->setObjectName("lblDuration");
        lblDuration->setGeometry(QRect(10, 50, 221, 20));
        txtDuration = new QLineEdit(groupBox_2);
        txtDuration->setObjectName("txtDuration");
        txtDuration->setGeometry(QRect(230, 50, 71, 26));
        lblSignalFreq = new QLabel(groupBox_2);
        lblSignalFreq->setObjectName("lblSignalFreq");
        lblSignalFreq->setGeometry(QRect(10, 80, 231, 20));
        txtSignalFreq = new QLineEdit(groupBox_2);
        txtSignalFreq->setObjectName("txtSignalFreq");
        txtSignalFreq->setGeometry(QRect(250, 80, 51, 26));
        btnAddSignal = new QPushButton(groupBox_2);
        btnAddSignal->setObjectName("btnAddSignal");
        btnAddSignal->setGeometry(QRect(200, 160, 101, 29));
        cmbSignalType = new QComboBox(groupBox_2);
        cmbSignalType->setObjectName("cmbSignalType");
        cmbSignalType->setGeometry(QRect(200, 110, 101, 28));
        dsbSignalAmplitude = new QDoubleSpinBox(groupBox_2);
        dsbSignalAmplitude->setObjectName("dsbSignalAmplitude");
        dsbSignalAmplitude->setGeometry(QRect(120, 160, 63, 29));
        dsbSignalAmplitude->setValue(1.000000000000000);
        lblSignalType = new QLabel(groupBox_2);
        lblSignalType->setObjectName("lblSignalType");
        lblSignalType->setGeometry(QRect(10, 110, 81, 21));
        lblSignalType_2 = new QLabel(groupBox_2);
        lblSignalType_2->setObjectName("lblSignalType_2");
        lblSignalType_2->setGeometry(QRect(10, 160, 101, 31));
        groupBox_3 = new QGroupBox(groupBox);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(10, 240, 311, 81));
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        lblFFTScale = new QLabel(groupBox_3);
        lblFFTScale->setObjectName("lblFFTScale");
        lblFFTScale->setGeometry(QRect(10, 20, 181, 20));
        cmbFFTScale = new QComboBox(groupBox_3);
        cmbFFTScale->setObjectName("cmbFFTScale");
        cmbFFTScale->setGeometry(QRect(170, 20, 131, 26));
        groupBox_4 = new QGroupBox(groupBox);
        groupBox_4->setObjectName("groupBox_4");
        groupBox_4->setGeometry(QRect(10, 320, 311, 121));
        groupBox_4->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        lblCutoffBwSlider = new QLabel(groupBox_4);
        lblCutoffBwSlider->setObjectName("lblCutoffBwSlider");
        lblCutoffBwSlider->setGeometry(QRect(10, 20, 181, 20));
        groupBox_5 = new QGroupBox(groupBox);
        groupBox_5->setObjectName("groupBox_5");
        groupBox_5->setGeometry(QRect(10, 440, 311, 181));
        groupBox_5->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        lblWindowType = new QLabel(groupBox_5);
        lblWindowType->setObjectName("lblWindowType");
        lblWindowType->setGeometry(QRect(10, 30, 171, 20));
        cmbWindowType = new QComboBox(groupBox_5);
        cmbWindowType->setObjectName("cmbWindowType");
        cmbWindowType->setGeometry(QRect(170, 30, 131, 26));

        verticalLayout->addWidget(groupBox);

        verticalLayoutWidget_2 = new QWidget(centralwidget);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(360, 60, 461, 631));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_6 = new QGroupBox(verticalLayoutWidget_2);
        groupBox_6->setObjectName("groupBox_6");
        groupBox_7 = new QGroupBox(groupBox_6);
        groupBox_7->setObjectName("groupBox_7");
        groupBox_7->setGeometry(QRect(10, 20, 431, 181));
        groupBox_7->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        btnLowPass = new QPushButton(groupBox_7);
        btnLowPass->setObjectName("btnLowPass");
        btnLowPass->setGeometry(QRect(30, 30, 111, 41));
        btnHighPass = new QPushButton(groupBox_7);
        btnHighPass->setObjectName("btnHighPass");
        btnHighPass->setGeometry(QRect(30, 80, 111, 41));
        btnBandPass = new QPushButton(groupBox_7);
        btnBandPass->setObjectName("btnBandPass");
        btnBandPass->setGeometry(QRect(30, 130, 111, 41));
        btnMovingAvg = new QPushButton(groupBox_7);
        btnMovingAvg->setObjectName("btnMovingAvg");
        btnMovingAvg->setGeometry(QRect(160, 130, 111, 41));
        btnBandStop = new QPushButton(groupBox_7);
        btnBandStop->setObjectName("btnBandStop");
        btnBandStop->setGeometry(QRect(160, 30, 111, 41));
        btnAllPass = new QPushButton(groupBox_7);
        btnAllPass->setObjectName("btnAllPass");
        btnAllPass->setGeometry(QRect(160, 80, 111, 41));
        btnWiener = new QPushButton(groupBox_7);
        btnWiener->setObjectName("btnWiener");
        btnWiener->setGeometry(QRect(290, 130, 111, 41));
        btnKalman = new QPushButton(groupBox_7);
        btnKalman->setObjectName("btnKalman");
        btnKalman->setGeometry(QRect(290, 80, 111, 41));
        btnMedian = new QPushButton(groupBox_7);
        btnMedian->setObjectName("btnMedian");
        btnMedian->setGeometry(QRect(290, 30, 111, 41));
        groupBox_8 = new QGroupBox(groupBox_6);
        groupBox_8->setObjectName("groupBox_8");
        groupBox_8->setGeometry(QRect(10, 200, 431, 211));
        groupBox_8->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        customPlotTimeOriginal = new QCustomPlot(groupBox_8);
        customPlotTimeOriginal->setObjectName("customPlotTimeOriginal");
        customPlotTimeOriginal->setGeometry(QRect(20, 20, 391, 181));
        customPlotTimeOriginal->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));
        groupBox_9 = new QGroupBox(groupBox_6);
        groupBox_9->setObjectName("groupBox_9");
        groupBox_9->setGeometry(QRect(10, 410, 431, 211));
        groupBox_9->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        customPlotTimeFiltered = new QCustomPlot(groupBox_9);
        customPlotTimeFiltered->setObjectName("customPlotTimeFiltered");
        customPlotTimeFiltered->setGeometry(QRect(20, 20, 391, 181));
        customPlotTimeFiltered->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));

        verticalLayout_2->addWidget(groupBox_6);

        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(20, 699, 1271, 51));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        groupBox_14 = new QGroupBox(horizontalLayoutWidget);
        groupBox_14->setObjectName("groupBox_14");
        btnAddNoise = new QPushButton(groupBox_14);
        btnAddNoise->setObjectName("btnAddNoise");
        btnAddNoise->setGeometry(QRect(640, 20, 121, 29));
        btnSave = new QPushButton(groupBox_14);
        btnSave->setObjectName("btnSave");
        btnSave->setGeometry(QRect(1110, 20, 121, 29));
        cmbNoiseType = new QComboBox(groupBox_14);
        cmbNoiseType->setObjectName("cmbNoiseType");
        cmbNoiseType->setGeometry(QRect(180, 20, 131, 26));
        lblNoiseType = new QLabel(groupBox_14);
        lblNoiseType->setObjectName("lblNoiseType");
        lblNoiseType->setGeometry(QRect(40, 20, 131, 21));
        lblNoiseAmplitude = new QLabel(groupBox_14);
        lblNoiseAmplitude->setObjectName("lblNoiseAmplitude");
        lblNoiseAmplitude->setGeometry(QRect(350, 20, 131, 21));
        dsbNoiseAmplitude = new QDoubleSpinBox(groupBox_14);
        dsbNoiseAmplitude->setObjectName("dsbNoiseAmplitude");
        dsbNoiseAmplitude->setGeometry(QRect(490, 20, 63, 29));
        dsbNoiseAmplitude->setValue(1.000000000000000);
        btnClear = new QPushButton(groupBox_14);
        btnClear->setObjectName("btnClear");
        btnClear->setGeometry(QRect(970, 20, 121, 29));

        horizontalLayout_2->addWidget(groupBox_14);

        verticalLayoutWidget_3 = new QWidget(centralwidget);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(830, 60, 461, 631));
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        groupBox_10 = new QGroupBox(verticalLayoutWidget_3);
        groupBox_10->setObjectName("groupBox_10");
        groupBox_11 = new QGroupBox(groupBox_10);
        groupBox_11->setObjectName("groupBox_11");
        groupBox_11->setGeometry(QRect(10, 20, 431, 181));
        groupBox_11->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        lblSampleRateValue = new QLabel(groupBox_11);
        lblSampleRateValue->setObjectName("lblSampleRateValue");
        lblSampleRateValue->setGeometry(QRect(40, 50, 191, 20));
        lblDurationValue = new QLabel(groupBox_11);
        lblDurationValue->setObjectName("lblDurationValue");
        lblDurationValue->setGeometry(QRect(40, 80, 221, 20));
        lblSignalFreqValue = new QLabel(groupBox_11);
        lblSignalFreqValue->setObjectName("lblSignalFreqValue");
        lblSignalFreqValue->setGeometry(QRect(40, 110, 231, 20));
        txtSampleValue = new QLabel(groupBox_11);
        txtSampleValue->setObjectName("txtSampleValue");
        txtSampleValue->setGeometry(QRect(270, 50, 101, 20));
        txtSampleValue->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"}"));
        txtDurationValue = new QLabel(groupBox_11);
        txtDurationValue->setObjectName("txtDurationValue");
        txtDurationValue->setGeometry(QRect(270, 80, 101, 20));
        txtDurationValue->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"}"));
        txtSignalFreqValue = new QLabel(groupBox_11);
        txtSignalFreqValue->setObjectName("txtSignalFreqValue");
        txtSignalFreqValue->setGeometry(QRect(270, 110, 101, 20));
        txtSignalFreqValue->setStyleSheet(QString::fromUtf8("QLabel {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"}"));
        groupBox_12 = new QGroupBox(groupBox_10);
        groupBox_12->setObjectName("groupBox_12");
        groupBox_12->setGeometry(QRect(10, 200, 431, 211));
        groupBox_12->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        customPlotFreqOriginal = new QCustomPlot(groupBox_12);
        customPlotFreqOriginal->setObjectName("customPlotFreqOriginal");
        customPlotFreqOriginal->setGeometry(QRect(20, 20, 391, 181));
        customPlotFreqOriginal->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));
        groupBox_13 = new QGroupBox(groupBox_10);
        groupBox_13->setObjectName("groupBox_13");
        groupBox_13->setGeometry(QRect(10, 410, 431, 211));
        groupBox_13->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"    border: 2px solid gray;\n"
"    border-radius: 5px;\n"
"    margin-top: 10px;\n"
"	color:red;\n"
"	border-color:red;\n"
"}\n"
"\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 10px;\n"
"    padding: 0 3px;\n"
"	color:red;\n"
"\n"
"}\n"
""));
        customPlotFreqFiltered = new QCustomPlot(groupBox_13);
        customPlotFreqFiltered->setObjectName("customPlotFreqFiltered");
        customPlotFreqFiltered->setGeometry(QRect(20, 20, 391, 181));
        customPlotFreqFiltered->setStyleSheet(QString::fromUtf8("border: 2px solid gray;\n"
"border-radius: 4px;\n"
""));

        verticalLayout_3->addWidget(groupBox_10);

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
        lblTitle->setText(QCoreApplication::translate("MainWindow", "DSP Analyzer", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        lblSampleRate->setText(QCoreApplication::translate("MainWindow", "\303\226rnekleme Frekan\304\261s\304\261n\304\261 Girin (Hz):", nullptr));
        lblDuration->setText(QCoreApplication::translate("MainWindow", "S\303\274reyi Girin (s):", nullptr));
        lblSignalFreq->setText(QCoreApplication::translate("MainWindow", "Sinyalin Frekans De\304\237erini Girin(Hz):", nullptr));
        btnAddSignal->setText(QCoreApplication::translate("MainWindow", "Sinyal Ekle", nullptr));
        lblSignalType->setText(QCoreApplication::translate("MainWindow", "Sinyal T\303\274r\303\274:", nullptr));
        lblSignalType_2->setText(QCoreApplication::translate("MainWindow", "Genlik De\304\237eri:", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        lblFFTScale->setText(QCoreApplication::translate("MainWindow", "FFT \303\226l\303\247ekleme Se\303\247imi", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        lblCutoffBwSlider->setText(QCoreApplication::translate("MainWindow", "Cutoff / Bandwith Ayar Slider", nullptr));
        groupBox_5->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        lblWindowType->setText(QCoreApplication::translate("MainWindow", "FFT Penceresi Se\303\247imi", nullptr));
        groupBox_6->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_7->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        btnLowPass->setText(QCoreApplication::translate("MainWindow", "Low-Pass", nullptr));
        btnHighPass->setText(QCoreApplication::translate("MainWindow", "High-Pass", nullptr));
        btnBandPass->setText(QCoreApplication::translate("MainWindow", "Band-Pass", nullptr));
        btnMovingAvg->setText(QCoreApplication::translate("MainWindow", "Moving Avg ", nullptr));
        btnBandStop->setText(QCoreApplication::translate("MainWindow", "Band-Stop", nullptr));
        btnAllPass->setText(QCoreApplication::translate("MainWindow", "All-Pass", nullptr));
        btnWiener->setText(QCoreApplication::translate("MainWindow", "Wiener", nullptr));
        btnKalman->setText(QCoreApplication::translate("MainWindow", "Kalman", nullptr));
        btnMedian->setText(QCoreApplication::translate("MainWindow", "Median", nullptr));
        groupBox_8->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_9->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_14->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        btnAddNoise->setText(QCoreApplication::translate("MainWindow", "G\303\274r\303\274lt\303\274 Ekle", nullptr));
        btnSave->setText(QCoreApplication::translate("MainWindow", "Kaydet", nullptr));
        lblNoiseType->setText(QCoreApplication::translate("MainWindow", "G\303\274r\303\274lt\303\274 T\303\274r\303\274 Se\303\247:", nullptr));
        lblNoiseAmplitude->setText(QCoreApplication::translate("MainWindow", "G\303\274r\303\274lt\303\274 \305\236iddeti Se\303\247:", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Temizle", nullptr));
        groupBox_10->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_11->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        lblSampleRateValue->setText(QCoreApplication::translate("MainWindow", "\303\226rnekleme Frekans\304\261 (Hz):", nullptr));
        lblDurationValue->setText(QCoreApplication::translate("MainWindow", "S\303\274re (s):", nullptr));
        lblSignalFreqValue->setText(QCoreApplication::translate("MainWindow", "Sinyallerin Frekans De\304\237erleri (Hz):", nullptr));
        txtSampleValue->setText(QString());
        txtDurationValue->setText(QString());
        txtSignalFreqValue->setText(QString());
        groupBox_12->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
        groupBox_13->setTitle(QCoreApplication::translate("MainWindow", "GroupBox", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
