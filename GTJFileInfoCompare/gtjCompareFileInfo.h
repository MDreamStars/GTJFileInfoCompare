/*!
*@file     gtjCompareFileInfo.h
*@brief    �Ա��ļ���Ϣ
*@author   maozg
*@date     2017��9��27��
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJCOMPAREFILEINFO_H
#define GTJCOMPAREFILEINFO_H
#include <QString>
#include <QThread>

#include "Common/GTJCommon.h"
//�˴������ļ���Ϣ�ıȶԣ���Ҫʹ��MD5��Ϣ���бȽ�
/*
* @brief �ļ���Ϣ�ȶ�����ִ����
*
* @author maozg  2017��9��28��
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
    void readXmlToMap(const QString &fileName, CollectDataMap& collectMap); // xml�ļ������ݴ浽map��
    void loadFileInfo();
    void compaerFileMD5Info();
signals:
    void outPutCompareInfo(const QString &strMsg);
    void onFinished(const bool bError);

private:
    QString m_strSrcFilePath; // �����ļ�·��
    QString m_strComFilePath; // �Ա��ļ�·��

    CollectDataMap m_sourceFileDataMap; //�洢�Ա��ļ��ı����ļ���Ϣ
    CollectDataMap m_compareFileDataMap; //�洢��Ҫ�Աȵ��ļ���Ϣ
    NoneFileDataMap m_loseFileData;  //�洢�ԱȺ�ȱʧ�ļ���
    CompareResultDataMap m_compareResultMap; //�洢�ԱȺ�һ�µ��ļ���Ϣ

    //��ʱ����xml��Ϣ
    QString tempComFileName;
    QString tempComFileSize;
    QString tempComFileTime;
    QString tempComFileMD5;

    bool m_bIsError;

};

//ִ�жԱȵ��߳���
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