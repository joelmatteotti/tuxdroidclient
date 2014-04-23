/* =============== GPL HEADER =====================
 * mainwindow.h is part of TuxDroidClient
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_btnConnect_clicked();

    void on_btnOpenMouth_clicked();

    void on_btnCloseMouth_clicked();

    void on_btnOpenEyes_clicked();

    void on_btnCloseEyes_clicked();

    void on_btnFlippersUp_clicked();

    void on_btnFlippersDown_clicked();

    void on_btnRotateLeft_clicked();

    void on_btnRotateRight_clicked();

    void on_btnLeftLedOn_clicked();

    void on_btnLeftLedOff_clicked();

    void on_btnLeftLedBlink_clicked();

    void on_btnRightLedOn_clicked();

    void on_btnRightLedOff_clicked();

    void on_btnRightLedBlink_clicked();

    void on_btnBothLedOn_clicked();

    void on_btnBothLedOff_clicked();

    void on_btnBothLedBlink_clicked();

    void on_btnTTSSpeak_clicked();

    void on_btnFlashSnd_clicked();

    void on_btnPlayAtt_clicked();

    void on_btnPlaySnd_clicked();

    void on_btnReset_clicked();

    void on_btnSendCmd_clicked();

    void sendMessageToServer(QString message);

    void readMessageFromServer();

    void onLeftButtonPressed();
    void onLeftButtonReleased();
    void onRightButtonPressed();
    void onRightButtonReleased();
    void onHeadButtonPressed();
    void onHeadButtonReleased();
    void onRemoteButtonPressed(QString button);
    void manageASR(QString asr);
    void onChargerPlugged();
    void onChargerUnPlugged();
    void onDongleConnected();
    void onDongleDisconnected();

    void on_K_1_clicked();
    void on_K_2_clicked();
    void on_K_3_clicked();
    void on_K_4_clicked();
    void on_K_5_clicked();
    void on_K_6_clicked();
    void on_K_7_clicked();
    void on_K_8_clicked();
    void on_K_9_clicked();
    void on_K_0_clicked();

    void on_K_STANDBY_clicked();
    void on_K_MUTE_clicked();
    void on_K_VOLUMEPLUS_clicked();
    void on_K_VOLUMEMINUS_clicked();
    void on_K_ESCAPE_clicked();
    void on_K_YES_clicked();
    void on_K_NO_clicked();
    void on_K_BACKSPACE_clicked();
    void on_K_STARTVOIP_clicked();
    void on_K_RECEIVECALL_clicked();
    void on_K_HANGUP_clicked();
    void on_K_STAR_clicked();
    void on_K_SHARP_clicked();
    void on_K_RED_clicked();
    void on_K_GREEN_clicked();
    void on_K_BLUE_clicked();
    void on_K_YELLOW_clicked();
    void on_K_CHANNELPLUS_clicked();
    void on_K_CHANNELMINUS_clicked();
    void on_K_UP_clicked();
    void on_K_DOWN_click();
    void on_K_LEFT_clicked();
    void on_K_RIGHT_clicked();
    void on_K_OK_clicked();
    void on_K_FASTREWIND_clicked();
    void on_K_FASTFORWARD_clicked();
    void on_K_PLAYPAUSE_clicked();
    void on_K_STOP_clicked();
    void on_K_RECORDING_clicked();
    void on_K_PREVIOUS_clicked();
    void on_K_NEXT_clicked();
    void on_K_MENU_clicked();
    void on_K_MOUSE_clicked();
    void on_K_ALT_clicked();




private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
