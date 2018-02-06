/*!
*@file     gtjDealWithFileManage.cpp
*@brief    文件管理
*@author   maozg
*@date     2017年9月27日
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
*@time         2017年9月28日
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
*@brief        求文件大小
*@author       maozg
*@time         2017年9月28日
*@param        const QString& strFilaPath
*@param[in]    dir文件的路径
*@param[in]    strFilname 文件名 
*@return        QString& 文件大小
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
*@brief        求文件最后一次创建的大小
*@author       maozg
*@time         2017年9月28日
*@param        const QString& strFilaPath
*@param[in]    dir文件的路径
*@param[in]    strFilname 文件名 
*@return        QString&
*/
QString GTJParseFinleCommon::getFileTime(const QDir& dir, const QString& strFilaPath)
{
    if (strFilaPath.isEmpty())
    {
        return QString("");
    }
    QFileInfo fileInfo(dir, strFilaPath);

    //这里只获取了最近修改的时间，如果想获取其他时间就调用其他的接口
    //created()获取创建时间
    //lastRead()获取最近访问时间
    QDateTime fileTime = fileInfo.lastModified();
    QString strFileTime = fileTime.toString("yyyy-MM-dd hh:mm:ss");

    return strFileTime;
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
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
*@brief   map中的数据保存到XML文件中
*@author       caohy
*@time         2017年9月30日
*@param
*@param[in]    文件名
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

    // 写操作
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("FileInfo");

    // 使用map迭代器遍历元素
    CollectDataMap::iterator it;

    // 记录文件数量
    int Num_file = 1;

    // map数据保存为xml格式
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
*@time         2017年12月21日
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

    //写丢失的文件信息
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

    //写MD5不一致的文件信息
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
