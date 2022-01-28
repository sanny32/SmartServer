//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include "smartcarddevice.h"
#include "rtumodbusserver.h"
#include "serialportsettings.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_smartCardDetected(SmartCardInfo smi);
    void on_rtuModbusServerStateChanged(QModbusDevice::State state);
    void on_rtuModbusServerErrorOccurred(QModbusDevice::Error error);
    void on_rtuModbusServerDataWritten(QModbusDataUnit::RegisterType table, int address, int size);
    void on_rtuModbusServerRestarted();
    void on_refreshSmartReaders_clicked();
    void on_startAddress_textEdited(const QString& text);
    void on_bufferSize_textEdited(const QString& text);
    void on_serialPortSettings_clicked();
    void on_serverAddress_valueChanged(int value);
    void on_addressTypeSelector_currentIndexChanged(int index);
    void on_serialPortSelector_currentTextChanged(const QString& text);
    void on_smartReaderSelector_currentTextChanged(const QString& text);
    void on_useSmartReaderEmulator_stateChanged(int state);

private:
    void loadSettings();
    void saveSettings();
    void setupModbusTableWidget();
    void updateSmartReaderSelector();
    void updateSerialPortSelector();
    void updateAddressTypeSelector();
    void createRtuModbusServer();

private:
    Ui::MainWindow *ui;
    SCARDCONTEXT _hContext;

    QMap<QString, SerialPortSettings> _serialPortsSettings;
    const quint8 _dataAlignmnet = 2; // выравнивание данных (размер ID смарт-карты в регистрах)
    std::unique_ptr<SmartCardDevice> _smartCardDevice;
    std::unique_ptr<RtuModbusServer> _rtuModbusServer;
};
#endif // MAINWINDOW_H
