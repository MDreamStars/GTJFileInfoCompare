/*!
*@file     gtjCompareFileInfo.cpp
*@brief    �Ա��ļ���Ϣ
*@author   maozg
*@date     2017��9��27��
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
*@time         2017��9��28��
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
*@time         2017��9��28��
*@param        
*@return       
*/
GTJCompareFileInfo::~GTJCompareFileInfo()
{

}

/*!
*@brief        ��������ִ��
*@author       maozg
*@time         2017��9��28��
*@param        
*@return       void
*/
void GTJCompareFileInfo::onExecute()
{
    //�����ļ���Ϣ
    loadFileInfo();
    
    //��ʼ�Ա��ļ�MD5��Ϣ
    compaerFileMD5Info();

    //�Ա��޴�����Ϣ
    if (m_loseFileData.size() == 0 &&
        m_compareResultMap.size() == 0)
    {
        outPutCompareInfo(CHAR_FORMAT(L"�ļ���Ϣһ��"));
        return;
    }

    m_bIsError = false;
}

/*!
*@brief        �ⲿ�ӿ�
*@author       maozg
*@time         2017��9��28��
*@param        
*@return       void
*/
void GTJCompareFileInfo::onExecuteCompare()
{
    outPutCompareInfo(CHAR_FORMAT(L"��ʼ�Ա��ļ���Ϣ!"));
    onExecute();
    outPutCompareInfo(CHAR_FORMAT(L"�Ա��ļ���Ϣ���!"));
    onFinished(m_bIsError);
}

/*!
*@brief   xml��ʽ���ݴ��뵽map��     
*@author       caohy
*@time         2017��10��11��
*@param        GTJCompareFileInfo* pCompare, QObject *parent /*= 0*/
/*@return       
*/
void GTJCompareFileInfo::readXmlToMap(const QString &fileName, CollectDataMap& collectMap)
{
    //�ж��ļ��Ƿ�Ϊ��
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
*@brief        �����ļ���Ϣ
*@author       maozg
*@time         2017��10��20��
*@param
*@return       void
*/
void GTJCompareFileInfo::loadFileInfo()
{
    outPutCompareInfo(CHAR_FORMAT(L"����Դ�ļ���Ϣ!"));
    //��������
    readXmlToMap(m_strSrcFilePath, m_sourceFileDataMap);
    outPutCompareInfo(CHAR_FORMAT(L"����Դ�ļ���Ϣ���!"));

    outPutCompareInfo(CHAR_FORMAT(L"���ضԱ��ļ���Ϣ!"));
    readXmlToMap(m_strComFilePath, m_compareFileDataMap);
    outPutCompareInfo(CHAR_FORMAT(L"���ضԱ��ļ���Ϣ���!"));
}

/*!
*@brief        �Ա��ļ���MD5��Ϣ
*@author       maozg
*@time         2017��10��20��
*@param
*@return       void
*/
void GTJCompareFileInfo::compaerFileMD5Info()
{
    //��Դ�ļ���ϢΪ����
    CollectDataMap::iterator itrSource = m_sourceFileDataMap.begin();
    for (; itrSource != m_sourceFileDataMap.end(); ++itrSource)
    {
        outPutCompareInfo(CHAR_FORMAT(L"���ڶԱ�:") + itrSource->first);
        //ֱ���ڶԱ������в��Ҹ��ļ������û�о�������ȱʧ���ļ�
        CollectDataMap::iterator itrCompare = m_compareFileDataMap.find(itrSource->first);
        if (itrCompare != m_compareFileDataMap.end())
        {
            // �ҵ��˶�Ӧ���ļ�
            QString strSrcMD5 = itrSource->second.m_strHash;
            QString strComMD5 = itrCompare->second.m_strHash;
            if (strSrcMD5 != strComMD5)
            {
                //ƥ�䲻һ��
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
            //�Ҳ�����Ӧ���ļ�
            //��¼��ʧ���ļ���Ϣ
            m_loseFileData.push_back(itrSource->first);
        }
    }
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��28��
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
*@time         2017��9��28��
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
*@time         2017��9��28��
*@param        
*@return       void
*/
void GTJCompareTask::run()
{
    m_pTask->onExecuteCompare();
}
