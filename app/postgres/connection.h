#ifndef POSTGRES_CONNECTION_H
#define POSTGRES_CONNECTION_H

#include "db/iconnection.h"

#include <QSqlDatabase>

namespace postgres {

class Connection : public db::IConnection
{
public:
    explicit Connection(const QSqlDatabase& db);

    // IConnection interface
private:
    std::unique_ptr<db::IQuery> createInsertionQuery(const QString &tableName, const db::FieldSet &fieldSet);
    std::unique_ptr<db::IQuery> createUpdateQuery(const QString &tableName, const db::FieldSet &fieldSet);
    std::unique_ptr<db::IQuery> createSelectQuery(const QString &tableName, const db::FieldSet &fieldSet);
    std::unique_ptr<db::IQuery> createSelectQuery(const QString &tableName, const QString &fieldList);
    std::unique_ptr<db::IQuery> createDeleteQuery(const QString &tableName, const db::FieldSet &fieldSet);

private:
    QSqlDatabase _db;
};

} // namespace postgres

#endif // POSTGRES_CONNECTION_H
