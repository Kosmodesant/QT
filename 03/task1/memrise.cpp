#include "memrise.h"
#include "./ui_memrise.h"

memrise::memrise(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::memrise)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

