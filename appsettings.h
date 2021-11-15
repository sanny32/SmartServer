//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <map>
#include <mutex>
#include <QSettings>

struct Setting
{
    const char* Name;
    QVariant DefaultValue;
};

///
/// \brief Настройки приложения
///
class AppSettings
{
public:
    static const Setting SmartCardReader;
    static const Setting SerialPortSettings;
    static const Setting ModbusAddressType;
    static const Setting ModbusStartAddress;
    static const Setting ModbusBufferSize;
    static const Setting ModbusServerAddress;

public:
    static AppSettings* instance();

    QVariant GetSetting(const Setting& setting);
    void SetSetting(const Setting& setting, const QVariant& value);   

private:   
    AppSettings() = default;
    ~AppSettings() = default;

    AppSettings(AppSettings const&) = delete;
    void operator=(AppSettings const&) = delete;

private:
    static AppSettings& getInstance()
    {
        static AppSettings instance;
        return instance;
    }

    QSettings settings;

    std::mutex mutex;      
    std::map<QString, QVariant> variantSettings;   
};

Q_DECLARE_OPAQUE_POINTER(AppSettings*)
Q_DECLARE_METATYPE(AppSettings*)

#endif // APPSETTINGS_H
