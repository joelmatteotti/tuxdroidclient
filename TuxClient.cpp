/* =============== GPL HEADER =====================
 * mainwindow.cpp is part of TuxDroidClient
 * Copyleft (C) 2012 Joel Matteotti <joel _DOT_ matteotti _AT_ free _DOT_ fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 *
 * ====================================================
*/
#include "TuxClient.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

QTcpSocket *tcpSocket;
static bool connected=false;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap img(":/tuxdroid.png");
    ui->imgLbl->setPixmap(img);


    /*
     * Voix disponible
    */
    ui->voice->clear();
    ui->voice->addItem("mb-fr1");
    ui->voice->addItem("mb-fr4");
    ui->voice->addItem("fr");
    ui->voice->addItem("fr-be");
    ui->voice->setCurrentIndex(0);


    /*
     * Valeurs minimales et maximales pour le pitch et le volum
    */
    ui->pitch->setMinimum(0);
    ui->pitch->setMaximum(80);
    ui->pitch->setValue(50);

    ui->speed->setMinimum(80);
    ui->speed->setMaximum(450);
    ui->speed->setValue(115);

    /*
     * ID des sons flash (1 à 17)
    */
    ui->flashSnd->setMinimum(1);
    ui->flashSnd->setMaximum(17);
    ui->flashSnd->setValue(1);

    /*
     * Boutons de la télécommande
    */
    ui->K_0->setFlat(true);
    ui->K_1->setFlat(true);
    ui->K_2->setFlat(true);
    ui->K_3->setFlat(true);
    ui->K_4->setFlat(true);
    ui->K_5->setFlat(true);
    ui->K_6->setFlat(true);
    ui->K_7->setFlat(true);
    ui->K_8->setFlat(true);
    ui->K_9->setFlat(true);

    ui->K_STANDBY->setFlat(true);
    ui->K_MUTE->setFlat(true);
    ui->K_VOLUMEPLUS->setFlat(true);
    ui->K_VOLUMEMINUS->setFlat(true);
    ui->K_ESCAPE->setFlat(true);
    ui->K_YES->setFlat(true);
    ui->K_NO->setFlat(true);
    ui->K_BACKSPACE->setFlat(true);
    ui->K_STARTVOIP->setFlat(true);
    ui->K_RECEIVECALL->setFlat(true);
    ui->K_HANGUP->setFlat(true);
    ui->K_STAR->setFlat(true);
    ui->K_SHARP->setFlat(true);
    ui->K_RED->setFlat(true);
    ui->K_GREEN->setFlat(true);
    ui->K_BLUE->setFlat(true);
    ui->K_YELLOW->setFlat(true);
    ui->K_CHANNELPLUS->setFlat(true);
    ui->K_CHANNELMINUS->setFlat(true);
    ui->K_LEFT->setFlat(true);
    ui->K_RIGHT->setFlat(true);
    ui->K_UP->setFlat(true);
    ui->K_DOWN->setFlat(true);
    ui->K_OK->setFlat(true);
    ui->K_FASTREWIND->setFlat(true);
    ui->K_FASTFORWARD->setFlat(true);
    ui->K_PLAYPAUSE->setFlat(true);
    ui->K_STOP->setFlat(true);
    ui->K_RECORDING->setFlat(true);
    ui->K_PREVIOUS->setFlat(true);
    ui->K_NEXT->setFlat(true);
    ui->K_MENU->setFlat(true);
    ui->K_MOUSE->setFlat(true);
    ui->K_ALT->setFlat(true);

}

MainWindow::~MainWindow()
{
    //Ferme la connexion
    tcpSocket->abort();
    tcpSocket->disconnectFromHost();

    delete tcpSocket;
    delete ui;
}

void MainWindow::sendMessageToServer(QString message)
{
    //envoie le message au server au format UTF-8
    if(connected)
    {
        tcpSocket->write(message.toUtf8()+"\n\r\0");
        tcpSocket->flush();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Vous devez vous connectez au server avant de pouvoir faire ça !");
        msgBox.exec();
    }

}

void MainWindow::on_btnConnect_clicked()
{
    //Préparation de la socket
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessageFromServer()));

    //Tentative de connection au server
    tcpSocket->connectToHost(ui->txtIP->toPlainText(),ui->txtPORT->toPlainText().toInt());

    if(tcpSocket->waitForConnected())
    {
        connected=true;
        //une fois qu'on est connecter on l'indique et on envoie la clef d'identification
        ui->txtStatus->setPlainText("Connecter !");
        QString cmd = "Tux_Key("+ui->txtUKEY->toPlainText()+")";
        tcpSocket->write(cmd.toUtf8());
    }
}

//Fonction appellée lors de l'appuie sur l'aile gauche de Tux
void MainWindow::onLeftButtonPressed()
{
    ui->txtStatus->setPlainText("Appuie sur l'aile gauche");
}

//Fonction appellée lors de la relâche de pression du bouton de l'aile gauche
void MainWindow::onLeftButtonReleased()
{
    ui->txtStatus->setPlainText("Relache de l'aile gauche");
}

//Fonction appellée lors de l'appuie sur l'aile droite de Tux
void MainWindow::onRightButtonPressed()
{
    ui->txtStatus->setPlainText("Appuie sur l'aile droite");
}

//Fonction appellée lors de la relâche de pression du bouton de l'aile droite
void MainWindow::onRightButtonReleased()
{
    ui->txtStatus->setPlainText("Relache de l'aile droite");
}

//Fonction appellée lorsqu'on appuyer sur la tête de Tux
void MainWindow::onHeadButtonPressed()
{
    ui->txtStatus->setPlainText("Appuie sur la tête");
}

//Fonction appellée lors de la relâche de pression du bouton de la tête
void MainWindow::onHeadButtonReleased()
{
    ui->txtStatus->setPlainText("Relache de la tête");
}

//Fonction appellée lors de l'appuie d'un bouton de la télécommande
void MainWindow::onRemoteButtonPressed(QString button)
{
    ui->txtStatus->setPlainText("Bouton de télécommande: "+button);
}

//Gestion du résultat de l'ASR
void MainWindow::manageASR(QString _asr)
{
    QString asr = _asr.replace("é", "e");
    asr = asr.replace("è", "e");

    if (asr.contains("nageoire") || asr.contains("aile"))
    {
        if(asr.contains("haut") || asr.contains("leve"))
            sendMessageToServer("Tux_Flippers(up)");
        if(asr.contains("bas") || asr.contains("baisse"))
            sendMessageToServer("Tux_Flippers(down)");
    }

    if (asr.contains("bouche"))
    {
        if (asr.contains("ouvre") || asr.contains("ouvrir") || asr.contains("ouverture"))
            sendMessageToServer("Tux_Open(Mouth)");
        if (asr.contains("ferme"))
            sendMessageToServer("Tux_Close(Mouth)");
    }

    if (asr.contains("yeux"))
    {
        if (asr.contains("ouvre") || asr.contains("ouvrir") || asr.contains("ouverture"))
            sendMessageToServer("Tux_Open(Eyes)");
        if (asr.contains("ferme"))
            sendMessageToServer("Tux_Close(Eyes)");
        if (asr.contains("clignote"))
            sendMessageToServer("Tux_Leds(both,blink,6,1.0)");
    }
}

//Fonction appellée lorsque le chargeur est branché
void MainWindow::onChargerPlugged()
{
    ui->txtStatus->setPlainText("Chargeur branché");
}

//Fonction appellée lorsque le chargeur est débraanché
void MainWindow::onChargerUnPlugged()
{
    ui->txtStatus->setPlainText("Chargeur débranché");
}

//Fonction appellée lors de la connexion du dongle
void MainWindow::onDongleConnected()
{
    ui->txtStatus->setPlainText("Dongle connecté");
}

//Fonction appellée lors de la déconnexion du dongle
void MainWindow::onDongleDisconnected()
{
    ui->txtStatus->setPlainText("Dongle déconnecté");
}

static bool isRecordingASR; // = false;


//Fonction permettant de lire les messages en provenance du server
void MainWindow::readMessageFromServer()
{
    QString msg = tcpSocket->read(1024); //récupère les données recues depuis le server.
    QStringList qsl;

    if(msg.startsWith("#TUXDROIDSERVER"))
    {
        qsl = msg.split(":");
        if(qsl.at(1) == "VALIDEDKEY\r")
            sendMessageToServer("Tux_SetId(TuxDroidClient_x4s14d4459sdtdc4_d5454)");
    }


    if(msg.startsWith("#TUXBUTTON")) //S'il s'agit d'un bouton
    {
        qsl = msg.split(":");
        if(qsl.at(1) == "LEFT\r")
            onLeftButtonPressed();
        if(qsl.at(1) == "LEFT_RELEASED\r")
            onLeftButtonReleased();
        if(qsl.at(1) == "RIGHT\r")
            onRightButtonPressed();
        if(qsl.at(1) == "RIGHT_RELEASED\r")
            onRightButtonReleased();
        if(qsl.at(1) == "HEAD\r")
            onHeadButtonPressed();
        if(qsl.at(1) == "HEAD_RELEASED\r")
            onHeadButtonReleased();
    }
    else if(msg.startsWith("#TUXREMOTE")) //S'il s'agit de la télécommande
    {
        qsl = msg.split(":");
        onRemoteButtonPressed(qsl.at(1).trimmed());
        //string button  = sp[1].ToString().Trim();

        if (qsl.at(1).trimmed() == "K_RECORDING")
        {
            isRecordingASR = true;
            sendMessageToServer("tux_micro(record,tcasr.wav)");
        }

        if (qsl.at(1).trimmed() == "RELEASE")
        {
            if (isRecordingASR)
            {
                ui->txtStatus->setPlainText("RECORDING ASR");
                sendMessageToServer("tux_micro(stop)");
                sendMessageToServer("tux_flac(tcasr.wav)");
            }
            else
            {
                ui->txtStatus->setPlainText("NOTHING");
            }
        }
    }
    else if (msg.startsWith("#TUXDROIDSERVER:FLACFINISH"))
    {
        if (isRecordingASR)
        {
            sendMessageToServer("tux_asr(tcasr.flac)");
        }
    }
    else if (msg.startsWith("#TUXASRRESULT"))
    {
        isRecordingASR = false;
        qsl = msg.split(":");


        QString asr = qsl.at(1).trimmed();

        ui->txtStatus->setPlainText(asr);

        manageASR(asr);
    }
    else if(msg.startsWith("#TUXCHARGER")) //S'il s'agit du chargeur
    {
        qsl = msg.split(":");

        if(qsl.at(1) == "PLUGGED\r")
            onChargerPlugged();
        if(qsl.at(1) == "UNPLUGGED\r")
            onChargerUnPlugged();
    }
    else if(msg.startsWith("#TUXDONGLE")) //S'il s'agit du dongle
    {
        qsl = msg.split(":");

        if(qsl.at(1) == "CONNECTED\r")
            onDongleConnected();
        if(qsl.at(1) == "DISCONNECTED\r")
            onDongleDisconnected();
    }
    else //S'il s'agit tout autre type de message
    {
        //tout autre type de message
        //surement réponse à une commande comme Tux_State()
        ui->txtStatus->setPlainText(msg.trimmed());
    }
}

void MainWindow::on_btnOpenMouth_clicked()
{
    sendMessageToServer("Tux_Open(Mouth)");
}

void MainWindow::on_btnCloseMouth_clicked()
{
    sendMessageToServer("Tux_Close(Mouth)");
}

void MainWindow::on_btnOpenEyes_clicked()
{
    sendMessageToServer("Tux_Open(Eyes)");
}

void MainWindow::on_btnCloseEyes_clicked()
{
    sendMessageToServer("Tux_Close(Eyes)");
}

void MainWindow::on_btnFlippersUp_clicked()
{
    sendMessageToServer("Tux_Flippers(Up)");
}

void MainWindow::on_btnFlippersDown_clicked()
{
    sendMessageToServer("Tux_Flippers(Down)");
}

void MainWindow::on_btnRotateLeft_clicked()
{
    sendMessageToServer("Tux_Rotate(left,1)");
}

void MainWindow::on_btnRotateRight_clicked()
{
    sendMessageToServer("Tux_Rotate(right,1)");
}

void MainWindow::on_btnLeftLedOn_clicked()
{
    sendMessageToServer("Tux_Leds(Left,On)");
}

void MainWindow::on_btnLeftLedOff_clicked()
{
    sendMessageToServer("Tux_Leds(Left,Off)");
}

void MainWindow::on_btnLeftLedBlink_clicked()
{
    sendMessageToServer("Tux_Leds(left,blink,6,1.0)");
}

void MainWindow::on_btnRightLedOn_clicked()
{
    sendMessageToServer("Tux_Leds(Right,On)");
}

void MainWindow::on_btnRightLedOff_clicked()
{
    sendMessageToServer("Tux_Leds(Right,Off)");
}

void MainWindow::on_btnRightLedBlink_clicked()
{
    sendMessageToServer("Tux_Leds(right,blink,6,1.0)");
}

void MainWindow::on_btnBothLedOn_clicked()
{
    sendMessageToServer("Tux_Leds(Both,On)");
}

void MainWindow::on_btnBothLedOff_clicked()
{
    sendMessageToServer("Tux_Leds(Both,Off)");
}

void MainWindow::on_btnBothLedBlink_clicked()
{
    sendMessageToServer("Tux_Leds(both,blink,6,1.0)");
}

void MainWindow::on_btnTTSSpeak_clicked()
{
    QString ttsVoice = ui->voice->currentText();
    QString ttsPitch = QString::number(ui->pitch->value());
    QString ttsSpeed = QString::number(ui->pitch->value());
    QString ttsMsg = ui->ttsMessage->toPlainText().replace(",",".");

    sendMessageToServer("Tux_TTS(\""+ttsMsg+"\","+ttsVoice+","+ttsPitch+","+ttsSpeed+",1,1)");
    ui->ttsMessage->setPlainText("");
}

void MainWindow::on_btnFlashSnd_clicked()
{
    sendMessageToServer("Tux_Flash("+QString::number(ui->flashSnd->value())+",100)");
}


void MainWindow::on_btnPlayAtt_clicked()
{
    if(!connected)
        return;

    QString file = QFileDialog::getOpenFileName( this, tr( "Sélection d'une attitune" ), "./", tr( "Attitunes (*.att)" ) );
    if( !file.isNull())
        sendMessageToServer("Tux_PlayAatt("+file+")");
}

void MainWindow::on_btnPlaySnd_clicked()
{
    if(!connected)
        return;

    QString file = QFileDialog::getOpenFileName( this, tr( "Sélection d'un fichier audio" ), "./", tr( "Fichiers audio (*.*)" ) );
    if( !file.isNull() )
        sendMessageToServer("Tux_Audio(PlayMusic,"+file+")");
}

void MainWindow::on_btnReset_clicked()
{
    sendMessageToServer("Tux_Reset()");
}

void MainWindow::on_btnSendCmd_clicked()
{
    sendMessageToServer(ui->rawCmd->toPlainText());
    ui->rawCmd->setPlainText("");
}

/* Boutons de la télécommande */

void MainWindow::on_K_1_clicked() { sendMessageToServer("tux_remote(K_1)"); }
void MainWindow::on_K_2_clicked() { sendMessageToServer("tux_remote(K_2)"); }
void MainWindow::on_K_3_clicked() { sendMessageToServer("tux_remote(K_3)"); }
void MainWindow::on_K_4_clicked() { sendMessageToServer("tux_remote(K_4)"); }
void MainWindow::on_K_5_clicked() { sendMessageToServer("tux_remote(K_5)"); }
void MainWindow::on_K_6_clicked() { sendMessageToServer("tux_remote(K_6)"); }
void MainWindow::on_K_7_clicked() { sendMessageToServer("tux_remote(K_7)"); }
void MainWindow::on_K_8_clicked() { sendMessageToServer("tux_remote(K_8)"); }
void MainWindow::on_K_9_clicked() { sendMessageToServer("tux_remote(K_9)"); }
void MainWindow::on_K_0_clicked() { sendMessageToServer("tux_remote(K_0)"); }

void MainWindow::on_K_STANDBY_clicked() { sendMessageToServer("tux_remote(K_STANDBY)"); }
void MainWindow::on_K_MUTE_clicked() { sendMessageToServer("tux_remote(K_MUTE)"); }
void MainWindow::on_K_VOLUMEPLUS_clicked() { sendMessageToServer("tux_remote(K_VOLUMEPLUS)"); }
void MainWindow::on_K_VOLUMEMINUS_clicked() { sendMessageToServer("tux_remote(K_VOLUMEMINUS)"); }
void MainWindow::on_K_ESCAPE_clicked() { sendMessageToServer("tux_remote(K_ESCAPE)"); }
void MainWindow::on_K_YES_clicked() { sendMessageToServer("tux_remote(K_YES)"); }
void MainWindow::on_K_NO_clicked() { sendMessageToServer("tux_remote(K_NO)"); }
void MainWindow::on_K_BACKSPACE_clicked() { sendMessageToServer("tux_remote(K_BACKSPACE)"); }
void MainWindow::on_K_STARTVOIP_clicked() { sendMessageToServer("tux_remote(K_STARTVOIP)"); }
void MainWindow::on_K_RECEIVECALL_clicked() { sendMessageToServer("tux_remote(K_RECEIVECALL)"); }
void MainWindow::on_K_HANGUP_clicked() { sendMessageToServer("tux_remote(K_HANGUP)"); }
void MainWindow::on_K_STAR_clicked() { sendMessageToServer("tux_remote(K_STAR)"); }
void MainWindow::on_K_SHARP_clicked() { sendMessageToServer("tux_remote(K_SHARP)"); }
void MainWindow::on_K_RED_clicked() { sendMessageToServer("tux_remote(K_RED)"); }
void MainWindow::on_K_GREEN_clicked() { sendMessageToServer("tux_remote(K_GREEN)"); }
void MainWindow::on_K_BLUE_clicked() { sendMessageToServer("tux_remote(K_BLUE)"); }
void MainWindow::on_K_YELLOW_clicked() { sendMessageToServer("tux_remote(K_YELLOW)"); }
void MainWindow::on_K_CHANNELPLUS_clicked() { sendMessageToServer("tux_remote(K_CHANNELPLUS)"); }
void MainWindow::on_K_CHANNELMINUS_clicked() { sendMessageToServer("tux_remote(K_CHANNELMINUS)"); }
void MainWindow::on_K_UP_clicked() { sendMessageToServer("tux_remote(K_UP)"); }
void MainWindow::on_K_DOWN_click() { sendMessageToServer("tux_remote(K_DOWN)"); }
void MainWindow::on_K_LEFT_clicked() { sendMessageToServer("tux_remote(K_LEFT)"); }
void MainWindow::on_K_RIGHT_clicked() { sendMessageToServer("tux_remote(K_RIGHT)"); }
void MainWindow::on_K_OK_clicked() { sendMessageToServer("tux_remote(K_OK)"); }
void MainWindow::on_K_FASTREWIND_clicked() { sendMessageToServer("tux_remote(K_FASTREWING)"); }
void MainWindow::on_K_FASTFORWARD_clicked() { sendMessageToServer("tux_remote(K_FASTFORWARD)"); }
void MainWindow::on_K_PLAYPAUSE_clicked() { sendMessageToServer("tux_remote(K_PLAYPAUSE)"); }
void MainWindow::on_K_STOP_clicked() { sendMessageToServer("tux_remote(K_STOP)"); }
void MainWindow::on_K_RECORDING_clicked() { sendMessageToServer("tux_remote(K_RECORDING)"); }
void MainWindow::on_K_PREVIOUS_clicked() { sendMessageToServer("tux_remote(K_PREVIOUS)"); }
void MainWindow::on_K_NEXT_clicked() { sendMessageToServer("tux_remote(K_NEXT)"); }
void MainWindow::on_K_MENU_clicked() { sendMessageToServer("tux_remote(K_MENU)"); }
void MainWindow::on_K_MOUSE_clicked() { sendMessageToServer("tux_remote(K_MOUSE)"); }
void MainWindow::on_K_ALT_clicked() { sendMessageToServer("tux_remote(K_ALT)"); }
