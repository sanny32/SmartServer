//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QSize>
#include <QPoint>
#include <QColor>
#include <QCoreApplication>
#include "appsettings.h"

const Setting AppSettings::SmartCardReader =        { "SmartCardReader",          QString()    };
const Setting AppSettings::SerialPortSettings =     { "SerialPortSettings",       QVariant()   };
const Setting AppSettings::ModbusAddressType =      { "ModbusAddressType",        QString()    };
const Setting AppSettings::ModbusStartAddress =     { "ModbusStartAddress",       1            };
const Setting AppSettings::ModbusBufferSize =       { "ModbusBufferSize",         5            };
const Setting AppSettings::ModbusServerAddress =    { "ModbusServerAddress",      1            };

//
/// \brief AppSettings::instance
/// \return
///
AppSettings* AppSettings::instance()
{
    static AppSettings* __ptr(nullptr);
    if (!qApp->property("appsettings").isValid() && !__ptr)
    {
        __ptr = &getInstance(); // Q_GLOBAL_STATIC pointer
        qApp->setProperty("appsettings", QVariant::fromValue(__ptr));
    }
    else if (!__ptr)
    {
        __ptr = qvariant_cast<AppSettings*>(qApp->property("appsettings"));
    }
    else if (!qApp->property("appsettings").isValid())
    {
        qApp->setProperty("appsettings", QVariant::fromValue(__ptr));
    }
    return __ptr;
}


///
/// \brief AppSettings::GetSetting
/// \param setting
/// \return
///
QVariant AppSettings::GetSetting(const Setting& setting)
{
    std::lock_guard<std::mutex> lock(mutex);
    {
        if(variantSettings.find(setting.Name) == variantSettings.end())
        {
            variantSettings[setting.Name] = settings.value(setting.Name, setting.DefaultValue);
        }

        return variantSettings.at(setting.Name);
    }  
}

///
/// \brief AppSettings::SetSetting
/// \param setting
/// \param value
///
void AppSettings::SetSetting(const Setting& setting, const QVariant& value)
{
    std::lock_guard<std::mutex> lock(mutex);
    {
        variantSettings[setting.Name] = value;
        settings.setValue(setting.Name, value);
    }
}
