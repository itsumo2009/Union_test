#include "query.h"

#include <QSqlRecord>

namespace postgres {

Query::Query(const QSqlQuery& query)
    : _query(query)
{

}

void Query::execute()
{
    _query.exec();
}

std::unique_ptr<db::FieldSet> Query::getNextResult()
{
    std::unique_ptr<db::FieldSet> result;
    if (_query.next())
    {
        result = std::make_unique<db::FieldSet>();
        for (int i = 0; i < _query.record().count(); ++i)
        {
            result->append(_query.record().fieldName(i), _query.record().value(i));
        }
    }
    return result;
}

} // namespace postgres
