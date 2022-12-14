#ifndef POSTGRES_QUERY_H
#define POSTGRES_QUERY_H

#include "db/iquery.h"

#include <QSqlQuery>

namespace postgres {

class Query : public db::IQuery
{
public:
    explicit Query(const QSqlQuery& query);

    // IQuery interface
private:
    void execute();
    std::unique_ptr<db::FieldSet> getNextResult();

private:
    QSqlQuery _query;
};

} // namespace postgres

#endif // POSTGRES_QUERY_H
