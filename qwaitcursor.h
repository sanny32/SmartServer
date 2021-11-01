// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#ifndef QWAITCURSOR_H
#define QWAITCURSOR_H

#endif // QWAITCURSOR_H

#include <QApplication>
#include <QWidget>

class QWaitCursor
{
public:
    QWaitCursor(QWidget* widget = nullptr)
    {
        _widget = widget;

        if(_widget != nullptr)
        {          
            _widget->setCursor(Qt::WaitCursor);
        }
        else
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
        }

        QApplication::processEvents();
    }

    ~QWaitCursor()
    {
       if(_widget != nullptr)
        {
            _widget->unsetCursor();
        }
        else
        {
            QApplication::restoreOverrideCursor();
        }

        QApplication::processEvents();
    }

private:   
    QWidget* _widget;
};
