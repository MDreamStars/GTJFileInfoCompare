 /*!
 *@file     gtjCollectFileInfo.cpp
 *@brief    收集文件信息
 *@author   maozg
 *@date     2017年9月27日
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
*@time         2017年9月27日
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
*@time         2017年9月27日
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
*@time         2017年9月27日
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
*@time         2017年9月27日
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
*@time         2017年9月27日
*@param        
*@return       
*/
GTJCollectFileInfo::~GTJCollectFileInfo()
{
    m_mapCollectFileData.clear();
}

/*!
*@brief   遍历整个文件夹，收集dll、GDB、GSP文件     
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJCollectFileInfo::onExecute()
{
    outPutCollectInfo(CHAR_FORMAT(L"开始收集文件信息!"));
    //收集文件信息
    findFileFunc(m_strCollectFilePath);
    outPutCollectInfo(CHAR_FORMAT(L"文件信息收集完成!"));
    onFinished();
}

/*!
*@brief   递归保存给定目录中所有符合要求的文件     
*@author       caohy
*@time         2017年9月29日
*@param        
*@return       void
*/
void GTJCollectFileInfo::findFileFunc(const QString &collectFilePath)
{
    //先收集当前目录
    collectFileInfo(collectFilePath);

    //收集子目录
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
*@brief        收集文件操作
*@author       maozg
*@time         2017年10月20日
*@param        const QString &collectFilePath
*@return       void
*/
void GTJCollectFileInfo::collectFileInfo(const QString &collectFilePath)
{
    // 选择过滤保存的文件类型
    QStringList filters;
    filters << "*.dll" << "*.pdb" << "*.GDB" << "*.GSP" << "*.ini"; // 选择过滤只有.dll、.pdb、.GDB文件

    QDir dir(collectFilePath);
    QStringList entrylist = dir.entryList(filters, QDir::Files | QDir::Hidden);

    // 把每个文件大小、时间、MD5信息保存
    foreach(QString file, entrylist)
    {
        outPutCollectInfo(CHAR_FORMAT(L"正在收集:") + collectFilePath + "/" + file + "...");
        
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
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJCollectFileInfo::onExecuteCollect()
{
    //开始收集任务
    onExecute();
}


