/*!
*@file     ������Ϣ
*@brief    
*@author   maozg
*@date     2017��9��25��
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

//�ļ���Ϣ
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

//�ļ���Ϣ��һ�µĴ洢
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

//�����������ռ������ļ���Ϣ���ݣ���ʱ�洢�����ջ�������ļ���
//Ҳ���ڴ洢�Ա�ʱԴ�ļ����Ա��ļ�����Ϣ
typedef map<QString, FileInfoData> CollectDataMap;
//���������ڶԱȺ�Ľ����Ϣ����ʱ�洢���Ա�����Ժ�ֱ��������ļ�
typedef map<QString, FileInfoNonSameData> CompareResultDataMap;
//�洢ȱʧ���ļ�����
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

