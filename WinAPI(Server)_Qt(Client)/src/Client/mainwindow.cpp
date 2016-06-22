#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "localsocketipcclient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
   LocalSocketIpcClient* lp = new LocalSocketIpcClient("\\\\.\\pipe\\my_pipe_name",this);
   lp->send_MessageToServer(ui->plainTextEdit->toPlainText());
}
