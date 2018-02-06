/*!
*@file     gtjDealWithFileManage.h
*@brief    �ļ�����
*@author   maozg
*@date     2017��9��27��
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJDEALWITHFILEMANAGE_H
#define GTJDEALWITHFILEMANAGE_H

#include <QString>
#include <QDir>
#include "Common\GTJCommon.h"

//�����ļ���Ϣ
//��ȡ�ļ���д�ļ��Ĳ����ڴ�ʵ��
//����ԱȵĽ��

/*
* @brief �����ļ���Ϣ
*
* @author maozg  2017��9��28��
*
* @class GTJParseFinleCommon
*/
class Q_DECL_EXPORT GTJParseFinleCommon 
{
public:
    static QString getFileName(const QString& strFilaPath);
    static QString getFileSize(const QDir& dir, const QString& strFilname);
    static QString getFileTime(const QDir& dir, const QString& strFilname);
    static QString getFileHash(const QString& strFilaPath);
    
    static void dataSaveXmlFile(const QString& strSavePath, CollectDataMap& collectData);
    static void saveCompareInfo(const QString& strSavePath, NoneFileDataMap& lostData, CompareResultDataMap& compareResultMap);
};

#endif