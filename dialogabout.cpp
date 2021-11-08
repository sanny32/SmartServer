//
// SmartServer
// Copyright (C) 2021 АО "Нефтеавтоматика"
//
// Разработчик
// Ананьев А.А. <Ananev-AA@nefteavtomatika.ru>
//

#include <QPushButton>
#include "dialogabout.h"
#include "ui_dialogabout.h"

///
/// \brief DialogAbout::DialogAbout
/// \param parent
///
DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog |
                   Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint);
    setFixedSize(size());

    ui->version->setText(QString("%1 build %2").arg(APP_VERSION, APP_BUILD));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("ОК");    
}

///
/// \brief DialogAbout::~DialogAbout
///
DialogAbout::~DialogAbout()
{
    delete ui;
}
