#include "dialog.h"

#include "core/interfaceregistry.h"
#include "postgres/connection.h"

#include <QApplication>
#include <QSqlDatabase>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSqlDatabase db(QSqlDatabase::addDatabase("QPSQL"));
    db.setHostName("localhost");
    db.setPort(5432);
    db.setDatabaseName("devices");
    db.setUserName("postgres");
    if (!db.open())
    {
        std::cout << "Database has not been opened" << std::endl;
        return 1;
    }
    postgres::Connection connection(db);
    core::InterfaceRegistry registry;
    registry.add<db::IConnection, postgres::Connection>([&connection](){ return &connection;});

    Dialog w(registry);
    w.show();
    return a.exec();
}
