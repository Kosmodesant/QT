#ifndef MEMRISE_H
#define MEMRISE_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class memrise; }
QT_END_NAMESPACE

class memrise : public QMainWindow
{
    Q_OBJECT

public:
    memrise(QWidget *parent = nullptr);
    ~memrise();

private:
    Ui::memrise *ui;
};
#endif // MEMRISE_H
