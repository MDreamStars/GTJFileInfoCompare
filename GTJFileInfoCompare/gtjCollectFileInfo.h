/*!
*@file     gtjCollectFileInfo.h
*@brief    收集文件信息
*@author   maozg
*@date     2017年9月27日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJCOLLECTFILEINFO_H
#define GTJCOLLECTFILEINFO_H
#include <QThread> 
#include <QString>
#include <QTreeWidget>

#include "gtjfileinfocompare.h"
#include "Common/GTJCommon.h"
#include <QFile>

/*
* @brief 收集文件信息类
*
* @author maozg  2017年9月27日
*
* @class GTJCollectFileInfo
*/

class GTJCollectFileInfo : public QObject
{
    Q_OBJECT
public:
    GTJCollectFileInfo(const QString &strFilePath);
    ~GTJCollectFileInfo();
   
    void onExecuteCollect();
    CollectDataMap & getData() { return m_mapCollectFileData; };
private:
    void onExecute();
signals:
    void outPutCollectInfo(const QString &strMsg);
    void onFinished();

private:
    void findFileFunc(const QString &collectFilePath); // 找到类型文件并把信息收集到map中
    void collectFileInfo(const QString &collectFilePath);

private:
    QString            m_strCollectFilePath;    // 待收集文件的路径
    CollectDataMap     m_mapCollectFileData;    // 收集文件映射
};

//此处进行文件信息的收集操作
//任务线程类
class GTJCollectTaskThread : public QThread
{
    Q_OBJECT
public:
    explicit GTJCollectTaskThread(GTJCollectFileInfo* pCollect,QObject *parent = 0);
    ~GTJCollectTaskThread();
    void run();
private:
    GTJCollectFileInfo* m_pCollect;
};

#endif