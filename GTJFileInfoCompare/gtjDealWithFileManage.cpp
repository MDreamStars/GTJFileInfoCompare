/*!
*@file     gtjDealWithFileManage.cpp
*@brief    �ļ�����
*@author   maozg
*@date     2017��9��27��
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#include "gtjDealWithFileManage.h"
#include <QFileInfo>
#include <QDateTime>
#include <QCryptographicHash>
#include <QXmlStreamWriter>

/*!
*@brief        
*@author       maozg
*@time         2017��9��28��
*@param        const QString& strFilaPath
*@return       QString&
*/
QString GTJParseFinleCommon::getFileName(const QString& strFilaPath)
{
    if (strFilaPath.isEmpty())
    {
        return QString("");
    }
    QFileInfo fileInfo(strFilaPath);

    QString strFileName = fileInfo.fileName();

    return strFileName;

}

/*!
*@brief        ���ļ���С
*@author       maozg
*@time         2017��9��28��
*@param        const QString& strFilaPath
*@param[in]    dir�ļ���·��
*@param[in]    strFilname �ļ��� 
*@return        QString& �ļ���С
*/
QString GTJParseFinleCommon::getFileSize(const QDir &dir, const QString& strFilname)
{
    if (strFilname.isEmpty())
    {
        return QString("");
    }
    QFileInfo fileInfo(dir, strFilname);

    qint64 nFileSize = fileInfo.size();
    QString strFileSize = QString::number(nFileSize);

    return strFileSize;
}

/*!
*@brief        ���ļ����һ�δ����Ĵ�С
*@author       maozg
*@time         2017��9��28��
*@param        const QString& strFilaPath
*@param[in]    dir�ļ���·��
*@param[in]    strFilname �ļ��� 
*@return        QString&
*/
QString GTJParseFinleCommon::getFileTime(const QDir& dir, const QString& strFilaPath)
{
    if (strFilaPath.isEmpty())
    {
        return QString("");
    }
    QFileInfo fileInfo(dir, strFilaPath);

    //����ֻ��ȡ������޸ĵ�ʱ�䣬������ȡ����ʱ��͵��������Ľӿ�
    //created()��ȡ����ʱ��
    //lastRead()��ȡ�������ʱ��
    QDateTime fileTime = fileInfo.lastModified();
    QString strFileTime = fileTime.toString("yyyy-MM-dd hh:mm:ss");

    return strFileTime;
}

/*!
*@brief        
*@author       maozg
*@time         2017��9��28��
*@param        const QString& strFilaPath
*@return        QString&
*/
QString GTJParseFinleCommon::getFileHash(const QString& strFilaPath)
{
    QFile localFile(strFilaPath);
    
    if (!localFile.open(QFile::ReadOnly))  
    {  
        return QString(""); 
    }  

    QCryptographicHash fileHash(QCryptographicHash::Md5);  

    quint64 totalBytes = 0;  
    quint64 bytesWritten = 0;  
    quint64 bytesToWrite = 0;  
    quint64 loadSize = 1024 * 4;  
    QByteArray buf;  

    totalBytes = localFile.size();  
    bytesToWrite = totalBytes;  

    while (true)  
    {  
        if (bytesToWrite > 0)  
        {  
            buf = localFile.read(qMin(bytesToWrite, loadSize));  
            fileHash.addData(buf);  
            bytesWritten += buf.length();  
            bytesToWrite -= buf.length();  
            buf.resize(0);  
        }  
        else  
        {  
            break;  
        }  

        if (bytesWritten == totalBytes)  
        {  
            break;  
        }  
    }  

    localFile.close();  
    QByteArray md5 = fileHash.result();  
    QString strHash = md5.toHex();

    return strHash;  
}

/*!
*@brief   map�е����ݱ��浽XML�ļ���
*@author       caohy
*@time         2017��9��30��
*@param
*@param[in]    �ļ���
*@param[in]
*@return       void
*/
void GTJParseFinleCommon::dataSaveXmlFile(const QString& strSavePath, CollectDataMap& collectData)
{
    if (!checkPath(strSavePath))
    {
        return;
    }

    QFile file(strSavePath);
    if (!file.open(QFile::WriteOnly))
    {
        return;
    }

    // д����
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("FileInfo");

    // ʹ��map����������Ԫ��
    CollectDataMap::iterator it;

    // ��¼�ļ�����
    int Num_file = 1;

    // map���ݱ���Ϊxml��ʽ
    for (it = collectData.begin(); it != collectData.end(); ++it)
    {
        xmlWriter.writeStartElement("file");
        xmlWriter.writeAttribute("file_num", QString::number(Num_file++));
        xmlWriter.writeTextElement("Name", it->first);
        xmlWriter.writeTextElement("Size", it->second.m_strFileSize);
        xmlWriter.writeTextElement("Time", it->second.m_strFileTime);
        xmlWriter.writeTextElement("Dir", it->second.m_strDir);
        xmlWriter.writeTextElement("Hash", it->second.m_strHash);

        xmlWriter.writeEndElement();
    }
    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    file.close();
}

/*!
*@brief
*@author       maozg
*@time         2017��12��21��
*@param        NoneFileDataMap& lostData, CompareResultDataMap& compareResultMap
*@return       void
*/
void GTJParseFinleCommon::saveCompareInfo(const QString& strSavePath, NoneFileDataMap& lostData, CompareResultDataMap& compareResultMap)
{
    if (!checkPath(strSavePath))
    {
        return;
    }

    QFile file(strSavePath);
    if (!file.open(QFile::WriteOnly))
    {
        return;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();

    //д��ʧ���ļ���Ϣ
    {
        xmlWriter.writeStartElement("lostFile");
        for (int i = 0; i < lostData.size(); ++i)
        {
            xmlWriter.writeStartElement("file");
            xmlWriter.writeTextElement("Name", lostData[i]);
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }

    //дMD5��һ�µ��ļ���Ϣ
    {
        xmlWriter.writeStartElement("ErrorFile");
        CompareResultDataMap::iterator itCompare = compareResultMap.begin();
        int nFileNum = 1;
        for (; itCompare != compareResultMap.end(); ++itCompare)
        {
            xmlWriter.writeStartElement("FileInfo");
            xmlWriter.writeAttribute("File_Num", QString::number(nFileNum++));
            {
                xmlWriter.writeStartElement("Sourcefile");
                xmlWriter.writeTextElement("Name", itCompare->first);
                xmlWriter.writeTextElement("Size", itCompare->second.strSrcFileSize);
                xmlWriter.writeTextElement("Time", itCompare->second.strSrcFileTime);
                xmlWriter.writeTextElement("Dir", itCompare->second.strSrcDir);
                xmlWriter.writeTextElement("Hash", itCompare->second.strSrcHash);
                xmlWriter.writeEndElement();

                xmlWriter.writeStartElement("Comparefile");
                xmlWriter.writeTextElement("Name", itCompare->first);
                xmlWriter.writeTextElement("Size", itCompare->second.strComFileSize);
                xmlWriter.writeTextElement("Time", itCompare->second.strComFileTime);
                xmlWriter.writeTextElement("Dir", itCompare->second.strComDir);
                xmlWriter.writeTextElement("Hash", itCompare->second.strComHash);
                xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndDocument();

    file.close();
}
