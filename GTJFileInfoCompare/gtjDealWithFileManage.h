/*!
*@file     gtjDealWithFileManage.h
*@brief    文件管理
*@author   maozg
*@date     2017年9月27日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#ifndef GTJDEALWITHFILEMANAGE_H
#define GTJDEALWITHFILEMANAGE_H

#include <QString>
#include <QDir>
#include "Common\GTJCommon.h"

//加载文件信息
//读取文件、写文件的操作在此实现
//输出对比的结果

/*
* @brief 解析文件信息
*
* @author maozg  2017年9月28日
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