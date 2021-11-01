//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QDebug>
#include <thread>
#include "smartcarddevice.h"
#include "smartcarderror.h"
#include "scc_getid.h"

///
/// \brief SmartCardDevice::SmartCardDevice
/// \param name
/// \param parent
///
SmartCardDevice::SmartCardDevice(SCARDCONTEXT hContext, const QString& name, QObject *parent)
    : QObject(parent)
    ,_hCard(0)
    ,_hContext(hContext)
    ,_actProtocol(0)
    ,_name(name)
    ,_destroy(false)
{       
    std::thread([&](std::wstring name)
    {
        do
        {
            const auto retCode = SCardConnect(_hContext, name.c_str(), SCARD_SHARE_DIRECT, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &_hCard, &_actProtocol);
            if(retCode == SCARD_S_SUCCESS)
            {
                const auto smi = readSmartCardInfo();
                SCardDisconnect(_hCard, SCARD_LEAVE_CARD);

                if(smi.isValid())
                {
                    emit smartCardDetected(smi);
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        while(!_destroy);
    }, _name.toStdWString()).detach();
}

///
/// \brief SmartCardDevice::~SmartCardDevice
///
SmartCardDevice::~SmartCardDevice()
{
    _destroy = true;
}

///
/// \brief SmartCardDevice::readSmartCardInfo
/// \return
///
SmartCardInfo SmartCardDevice::readSmartCardInfo()
{
    SmartCardInfo smi;

    try
    {
        SCC_GetId sc;
        const auto cardId = sc.send(_hCard, _actProtocol);

        smi.setId(cardId);
    }
    catch(std::exception& ex)
    {
        qWarning() << ex.what();
    }

    return smi;
}
