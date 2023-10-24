#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setCheckable(true);
    ui->imageLabel->raise();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int currentValue = ui->progressBar->value();
    if (currentValue + 10 <= 100) {
        ui->progressBar->setValue(currentValue + 10);
    } else {
        ui->progressBar->setValue(0);
    }
}

