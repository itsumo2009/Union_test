#include "connection.h"

#include "query.h"

namespace postgres {

Connection::Connection(const QSqlDatabase& db)
    : _db(db)
{

}

std::unique_ptr<db::IQuery> Connection::createInsertionQuery(const QString& tableName, const db::FieldSet& fieldSet)
{
    assert(!fieldSet.fields.isEmpty());

    QString insertionTemplate = "INSERT INTO public.%1(%2) VALUES (%3)";
    QStringList fieldList;
    QStringList valuesPlaceholder;
    for (auto field : fieldSet.fields)
    {
        fieldList.append(field.first);
        valuesPlaceholder.append("?");
    }
    QSqlQuery query(_db);
    query.prepare(insertionTemplate.arg(tableName).arg(fieldList.join(", ")).arg(valuesPlaceholder.join(", ")));
    for (int i=0; i < fieldSet.fields.size(); ++i)
    {
        query.bindValue(i, fieldSet.fields[i].second);
    }

    return std::make_unique<Query>(query);
}

std::unique_ptr<db::IQuery> Connection::createUpdateQuery(const QString& tableName, const db::FieldSet& fieldSet)
{
    assert(!fieldSet.fields.isEmpty());

    QString updateTemplate = "UPDATE public.%1 SET %2 WHERE id=?";
    QStringList fieldList;

    for (int i=1; i < fieldSet.fields.size(); ++i)
    {
        fieldList.append(fieldSet.fields[i].first + "=?");
    }

    QSqlQuery query(_db);
    query.prepare(updateTemplate.arg(tableName).arg(fieldList.join(", ")));

    for (int i=1; i < fieldSet.fields.size(); ++i)
    {
        query.bindValue(i - 1, fieldSet.fields[i].second);
    }

    query.bindValue(fieldSet.fields.size() - 1, fieldSet.fields[0].second);
    return std::make_unique<Query>(query);
}

std::unique_ptr<db::IQuery> Connection::createSelectQuery(const QString& tableName, const db::FieldSet& fieldSet)
{
    QString selectTemplate = ("SELECT %2 FROM public.%1 WHERE %3");
    QSqlQuery query(_db);
    QStringList fieldList;
    QStringList whereList;
    for (int i=0; i < fieldSet.fields.size(); ++i)
    {
        if (fieldSet.fields[i].second.isNull())
        {
            fieldList.append(fieldSet.fields[i].first);
        }
        else
        {
            whereList.append(fieldSet.fields[i].first + "=?");
        }
    }
    assert(!fieldList.isEmpty());
    query.prepare(selectTemplate.arg(tableName).arg(fieldList.join(", ")).arg(whereList.isEmpty() ? "TRUE" : whereList.join(" AND")));
    for (int i=0, j =0; i < fieldSet.fields.size(); ++i)
    {
        if (!fieldSet.fields[i].second.isNull())
        {
            query.bindValue(j++, fieldSet.fields[i].second);
        }
    }
    return std::make_unique<Query>(query);
}

std::unique_ptr<db::IQuery> Connection::createSelectQuery(const QString& tableName, const QString& fieldList)
{
    QString selectTemplate = ("SELECT %2 FROM public.%1");
    QSqlQuery query(_db);

    query.prepare(selectTemplate.arg(tableName).arg(fieldList));
    return std::make_unique<Query>(query);
}

std::unique_ptr<db::IQuery> Connection::createDeleteQuery(const QString& tableName, const db::FieldSet& fieldSet)
{
    assert(!fieldSet.fields.isEmpty());
    QString deleteTemplate ="DELETE FROM public.%1 WHERE %2";
    QSqlQuery query(_db);
    QStringList whereList;
    for (int i=0; i < fieldSet.fields.size(); ++i)
    {
        whereList.append(fieldSet.fields[i].first + "=?");
    }
    query.prepare(deleteTemplate.arg(tableName).arg(whereList.join(" AND")));
    for (int i=0; i < fieldSet.fields.size(); ++i)
    {
        query.bindValue(i, fieldSet.fields[i].second);
    }
    return std::make_unique<Query>(query);
}

} // namespace postgres
