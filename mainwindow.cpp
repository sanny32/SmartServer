//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <thread>
#include <QDebug>
#include <QAbstractEventDispatcher>
#include "applogger.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

///
/// \brief MainWindow::MainWindow
/// \param parent
///
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    ,_hContext(0)
{
    ui->setupUi(this);
    AppLogger::getInstance().setup(*qApp, ui->logWidget);

    // событие, генерируемое при бездействии основного GUI-потока приложения
    auto dispatcher = QAbstractEventDispatcher::instance();
    connect(dispatcher, SIGNAL(awake()), this, SLOT(on_awake()));

    const auto retCode = SCardEstablishContext(SCARD_SCOPE_USER, nullptr, nullptr, &_hContext);
    if(retCode != SCARD_S_SUCCESS)
    {
        qCritical().nospace() << "Ошибка инициализации контекста (" << retCode << ")";
    }
    else
    {
        updateSmartReaderSelector();
    }
}

///
/// \brief MainWindow::~MainWindow
///
MainWindow::~MainWindow()
{    
    SCardReleaseContext(_hContext);
    delete ui;
}

///
/// \brief MainWindow::on_awake
///
void MainWindow::on_awake()
{
}

///
/// \brief MainWindow::on_smartReaderSelector_currentTextChanged
/// \param text
///
void MainWindow::on_smartReaderSelector_currentTextChanged(const QString& text)
{
    _smartCardDevice = std::make_unique<SmartCardDevice>(_hContext, text);
    connect(_smartCardDevice.get(), &SmartCardDevice::smartCardDetected, this, &MainWindow::on_smartCardDetected);
}

///
/// \brief MainWindow::on_smartCardDetected
/// \param smi
///
void MainWindow::on_smartCardDetected(SmartCardInfo smi)
{
    qInfo().noquote().nospace() << "Обнаружена смарт-карта [" << smi.id().toString() << "]";
}

///
/// \brief MainWindow::updateSmartReaderSelector
///
void MainWindow::updateSmartReaderSelector()
{
    ui->smartReaderSelector->clear();

    LPTSTR pmszReaders = nullptr;
    unsigned long size = SCARD_AUTOALLOCATE;
    const auto retCode = SCardListReaders(_hContext, nullptr, (LPTSTR)&pmszReaders, &size);

    if(retCode == SCARD_S_SUCCESS)
    {
        LPTSTR pReader = pmszReaders;
        while (pReader != nullptr && '\0' != *pReader)
        {
            const int len = (int)wcslen((wchar_t *)pReader);
            ui->smartReaderSelector->addItem(QString::fromWCharArray(pReader, len));

            pReader += len + 1;
        }

        SCardFreeMemory(_hContext, pmszReaders);
    }
    else
    {
        qCritical().nospace() << "Ошибка получения списка устройств чтения смарт-карт (" << retCode << ")";
    }
}
