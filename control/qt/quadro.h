#ifndef QUADRO_H
#define QUADRO_H

#include <QMainWindow>
#include <serial.h>
#include <string>
#include <fstream>
#include <QTimer>
#include <vect.h>
#include <quadrocopter.h>
#include <joystick.h>

using std::string;
using std::ofstream;

namespace Ui {
    class Quadro;
}

class Quadro : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Quadro(QWidget *parent = 0);
    ~Quadro();
    
private slots:
    void timer_auto_update();
    void timer_reconnect_update();

    void on_button_connect_clicked();
    void on_button_disconnect_clicked();
    void on_reset_clicked();

    void on_button_auto_clicked();

    void on_button_joystick_connect_clicked();
    void on_button_joystick_disconnect_clicked();
    void on_joystick_use_clicked();
    void on_button_joystick_read_defaults_clicked();

    void on_save_toggled();

private:
    Ui::Quadro *ui;

    static const double timer_auto_interval = 50;
    static const double timer_reconnect_interval = 1000;
    QTimer timer_auto, timer_reconnect;

    bool quadro_reconnect, joy_reconnect;

    bool joystick_use, mode_auto, save;

    quadrocopter quadro;
    joystick joy;
    double power_interface;

    string save_filename;
    ofstream save_file;

    void quadro_connect();
    void quadro_disconnect();
    void joy_disconnect();
    void joy_connect();

    void set_quadro_data();
    void set_auto(bool);

    void save_data();
    void save_open();
    void save_close();

    void interface_read();
    void interface_write();
};

#endif // QUADRO_H
