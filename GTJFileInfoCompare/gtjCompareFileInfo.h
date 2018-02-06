/*!
*@file     gtjCompareFileInfo.h
*@brief    对比文件信息
*@author   maozg
*@date     2017年9月27日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJCOMPAREFILEINFO_H
#define GTJCOMPAREFILEINFO_H
#include <QString>
#include <QThread>

#include "Common/GTJCommon.h"
//此处处理文件信息的比对，主要使用MD5信息进行比较
/*
* @brief 文件信息比对任务执行类
*
* @author maozg  2017年9月28日
*
* @class GTJCompareFileInfo
*/
class GTJCompareFileInfo : public QObject
{
    Q_OBJECT
public:
    GTJCompareFileInfo(const QString &strSrcFilePath, const QString &strComFilePath);
    ~GTJCompareFileInfo();

    void onExecuteCompare();
    NoneFileDataMap& getLostData() { return m_loseFileData; };
    CompareResultDataMap& getInconsistencyData() { return m_compareResultMap; };
private:
    void onExecute();
    void readXmlToMap(const QString &fileName, CollectDataMap& collectMap); // xml文件的内容存到map中
    void loadFileInfo();
    void compaerFileMD5Info();
signals:
    void outPutCompareInfo(const QString &strMsg);
    void onFinished(const bool bError);

private:
    QString m_strSrcFilePath; // 本地文件路径
    QString m_strComFilePath; // 对比文件路径

    CollectDataMap m_sourceFileDataMap; //存储对比文件的本地文件信息
    CollectDataMap m_compareFileDataMap; //存储需要对比的文件信息
    NoneFileDataMap m_loseFileData;  //存储对比后缺失文件名
    CompareResultDataMap m_compareResultMap; //存储对比后不一致的文件信息

    //临时保存xml信息
    QString tempComFileName;
    QString tempComFileSize;
    QString tempComFileTime;
    QString tempComFileMD5;

    bool m_bIsError;

};

//执行对比的线程类
class GTJCompareTask : public QThread
{
    Q_OBJECT
public:
    GTJCompareTask(GTJCompareFileInfo* pCompare, QObject *parent = 0);
    ~GTJCompareTask();
    
    void run();

private:
    GTJCompareFileInfo* m_pTask;
};
#endif