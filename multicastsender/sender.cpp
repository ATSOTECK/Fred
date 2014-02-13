/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtNetwork>
#include <QClipboard>
#include <QtCore>
#include "sender.h"
#include "mouse_conversions.h"

Sender::Sender(QWidget *parent)
    : QDialog(parent)
{
    x=y=z=0;
    groupAddress = QHostAddress("192.168.1.101");
    setFocusPolicy(Qt::StrongFocus);
    statusLabel = new QLabel(tr("Ready to multicast datagrams to group %1 on port 45454").arg(groupAddress.toString()));

    ttlLabel = new QLabel(tr("TTL for multicast datagrams:"));
    ttlSpinBox = new QSpinBox;
    ttlSpinBox->setRange(0, 179);

    QHBoxLayout *ttlLayout = new QHBoxLayout;
    ttlLayout->addWidget(ttlLabel);
    ttlLayout->addWidget(ttlSpinBox);

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));
    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setEnabled(false);

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(pauseButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,  QDialogButtonBox::RejectRole);

    timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    messageNo = 1;

    connect(ttlSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ttlChanged(int)));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startSending()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(timer, SIGNAL(timeout()), this, SLOT(sendDatagram()));
    connect(pauseButton,SIGNAL(clicked()),this,SLOT(pause()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(ttlLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Multicast Sender"));
    ttlSpinBox->setValue(1);
}

void Sender::ttlChanged(int newTtl)
{
    //udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, newTtl);
    messageNo = newTtl;
}

void Sender::startSending()
{
    startButton->setEnabled(false);
    pauseButton->setEnabled(true);
    timer->start(340);
}
void Sender::pause()
{
    timer->stop();
    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
}

void Sender::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W) {
        x = 350;
    }
    if(event->key() == Qt::Key_A) {
        y = -350;
    }
    if(event->key() == Qt::Key_S) {
        x = -350;
    }
    if(event->key() == Qt::Key_D) {
        y = 350;
    }
    if(event->key() == Qt::Key_Q) {
        z = 350;
    }
    if(event->key() == Qt::Key_E) {
       z = -350;

    }
    if(event->key() == Qt::Key_X) {
       x = y = z = 0;
    }
    QWidget::keyPressEvent(event);
}



void Sender::sendDatagram()
{

    QClipboard *cb = QApplication::clipboard();
    //hapens here

    //QString motorEncoding = convertMouseHTTPtoMotorHTTP(cb->text());
    QString motorEncoding = convertMouseHTTPtoMotorHTTP( "?TX=" + QString::number(x) + "&TY="+QString::number(y)+"&TZ="+QString::number(z)+"&RX=0&RY=0&RZ=0" );
    qDebug() << "?TX=" + QString::number(x) + "&TY="+QString::number(y)+"&TZ="+QString::number(z);
    statusLabel->setText(motorEncoding);
    QByteArray datagram = motorEncoding.toUtf8().data();
    udpSocket->writeDatagram(datagram.data(), datagram.size(),
                             groupAddress, 45454);

    qDebug() << "sent";

}
