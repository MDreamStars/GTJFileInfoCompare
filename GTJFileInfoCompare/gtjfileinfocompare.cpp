/*!
*@file     gtjfileinfocompare.cpp
*@brief    界面操作文件
*@author   maozg
*@date     2017年9月27日
*@remarks
*@version 3.0
*Copyright (c) 1998-2017 Glodon Corporation
*/
#include "gtjfileinfocompare.h"
#include "gtjCollectFileInfo.h"
#include "Common/GTJCommon.h"
#include <QFileDialog>
#include "gtjCompareFileInfo.h"
#include <QMessageBox>
#include <QHeaderView>

GTJFileInfoCompare::GTJFileInfoCompare(QWidget *parent)
    : QWidget(parent)
{
    m_pCollectFileInfo = nullptr;
    m_pCompareInfo = nullptr;
    iniUi();
}

GTJFileInfoCompare::~GTJFileInfoCompare()
{

}

/*!
*@brief        初始化界面信息
*@author       maozg
*@time         2017年9月25日
*@param        
*@return       void
*/
void GTJFileInfoCompare::iniUi()
{
    this->setWindowTitle(CHAR_FORMAT(L"文件信息收集对比工具"));
    QIcon icWindow(":/window/exe/windowICO.png");
    this->setWindowIcon(icWindow);

    m_pTabWidget = new QTabWidget();

    //tab页签1
    QWidget* pCollectWidget = new QWidget();
    QLabel* pLable = new QLabel(CHAR_FORMAT(L"软件安装目录:"));
    m_pFileInfoPath = new QLineEdit();
    m_pCollectBut = new QPushButton(CHAR_FORMAT(L"开始收集"));
    m_pCollectBut->setStyleSheet("background-color: rgb(250, 128, 10);");

    QPushButton* pOpenButton = new QPushButton(CHAR_FORMAT(L"打开"));
    QLabel* pLableOut = new QLabel(CHAR_FORMAT(L"输出:"));
    m_pCollOutPut = new QTextEdit();
    m_pCollOutPut->setReadOnly(true);
    QGridLayout* pLayout = new QGridLayout();
    pLayout->addWidget(pLable, 0, 0);
    pLayout->addWidget(m_pFileInfoPath, 0, 1);
    pLayout->addWidget(pOpenButton, 0, 2);
    pLayout->addWidget(m_pCollectBut, 1, 1, 1, 2);
    pLayout->addWidget(pLableOut, 2, 0);
    pLayout->addWidget(m_pCollOutPut, 2, 1, 1, 2);

    // 设置水平间距
    pLayout->setHorizontalSpacing(10);
    // 设置垂直间距
    pLayout->setVerticalSpacing(10);

    pCollectWidget->setLayout(pLayout);

    QIcon* pFinleInfoIcon = new QIcon(":/collect/fileInfo/fileInfo.png");
    m_pTabWidget->addTab(pCollectWidget, *pFinleInfoIcon, CHAR_FORMAT(L"信息收集"));

    //tab页签2
    QWidget* pCompareWidget = new QWidget();
    QIcon* pCompareIcon = new QIcon(":/compare/fileinfo/compare.png");
    QLabel* pLable2 = new QLabel(CHAR_FORMAT(L"本地收集的结果文件:"));
    QLabel* pLable3 = new QLabel(CHAR_FORMAT(L"用户收集的结果文件:"));

    m_pSourceFilePath = new QLineEdit();
    m_pCompareFilePath = new QLineEdit();

    QPushButton* pOpen2 = new QPushButton(CHAR_FORMAT(L"打开"));
    QPushButton* pOpen3 = new QPushButton(CHAR_FORMAT(L"打开"));
    m_pCompareBut = new QPushButton(CHAR_FORMAT(L"开始对比"));
    m_pCompareBut->setStyleSheet("background-color: rgb(250, 128, 10);");

    QLabel* pOut = new QLabel(CHAR_FORMAT(L"输出:"));
    m_pCompareOutPut = new QTextEdit();
    m_pCompareOutPut->setReadOnly(true);
    QGridLayout* pCompareLayout = new QGridLayout();
    pCompareLayout->addWidget(pLable2, 0, 0);
    pCompareLayout->addWidget(m_pSourceFilePath, 0, 1);
    pCompareLayout->addWidget(pOpen2, 0, 2);
    pCompareLayout->addWidget(pLable3, 1, 0);
    pCompareLayout->addWidget(m_pCompareFilePath, 1, 1);
    pCompareLayout->addWidget(pOpen3, 1, 2);
    pCompareLayout->addWidget(m_pCompareBut, 2, 1, 1, 2);
    pCompareLayout->addWidget(pOut, 3, 0);
    pCompareLayout->addWidget(m_pCompareOutPut, 3, 1, 1, 2);
    pCompareWidget->setLayout(pCompareLayout);

    m_pTabWidget->addTab(pCompareWidget, *pCompareIcon, CHAR_FORMAT(L"信息对比"));

    //建立连接
    connect(pOpenButton, SIGNAL(clicked()), this, SLOT(openCollectFileInfoPath()));
    connect(m_pCollectBut, SIGNAL(clicked()), this, SLOT(runCollectFinleInfo()));
    connect(pOpen2, SIGNAL(clicked()), this, SLOT(openSourceFilePath()));
    connect(pOpen3, SIGNAL(clicked()), this, SLOT(openCompareFilePath()));
    connect(m_pCompareBut, SIGNAL(clicked()), this, SLOT(runCompareFileInfo()));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(m_pTabWidget);
    this->setLayout(layout);

    this->setFixedSize(500, 300);
}

/*!
*@brief        获取收集文件信息的路径
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJFileInfoCompare::openCollectFileInfoPath()
{
    //打开文件路径
    QString strFilePath = QFileDialog::getExistingDirectory(this, CHAR_FORMAT(L"打开文件夹"),".", 0);
    if (!strFilePath.isNull())
    {
        m_pFileInfoPath->setText(strFilePath);
    }
}

/*!
*@brief        开始收集文件信息
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJFileInfoCompare::runCollectFinleInfo()
{
    QString strFileInfoPath = m_pFileInfoPath->text();
    if (strFileInfoPath.isEmpty())
    {
        return;
    }
    m_mapCollectFileData.clear();
    m_pCollectFileInfo = new GTJCollectFileInfo(strFileInfoPath);
    GTJCollectTaskThread* pCollectTask = new GTJCollectTaskThread(m_pCollectFileInfo);

    connect(pCollectTask, &GTJCollectTaskThread::finished, pCollectTask, &QObject::deleteLater);
    connect(m_pCollectFileInfo, SIGNAL(outPutCollectInfo(const QString &)), this, SLOT(outPutCollectInfo(const QString &)));
    connect(m_pCollectFileInfo, SIGNAL(onFinished()), this, SLOT(onCollectFinished()));

    //禁用按钮，避免任务线程没有执行完就又点击按钮
    m_pCollectBut->setEnabled(false);

    pCollectTask->start();
}

/*!
*@brief        获取源文件信息路径
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJFileInfoCompare::openSourceFilePath()
{
    //打开文件路径
    QString strFilePath = QFileDialog::getOpenFileName(this, CHAR_FORMAT(L"打开源文件"),".", 0);
    if (!strFilePath.isNull())
    {
        m_pSourceFilePath->setText(strFilePath);
    }
}

/*!
*@brief        获取对比文件信息路径
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJFileInfoCompare::openCompareFilePath()
{
    //打开文件路径
    QString strFilePath = QFileDialog::getOpenFileName(this, CHAR_FORMAT(L"打开对比文件"),".", 0);
    if (!strFilePath.isNull())
    {
        m_pCompareFilePath->setText(strFilePath);
    }
}

/*!
*@brief        开始对比
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJFileInfoCompare::runCompareFileInfo()
{
    QString strSrcFilePath = m_pSourceFilePath->text();
    QString strComFilePath = m_pCompareFilePath->text();

    if (strSrcFilePath.isEmpty() || strComFilePath.isEmpty())
    {
        return;
    }

    //pCompareInfo 在线程内部释放内存
    m_pCompareInfo = new GTJCompareFileInfo(strSrcFilePath, strComFilePath);
    GTJCompareTask* pTask = new GTJCompareTask(m_pCompareInfo);

    connect(pTask, &GTJCompareTask::finished, pTask, &QObject::deleteLater);
    connect(m_pCompareInfo, SIGNAL(outPutCompareInfo(const QString&)), this,SLOT(outPutCompareInfo(const QString&)));
    connect(m_pCompareInfo, SIGNAL(onFinished(const bool&)), this, SLOT(onCompareFinished(const bool&)));

    //禁用按钮，避免任务线程还没执行完，又点按钮
    m_pCompareBut->setEnabled(false);
    pTask->start();
}

/*!
*@brief        输出处理信息
*@author       maozg
*@time         2017年9月27日
*@param        const QString &strMsg
*@return       void
*/
void GTJFileInfoCompare::outPutCollectInfo(const QString &strMsg)
{
    m_pCollOutPut->append(strMsg);
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月27日
*@param        
*@return       void
*/
void GTJFileInfoCompare::onCollectFinished()
{
    QMessageBox message(QMessageBox::Information, CHAR_FORMAT(L"收集文件信息"), CHAR_FORMAT(L"文件信息收集完成,请保存收集的结果!"));
    QPushButton* pSave = message.addButton(CHAR_FORMAT(L"保存"), QMessageBox::RejectRole);
    connect(pSave, SIGNAL(clicked()), this, SLOT(saveCollectResult()));
    message.exec();

    m_pCollectBut->setEnabled(true);
    delete m_pCollectFileInfo;
    m_pCollectFileInfo = nullptr;
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        const QString &strMsg
*@return       void
*/
void GTJFileInfoCompare::outPutCompareInfo(const QString &strMsg)
{
    m_pCompareOutPut->append(strMsg);
}

/*!
*@brief        
*@author       maozg
*@time         2017年9月28日
*@param        
*@return       void
*/
void GTJFileInfoCompare::onCompareFinished(const bool bError)
{
    if (bError)
    {
        QMessageBox::information(this, CHAR_FORMAT(L"文件信息对比"), CHAR_FORMAT(L"文件信息对比完成,源文件信息与\n对比文件信息对比一致,无错误信息!"),
            CHAR_FORMAT(L"确定"));
    }
    else
    {
        QMessageBox message(QMessageBox::Critical, CHAR_FORMAT(L"文件信息对比"), CHAR_FORMAT(L"文件信息对比完成,源文件信息与对比\n文件信息不一致,请查看错误信息!"));
        QPushButton* pLook = message.addButton(CHAR_FORMAT(L"查看"), QMessageBox::RejectRole);
        connect(pLook, SIGNAL(clicked()), this, SLOT(displayCompareErrorInfo()));
        message.exec();
    }

    m_pCompareBut->setEnabled(true);
    delete m_pCompareInfo;
    m_pCompareInfo = nullptr;
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return       void
*/
void GTJFileInfoCompare::saveCollectResult()
{
    QString strSavePath = QFileDialog::getSaveFileName(this, CHAR_FORMAT(L"保存收集结果"), "", "*.xml");
    if (!strSavePath.isEmpty())
    {
        QFileInfo fileInfo(strSavePath);
        if (!fileInfo.isFile())
        {
            strSavePath += ".xml";
        }

        m_pCollOutPut->append(CHAR_FORMAT(L"开始保存收集的结果..."));
        
        GTJParseFinleCommon::dataSaveXmlFile(strSavePath, m_pCollectFileInfo->getData());
        m_pCollOutPut->append(CHAR_FORMAT(L"保存完成!"));

        QMessageBox::question(this, CHAR_FORMAT(L"文件信息收集"), CHAR_FORMAT(L"文件收集结果保存完成!"),
            CHAR_FORMAT(L"确定"));
    }
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return       void
*/
void GTJFileInfoCompare::displayCompareErrorInfo()
{
    CompareResultList::instance().displayCompareData(m_pCompareInfo->getLostData(), m_pCompareInfo->getInconsistencyData());
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return       CompareResultList
*/
CompareResultList& CompareResultList::instance()
{
    static CompareResultList oResultList;
    return oResultList;
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param        CompareResultDataMap& compareResultMap
*@return       void
*/
void CompareResultList::displayCompareData(NoneFileDataMap& lostData, CompareResultDataMap& compareResultMap)
{
    if (nullptr != m_pTable)
    {
        m_pTable->clear();
        m_lostData.clear();
        m_compareResultMap.clear();

        m_lostData = lostData;
        m_compareResultMap = compareResultMap;

        m_pTable->setRowCount((compareResultMap.size() + lostData.size()));
        m_pTable->setColumnCount(2);
        m_pTable->setColumnWidth(0, 150);
        m_pTable->horizontalHeader()->setStretchLastSection(true);

        QStringList strHead;
        strHead << CHAR_FORMAT(L"文件名") << CHAR_FORMAT(L"错误信息");
        m_pTable->setHorizontalHeaderLabels(strHead);
        int nRow = 0;
        //先设置缺失的文件
        for (size_t i = 0; i < lostData.size(); ++i)
        {
            QTableWidgetItem* pItem = new QTableWidgetItem(lostData[i]);
            QTableWidgetItem* pItem2 = new QTableWidgetItem(CHAR_FORMAT(L"文件缺失"));
            m_pTable->setItem(nRow, 0, pItem);
            m_pTable->setItem(nRow, 1, pItem2);

            nRow++;
        }
        
        CompareResultDataMap::iterator itCompare = compareResultMap.begin();
        for (; itCompare != compareResultMap.end(); ++itCompare)
        {
            QTableWidgetItem* pItem = new QTableWidgetItem(itCompare->first);

            QString strErrorInfo = QString(CHAR_FORMAT(
                L"源文件MD5码:%1\n对比文件MD5码:%2")).arg(itCompare->second.strSrcHash)
                .arg(itCompare->second.strComHash);
            QTableWidgetItem* pItem2 = new QTableWidgetItem(strErrorInfo);
            m_pTable->setItem(nRow, 0, pItem);
            m_pTable->setItem(nRow, 1, pItem2);
            m_pTable->resizeRowToContents(nRow);
            m_pTable->resizeColumnToContents(1);

            nRow++;
        }
    }

    this->show();
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return
*/
CompareResultList::CompareResultList(QWidget *parent /*= 0*/)
    :QWidget(parent)
{
    m_pTable = nullptr;
    iniUi(parent);
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return       void
*/
void CompareResultList::iniUi(QWidget *parent /*= 0*/)
{
    this->setWindowTitle(CHAR_FORMAT(L"对比信息错误列表"));

    m_pTable = new QTableWidget(parent);
    QPushButton* pSaveBt = new QPushButton(CHAR_FORMAT(L"保存数据"));
    QPushButton* pCancelBt = new QPushButton(CHAR_FORMAT(L"取消"));

    QVBoxLayout *pVlay = new QVBoxLayout();
    pVlay->addWidget(m_pTable);
    pVlay->setSpacing(2);

    QHBoxLayout* pHlay = new QHBoxLayout();
    pHlay->setSpacing(10);
    pHlay->addWidget(pSaveBt, 1, Qt::AlignRight);
    pHlay->addWidget(pCancelBt, 0, Qt::AlignRight);

    pVlay->addLayout(pHlay, 1);
    this->setLayout(pVlay);

    this->setMinimumSize(550, 350);

    connect(pSaveBt, SIGNAL(clicked()), this, SLOT(saveData()));
    connect(pCancelBt, SIGNAL(clicked()), this, SLOT(hidList()));
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return       void
*/
void CompareResultList::saveData()
{
    QString strSavePath = QFileDialog::getSaveFileName(this, CHAR_FORMAT(L"保存对比结果"), "", "*.xml");
    if (!strSavePath.isEmpty())
    {
        QFileInfo fileInfo(strSavePath);
        if (!fileInfo.isFile())
        {
            strSavePath += ".xml";
        }

        GTJParseFinleCommon::saveCompareInfo(strSavePath, m_lostData, m_compareResultMap);

        QMessageBox::question(this, CHAR_FORMAT(L"文件信息对比"), CHAR_FORMAT(L"保存成功!"),
            CHAR_FORMAT(L"确定"));
    }
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return       void
*/
void CompareResultList::hidList()
{
    this->hide();
}

/*!
*@brief
*@author       maozg
*@time         2017年12月21日
*@param
*@return
*/
CompareResultList::~CompareResultList()
{

}

