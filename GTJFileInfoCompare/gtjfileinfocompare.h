/*!
*@file     gtjfileinfocompare.h
*@brief    界面操作文件
*@author   maozg
*@date     2017年9月27日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJFILEINFOCOMPARE_H
#define GTJFILEINFOCOMPARE_H

#include <QtWidgets/QMainWindow>
#include "ui_gtjfileinfocompare.h"
#include <QTabWidget>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QIcon>
#include <QGridLayout>
#include <QTextEdit>
#include <QTableWidget>
#include <QProgressDialog>
#include "gtjDealWithFileManage.h"


class GTJCollectFileInfo;
class GTJCompareFileInfo;

class CompareResultList : public QWidget
{
    Q_OBJECT
public:
    static CompareResultList& instance();
    void displayCompareData(NoneFileDataMap& lostData, CompareResultDataMap& compareResultMap);
private:
    CompareResultList(QWidget *parent = 0);
    ~CompareResultList();

    CompareResultList(const CompareResultList&);
    CompareResultList& operator = (CompareResultList &) {};

    void iniUi(QWidget *parent = 0);
private slots:
    void saveData();
    void hidList();

private:
    QTableWidget *m_pTable;
    NoneFileDataMap m_lostData;
    CompareResultDataMap m_compareResultMap;
};

class GTJFileInfoCompare : public QWidget
{
    Q_OBJECT
public:
    GTJFileInfoCompare(QWidget *parent = 0);
    ~GTJFileInfoCompare();

private:
    Ui::GTJFileInfoCompareClass ui;
    void iniUi();
private slots:
    void openCollectFileInfoPath();
    void runCollectFinleInfo();
    void openSourceFilePath();
    void openCompareFilePath();
    void runCompareFileInfo();
    void outPutCollectInfo(const QString &strMsg);
    void onCollectFinished();
    void outPutCompareInfo(const QString &strMsg);
    void onCompareFinished(const bool bError);
    void saveCollectResult();
    void displayCompareErrorInfo();

private:
    QTabWidget* m_pTabWidget;
    QLineEdit* m_pFileInfoPath;    //收集文件信息的路径
    QLineEdit* m_pSourceFilePath;  //对比文件信息源文件路径
    QLineEdit* m_pCompareFilePath; //对比文件信息对比文件路径
    QTextEdit* m_pCollOutPut;      //收集文件信息的输出
    QTextEdit* m_pCompareOutPut;   //对比文件信息的输出
    //QTableWidget* m_pSetting;      //设置界面
    QPushButton* m_pCollectBut;
    QPushButton* m_pCompareBut;
    CollectDataMap m_mapCollectFileData;    // 收集文件映射
    CompareResultDataMap m_compareResultMap;
    GTJCollectFileInfo* m_pCollectFileInfo;
    GTJCompareFileInfo* m_pCompareInfo;
};

#endif // GTJFILEINFOCOMPARE_H
