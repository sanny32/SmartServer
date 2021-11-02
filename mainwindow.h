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

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_awake();
    void on_smartCardDetected(SmartCardInfo smi);
    void on_refreshSmartReaders_clicked();
    void on_smartReaderSelector_currentTextChanged(const QString& text);

private:
    void updateSmartReaderSelector();

private:
    Ui::MainWindow *ui;
    SCARDCONTEXT _hContext;
    std::unique_ptr<SmartCardDevice> _smartCardDevice;
};
#endif // MAINWINDOW_H
