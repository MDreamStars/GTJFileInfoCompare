/*!
*@file     gtjCompareFileInfo.cpp
*@brief    对比文件信息
*@author   maozg
*@date     2017年9月27日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#include <QXmlStreamReader>
#include <QFile>
#include <QXmlStreamWriter>

#include "gtjCompareFileInfo.h"
#include "Common/GTJCommon.h"
#include "gtjDealWithFileManage.h"

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        const QString &strSrcFilePath, const QString &strComFilePath
*@return       
*/
GTJCompareFileInfo::GTJCompareFileInfo(const QString &strSrcFilePath, const QString &strComFilePath) 
    : m_strSrcFilePath(strSrcFilePath), m_strComFilePath(strComFilePath), m_bIsError(true)
{

}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        
*@return       
*/
GTJCompareFileInfo::~GTJCompareFileInfo()
{

}

/*!
*@brief        具体任务执行
*@author       maozg
*@time         2017年9月28日
*@param        
*@return       void
*/
void GTJCompareFileInfo::onExecute()
{
    //加载文件信息
    loadFileInfo();
    
    //开始对比文件MD5信息
    compaerFileMD5Info();

    //对比无错误信息
    if (m_loseFileData.size() == 0 &&
        m_compareResultMap.size() == 0)
    {
        outPutCompareInfo(CHAR_FORMAT(L"文件信息一致"));
        return;
    }

    m_bIsError = false;
}

/*!
*@brief        外部接口
*@author       maozg
*@time         2017年9月28日
*@param        
*@return       void
*/
void GTJCompareFileInfo::onExecuteCompare()
{
    outPutCompareInfo(CHAR_FORMAT(L"开始对比文件信息!"));
    onExecute();
    outPutCompareInfo(CHAR_FORMAT(L"对比文件信息完成!"));
    onFinished(m_bIsError);
}

/*!
*@brief   xml格式内容存入到map中     
*@author       caohy
*@time         2017年10月11日
*@param        GTJCompareFileInfo* pCompare, QObject *parent /*= 0*/
/*@return       
*/
void GTJCompareFileInfo::readXmlToMap(const QString &fileName, CollectDataMap& collectMap)
{
    //判断文件是否为空
    QFile file(fileName);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return;
    }

    QXmlStreamReader xmlRead(&file);
    QString strFileName = "";
    QString strFileSize = "";
    QString strFileTime = "";
    QString strFileDir = "";
    QString strFileHash = "";

    while (!xmlRead.atEnd())
    {
        QString strTemp = xmlRead.name().toString();
        
        
        if (xmlRead.isStartElement())
        {
            if ("Name" == xmlRead.name())
            {
                strFileName = xmlRead.readElementText();
            }
            else if ("Size" == xmlRead.name())
            {
                strFileSize = xmlRead.readElementText();
            }
            else if ("Time" == xmlRead.name())
            {
                strFileTime = xmlRead.readElementText();
            }
            else if ("Dir" == xmlRead.name())
            {
                strFileDir = xmlRead.readElementText();
            }
            else if ("Hash" == xmlRead.name())
            {
                strFileHash = xmlRead.readElementText();
            }
        }
        else if (xmlRead.isEndElement() && "file" == xmlRead.name())
        {
            FileInfoData fileInfo(strFileSize, strFileTime, strFileDir, strFileHash);
            collectMap[strFileName] = fileInfo;
        }

        xmlRead.readNext();
    }

    file.close();
}

/*!
*@brief        加载文件信息
*@author       maozg
*@time         2017年10月20日
*@param
*@return       void
*/
void GTJCompareFileInfo::loadFileInfo()
{
    outPutCompareInfo(CHAR_FORMAT(L"加载源文件信息!"));
    //加载数据
    readXmlToMap(m_strSrcFilePath, m_sourceFileDataMap);
    outPutCompareInfo(CHAR_FORMAT(L"加载源文件信息完成!"));

    outPutCompareInfo(CHAR_FORMAT(L"加载对比文件信息!"));
    readXmlToMap(m_strComFilePath, m_compareFileDataMap);
    outPutCompareInfo(CHAR_FORMAT(L"加载对比文件信息完成!"));
}

/*!
*@brief        对比文件的MD5信息
*@author       maozg
*@time         2017年10月20日
*@param
*@return       void
*/
void GTJCompareFileInfo::compaerFileMD5Info()
{
    //以源文件信息为基础
    CollectDataMap::iterator itrSource = m_sourceFileDataMap.begin();
    for (; itrSource != m_sourceFileDataMap.end(); ++itrSource)
    {
        outPutCompareInfo(CHAR_FORMAT(L"正在对比:") + itrSource->first);
        //直接在对比数据中查找该文件，如果没有就属于是缺失的文件
        CollectDataMap::iterator itrCompare = m_compareFileDataMap.find(itrSource->first);
        if (itrCompare != m_compareFileDataMap.end())
        {
            // 找到了对应的文件
            QString strSrcMD5 = itrSource->second.m_strHash;
            QString strComMD5 = itrCompare->second.m_strHash;
            if (strSrcMD5 != strComMD5)
            {
                //匹配不一致
                FileInfoNonSameData fileResult;
                fileResult.strSrcFileSize = itrSource->second.m_strFileSize;
                fileResult.strComFileSize = itrCompare->second.m_strFileSize;
                fileResult.strSrcFileTime = itrSource->second.m_strFileTime;
                fileResult.strComFileTime = itrCompare->second.m_strFileTime;
                fileResult.strSrcDir = itrSource->second.m_strDir;
                fileResult.strComDir = itrCompare->second.m_strDir;
                fileResult.strSrcHash = itrSource->second.m_strHash;
                fileResult.strComHash = itrCompare->second.m_strHash;

                m_compareResultMap[itrSource->first] = fileResult;
            }
        }
        else
        {
            //找不到对应的文件
            //记录丢失的文件信息
            m_loseFileData.push_back(itrSource->first);
        }
    }
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        GTJCompareFileInfo* pCompare, QObject *parent /*= 0*/
/*@return       
*/
GTJCompareTask::GTJCompareTask(GTJCompareFileInfo* pCompare, QObject *parent /*= 0*/)
    : QThread(parent), m_pTask(pCompare)
{
      
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        
*@return       
*/
GTJCompareTask::~GTJCompareTask()
{
    m_pTask = nullptr;
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        
*@return       void
*/
void GTJCompareTask::run()
{
    m_pTask->onExecuteCompare();
}
