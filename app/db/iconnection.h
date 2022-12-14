#ifndef DB_ICONNECTION_H
#define DB_ICONNECTION_H

#include "iquery.h"

#include <memory>


namespace db {

class IConnection
{
public:
    virtual ~IConnection() { }

    virtual std::unique_ptr<IQuery> createInsertionQuery(const QString& tableName, const FieldSet& fieldSet) = 0;
    virtual std::unique_ptr<IQuery> createUpdateQuery(const QString& tableName, const FieldSet& fieldSet) = 0;
    virtual std::unique_ptr<IQuery> createSelectQuery(const QString& tableName, const FieldSet& fieldSet) = 0;
    virtual std::unique_ptr<IQuery> createSelectQuery(const QString& tableName, const QString& fieldList) = 0;
    virtual std::unique_ptr<IQuery> createDeleteQuery(const QString& tableName, const FieldSet& fieldSet) = 0;
};

} // namespace db

#endif // DB_ICONNECTION_H
