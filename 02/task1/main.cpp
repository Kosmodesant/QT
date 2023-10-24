#include <QCoreApplication>
#include <QNetworkRequest>
#include <QtSql>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QNetworkRequest request;
    request.setUrl(QUrl("https://github.com/Kosmodesant"));

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("Localhost");
    db.setDatabaseName("DBANAME");
    db.setUserName("USERNAME");
    db.setPassword("PASSWORD");
    bool ok = db.open();

    return a.exec();
}
