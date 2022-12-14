#ifndef IQUERY_H
#define IQUERY_H

#include "fieldset.h"

#include <QString>
#include <QVariant>

#include <memory>

namespace db {

class IQuery
{
public:
    virtual ~IQuery() { };
    virtual void execute() = 0;

    virtual std::unique_ptr<FieldSet> getNextResult() = 0;
};

}
#endif // IQUERY_H
