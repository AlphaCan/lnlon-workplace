#ifndef ETHERCATMASTER_H
#define ETHERCATMASTER_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QTimer>

namespace Ui {
class EthercatMaster;
}

class EthercatMaster : public QMainWindow
{
    Q_OBJECT

public:
    explicit EthercatMaster(QWidget *parent = 0);
    ~EthercatMaster();

private slots:
    void on_pushButton_motorON_clicked();

    void on_pushButton_Exit_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_disconnect_clicked();

    void ShowActualData();

    void on_pushButton_J1UP_pressed();

    void on_pushButton_J2UP_pressed();


private:
    Ui::EthercatMaster *ui;
    bool isrunning;
    QTimer *timer;
};



#endif // ETHERCATMASTER_H
