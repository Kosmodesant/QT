#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pb_clearResult->setCheckable(true);

    series = new QLineSeries();
    chart = new QChart(); // Инициализируйте QChart
    chartView = new QChartView(chart); // Инициализируйте QChartView
    chartView->resize(400, 300);
    chartView->show();

    QObject::connect(this, &MainWindow::sig_displayChart, this, &MainWindow::DisplayChart);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete series;
    delete chart; // Удалите QChart
    delete chartView; // Удалите QChartView
}

QVector<uint32_t> MainWindow::ReadFile(QString path, uint8_t numberChannel)
{
    QFile file(path);
    file.open(QIODevice::ReadOnly);

    if (file.isOpen() == false) {

        if (pathToFile.isEmpty()) {
            QMessageBox mb;
            mb.setWindowTitle("Ошибка");
            mb.setText("Ошибка открытия файла");
            mb.exec();
        }
    }
    else {

        // продумайте, как выйти из функции
    }

    QDataStream dataStream;
    dataStream.setDevice(&file);
    dataStream.setByteOrder(QDataStream::LittleEndian);

    QVector<uint32_t> readData;
    readData.clear();
    uint32_t currentWorld = 0, sizeFrame = 0;

    while (dataStream.atEnd() == false) {

        dataStream >> currentWorld;

        if (currentWorld == 0xFFFFFFFF) {

            dataStream >> currentWorld;

            if (currentWorld < 0x80000000) {

                dataStream >> sizeFrame;

                if (sizeFrame > 1500) {
                    continue;
                }

                for (int i = 0; i < sizeFrame / sizeof(uint32_t); i++) {

                    dataStream >> currentWorld;

                    if ((currentWorld >> 24) == numberChannel) {

                        readData.append(currentWorld);

                    }
                }
            }
        }
    }
    ui->chB_readSucces->setChecked(true);
    return readData;
}

QVector<double> MainWindow::ProcessFile(const QVector<uint32_t> dataFile)
{
    QVector<double> resultData;
    resultData.clear();

    foreach (int word, dataFile) {
        word &= 0x00FFFFFF;
        if (word > 0x800000) {
            word -= 0x1000000;
        }

        double res = ((double)word / 6000000) * 10;
        resultData.append(res);
    }
    ui->chB_procFileSucces->setChecked(true);

    return resultData;
}

QVector<double> MainWindow::FindMax(QVector<double> resultData)
{
    double max = 0, sMax = 0;
    // Поиск первого максимума
    foreach (double num, resultData) {
        if (num > max) {
            max = num;
        }
    }

    // Поиск второго максимума
    foreach (double num, resultData) {
        if (num > sMax && (qFuzzyCompare(num, max) == false)) {
            sMax = num;
        }
    }

    QVector<double> maxs = { max, sMax };
    ui->chB_maxSucess->setChecked(true);
    return maxs;
}

QVector<double> MainWindow::FindMin(QVector<double> resultData)
{

    double min = 0, sMin = 0;

    // Поиск первого минимума
    foreach (double num, resultData) {
        if (num < min) {
            min = num;
        }
    }

    // Поиск второго минимума
    foreach (double num, resultData) {
        if (num < sMin && (qFuzzyCompare(num, min) == false)) {
            sMin = num;
        }
    }

    QVector<double> mins = { min, sMin };
    ui->chB_minSucess->setChecked(true);
    return mins;
}

void MainWindow::DisplayResult(QVector<double> mins, QVector<double> maxs)
{
    ui->te_Result->append("Расчет закончен!");

    ui->te_Result->append("Первый минимум " + QString::number(mins.first()));
    ui->te_Result->append("Второй минимум " + QString::number(mins.at(1)));

    ui->te_Result->append("Первый максимум " + QString::number(maxs.first()));
    ui->te_Result->append("Второй максимум " + QString::number(maxs.at(1)));
}

void MainWindow::DisplayChart()
{
    chart->legend()->hide(); // скрыть легенду
    chart->addSeries(series); // добавить серию на график
    chart->createDefaultAxes(); // Создать ось на основе серии, добавленной к диаграмме
    chart->setTitle("Simple line chart"); // Устанавливаем заголовок графика
}

void MainWindow::on_pb_path_clicked()
{
    pathToFile = "";
    ui->le_path->clear();

    pathToFile = QFileDialog::getOpenFileName(this,
        tr("Открыть файл"), "/home/", tr("ADC Files (*.adc)"));

    ui->le_path->setText(pathToFile);
}

void MainWindow::on_pb_start_clicked()
{
     // Очистка графика перед началом новой обработки
    series->clear();
//    chart->removeAllSeries();
    chart->removeSeries(series);
    //проверка на то, что файл выбран
    if (pathToFile.isEmpty()) {

        QMessageBox mb;
        mb.setWindowTitle("Ошибка");
        mb.setText("Выберите файл!");
        mb.exec();

        return;
    }

    ui->chB_maxSucess->setChecked(false);
    ui->chB_procFileSucces->setChecked(false);
    ui->chB_readSucces->setChecked(false);
    ui->chB_minSucess->setChecked(false);

    int selectIndex = ui->cmB_numCh->currentIndex();
    //Маски каналов
    if (selectIndex == 0) {
        numberSelectChannel = 0xEA;
    }
    else if (selectIndex == 1) {
        numberSelectChannel = 0xEF;
    }
    else if (selectIndex == 2) {
        numberSelectChannel = 0xED;
    }

    auto read = [&] { return ReadFile(pathToFile, numberSelectChannel); };
    auto process = [&](QVector<uint32_t> res) { return ProcessFile(res); };
    auto findMax = [&](QVector<double> res) {
        maxs = FindMax(res);
        mins = FindMin(res);
        DisplayResult(mins, maxs);

        /*
         * код наполнения серии
         * и вызов сигнала для отображения графика
         */

        for (int i = 0; i < FD; i++) {
            series->append(i, res[i]);
        }
        emit sig_displayChart();
    };

    auto result = QtConcurrent::run(read)
        .then(process)
        .then(findMax);
}
