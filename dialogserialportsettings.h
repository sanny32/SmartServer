//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef DIALOGSERIALPORTSETTINGS_H
#define DIALOGSERIALPORTSETTINGS_H

#include <QDialog>
#include "serialportsettings.h"

namespace Ui {
class DialogSerialPortSettings;
}

class DialogSerialPortSettings : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSerialPortSettings(SerialPortSettings& settings, QWidget *parent = nullptr);
    ~DialogSerialPortSettings();

    void accept() override;

private:
    Ui::DialogSerialPortSettings *ui;
    SerialPortSettings& _settings;
};

#endif // DIALOGSERIALPORTSETTINGS_H
