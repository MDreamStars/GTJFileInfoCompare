 /*!
 *@file     gtjCollectFileInfo.cpp
 *@brief    �ռ��ļ���Ϣ
 *@author   maozg
 *@date     2017��9��27��
 *@remarks
 *@version 3.0
 *Copyright (c) 1998-2017 Glodon Corporation
 */
#include <QDir>
#include <QFileInfo>
#include <QtWidgets/QMainWindow>
#include <Qmap>
#include <Qlist>
#include <QString>

#include "gtjCollectFileInfo.h"
#include "Common/GTJCommon.h"
#include "ui_gtjfileinfocompare.h"
#include "gtjfileinfocompare.h"
#include "GTJFileInfoCompare.h"
//#include "GTJParseFinleCommon.h"
#include "gtjDealWithFileManage.h"

/*!
*@brief        
*@author       maozg
*@time         2017��9��27��
*@param        QObject *parent /*= 0*/
/*@return       
    */
GTJCollectTaskThread::GTJCollectTaskThread(GTJCollectFileInfo* pCollect, QObject *parent /*= 0*/)
    : QThread(parent), m_pCollect(pCollect)
{
    
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��27��
*@param        
*@return       
*/
GTJCollectTaskThread::~GTJCollectTaskThread()
{
    m_pCollect = nullptr;
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��27��
*@param        
*@return       void
*/
void GTJCollectTaskThread::run()
{
    m_pCollect->onExecuteCollect(); 
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��27��
*@param        const QString &strFilePath
*@return       
*/
GTJCollectFileInfo::GTJCollectFileInfo(const QString &strFilePath)
    : m_strCollectFilePath(strFilePath)
{
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��27��
*@param        
*@return       
*/
GTJCollectFileInfo::~GTJCollectFileInfo()
{
    m_mapCollectFileData.clear();
}

/*!
*@brief   ���������ļ��У��ռ�dll��GDB��GSP�ļ�     
*@author       maozg
*@time         2017��9��27��
*@param        
*@return       void
*/
void GTJCollectFileInfo::onExecute()
{
    outPutCollectInfo(CHAR_FORMAT(L"��ʼ�ռ��ļ���Ϣ!"));
    //�ռ��ļ���Ϣ
    findFileFunc(m_strCollectFilePath);
    outPutCollectInfo(CHAR_FORMAT(L"�ļ���Ϣ�ռ����!"));
    onFinished();
}

/*!
*@brief   �ݹ鱣�����Ŀ¼�����з���Ҫ����ļ�     
*@author       caohy
*@time         2017��9��29��
*@param        
*@return       void
*/
void GTJCollectFileInfo::findFileFunc(const QString &collectFilePath)
{
    //���ռ���ǰĿ¼
    collectFileInfo(collectFilePath);

    //�ռ���Ŀ¼
    QDir dir(collectFilePath);
    QStringList dirlist = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    if (dirlist.empty())
    {
        return;
    }

    foreach(QString subDir, dirlist)
    {
        findFileFunc(collectFilePath + QDir::separator() + subDir);
    }
}

/*!
*@brief        �ռ��ļ�����
*@author       maozg
*@time         2017��10��20��
*@param        const QString &collectFilePath
*@return       void
*/
void GTJCollectFileInfo::collectFileInfo(const QString &collectFilePath)
{
    // ѡ����˱�����ļ�����
    QStringList filters;
    filters << "*.dll" << "*.pdb" << "*.GDB" << "*.GSP" << "*.ini"; // ѡ�����ֻ��.dll��.pdb��.GDB�ļ�

    QDir dir(collectFilePath);
    QStringList entrylist = dir.entryList(filters, QDir::Files | QDir::Hidden);

    // ��ÿ���ļ���С��ʱ�䡢MD5��Ϣ����
    foreach(QString file, entrylist)
    {
        outPutCollectInfo(CHAR_FORMAT(L"�����ռ�:") + collectFilePath + "/" + file + "...");
        
        m_mapCollectFileData.insert(std::make_pair(file,
            FileInfoData(
                GTJParseFinleCommon::getFileSize(dir, file),
                GTJParseFinleCommon::getFileTime(dir, file),
                collectFilePath,
                GTJParseFinleCommon::getFileHash(collectFilePath + "/" + file))));
    }
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��27��
*@param        
*@return       void
*/
void GTJCollectFileInfo::onExecuteCollect()
{
    //��ʼ�ռ�����
    onExecute();
}


