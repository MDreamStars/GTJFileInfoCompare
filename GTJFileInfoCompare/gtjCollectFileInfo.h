/*!
*@file     gtjCollectFileInfo.h
*@brief    �ռ��ļ���Ϣ
*@author   maozg
*@date     2017��9��27��
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
* @brief �ռ��ļ���Ϣ��
*
* @author maozg  2017��9��27��
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
    void findFileFunc(const QString &collectFilePath); // �ҵ������ļ�������Ϣ�ռ���map��
    void collectFileInfo(const QString &collectFilePath);

private:
    QString            m_strCollectFilePath;    // ���ռ��ļ���·��
    CollectDataMap     m_mapCollectFileData;    // �ռ��ļ�ӳ��
};

//�˴������ļ���Ϣ���ռ�����
//�����߳���
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