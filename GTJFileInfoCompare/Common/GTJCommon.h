/*!
*@file     公共信息
*@brief    
*@author   maozg
*@date     2017年9月25日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJCOMMON_H
#define GTJCOMMON_H
#include <map>
#include <vector>
#include <QString>
#include <QDir>

#define CHAR_FORMAT(s) QString::fromWCharArray(s)
using namespace std;

//文件信息
struct FileInfoData
{
    QString m_strFileSize;
    QString m_strFileTime;
    QString m_strDir;
    QString m_strHash;

    FileInfoData(const QString& strFileSize,
        const QString& strFileTime, const QString& strDir, const QString& strHash)
        : m_strFileSize(strFileSize),
          m_strFileTime(strFileTime),
          m_strDir(strDir),
          m_strHash(strHash)
    {
    }

    FileInfoData()
    {
        m_strFileSize = "";
        m_strFileTime = "";
        m_strDir = "";
        m_strHash = "";
    }
};

//文件信息不一致的存储
struct FileInfoNonSameData
{
    QString strSrcFileSize;
    QString strComFileSize;
    QString strSrcFileTime;
    QString strComFileTime;
    QString strSrcDir;
    QString strComDir;
    QString strSrcHash;
    QString strComHash;

    FileInfoNonSameData()
    {
        strSrcFileSize = "";
        strComFileSize = "";
        strSrcFileTime = "";
        strComFileTime = "";
        strSrcDir = "";
        strComDir = "";
        strSrcHash = "";
        strComHash = "";
    }
};

//此类型用于收集到的文件信息数据，临时存储，最终会输出到文件中
//也用于存储对比时源文件、对比文件的信息
typedef map<QString, FileInfoData> CollectDataMap;
//此类型用于对比后的结果信息，临时存储，对比完成以后，直接输出成文件
typedef map<QString, FileInfoNonSameData> CompareResultDataMap;
//存储缺失的文件名称
typedef vector<QString> NoneFileDataMap;

static bool checkPath(const QString & strFilePath)
{
    QFile outPutDir(strFilePath);
    if (!outPutDir.exists())
    {
        if (!outPutDir.open(QFile::ReadWrite))
        {
            return false;
        }

        outPutDir.close();
    }
    return true;
}

#endif

