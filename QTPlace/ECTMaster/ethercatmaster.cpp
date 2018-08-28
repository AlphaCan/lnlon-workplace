#include "ethercatmaster.h"
#include "ui_ethercatmaster.h"
#include "simple_test.h"

extern timer_t timerid;
extern boolean inOP;
extern uint8_t IOmap[4096];

EthercatMaster::EthercatMaster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EthercatMaster)
{
    ui->setupUi(this);
    isrunning = false;
    ui->pushButton_motorON->setStyleSheet("background-color: rgb(0, 255, 227)");
    ui->pushButton_J1DOWN->setEnabled(false);
    ui->pushButton_J1UP->setEnabled(false);
    ui->pushButton_J2UP->setEnabled(false);
    ui->pushButton_J2DOWN->setEnabled(false);
    ui->pushButton_J3DOWN->setEnabled(false);
    ui->pushButton_J3UP->setEnabled(false);
    ui->pushButton_J4UP->setEnabled(false);
    ui->pushButton_J4DOWN->setEnabled(false);
    ui->pushButton_motorON->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(false);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(ShowActualData()));

}

EthercatMaster::~EthercatMaster()
{
    delete ui;
}

void EthercatMaster::on_pushButton_motorON_clicked()
{
    if(isrunning)
    {

        ui->pushButton_motorON->setText("Servo-ON");
        servo_off();
        ui->pushButton_motorON->setStyleSheet("background-color: rgb(0, 255, 227)");
        isrunning = false;
    }
    else if(!isrunning)
    {

        ui->pushButton_motorON->setText("Servo-OFF");
//        ui->label_9->setText(QString::number(i,10));
        servo_on();
        ui->pushButton_motorON->setStyleSheet("background-color: rgb(0, 255, 13)");
        isrunning = true;
    }

}

void EthercatMaster::on_pushButton_Exit_clicked()
{
    if(!(QMessageBox::information(this,tr("EtherCAT Master"),tr("Are you sure quit this Application?"),tr("YES"),tr("NO"))))
    {
        QApplication* app;
        app->quit();
    }

}

void EthercatMaster::on_pushButton_Connect_clicked()
{
    mainFUN();
    ui->pushButton_Connect->setEnabled(false);
    ui->pushButton_J1DOWN->setEnabled(true);
    ui->pushButton_J1UP->setEnabled(true);
    ui->pushButton_J2UP->setEnabled(true);
    ui->pushButton_J2DOWN->setEnabled(true);
    ui->pushButton_J3DOWN->setEnabled(true);
    ui->pushButton_J3UP->setEnabled(true);
    ui->pushButton_J4UP->setEnabled(true);
    ui->pushButton_J4DOWN->setEnabled(true);
    ui->pushButton_motorON->setEnabled(true);
    ui->pushButton_disconnect->setEnabled(true);
    timer->start(100);
}

void EthercatMaster::on_pushButton_disconnect_clicked()
{
    printf("detele timer \n");
    timer_delete(timerid);
//    exit(EXIT_SUCCESS);
    printf("\nRequest init state for all slaves\n");
    ec_slave[0].state = EC_STATE_INIT;
    /* request INIT state for all slaves */
    ec_writestate(0);
    printf("End simple test, close socket\n");
    /* stop SOEM, close socket */
    ec_close();
    printf("End program\n");
    ui->pushButton_Connect->setEnabled(true);
    ui->pushButton_J1DOWN->setEnabled(false);
    ui->pushButton_J1UP->setEnabled(false);
    ui->pushButton_J2UP->setEnabled(false);
    ui->pushButton_J2DOWN->setEnabled(false);
    ui->pushButton_J3DOWN->setEnabled(false);
    ui->pushButton_J3UP->setEnabled(false);
    ui->pushButton_J4UP->setEnabled(false);
    ui->pushButton_J4DOWN->setEnabled(false);
    ui->pushButton_motorON->setEnabled(false);
    ui->pushButton_disconnect->setEnabled(false);
}

void EthercatMaster:: ShowActualData()
{
    ui->lineEdit_J1keywords->setText(QString::number(get_status_words(0),16));
    ui->lineEdit_J1position->setText(QString::number(get_position(0),10));
    ui->lineEdit_J1velocity->setText(QString::number(get_velocity(0),10));
    ui->lineEdit_J1torque->setText(QString::number(get_torque(0),10));

    ui->lineEdit_J2keywords->setText(QString::number(get_status_words(1),16));
    ui->lineEdit_J2position->setText(QString::number(get_position(1),10));
    ui->lineEdit_J2velocity->setText(QString::number(get_velocity(1),10));
    ui->lineEdit_J2torque->setText(QString::number(get_torque(1),10));

    ui->lineEdit_J3keywords->setText(QString::number(get_status_words(2),16));
    ui->lineEdit_J3position->setText(QString::number(get_position(2),10));
    ui->lineEdit_J3velocity->setText(QString::number(get_velocity(2),10));
    ui->lineEdit_J3torque->setText(QString::number(get_torque(2),10));

    ui->lineEdit_J4keywords->setText(QString::number(get_status_words(3),16));
    ui->lineEdit_J4position->setText(QString::number(get_position(3),10));
    ui->lineEdit_J4velocity->setText(QString::number(get_velocity(3),10));
    ui->lineEdit_J4torque->setText(QString::number(get_torque(3),10));
}

void EthercatMaster::on_pushButton_J1UP_pressed()
{


}

void EthercatMaster::on_pushButton_J2UP_pressed()
{
    set_position(1,500);
}
