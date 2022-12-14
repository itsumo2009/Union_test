#ifndef ENTITIES_PARAMETER_H
#define ENTITIES_PARAMETER_H


#include <QString>

#include "db/fieldset.h"

namespace entities {

struct Parameter
{
    int id = 0;
    QString name;
    int typeId = 0;
    int deviceId = 0;

    db::FieldSet toFieldSet() const;
    static Parameter fromFieldSet(const db::FieldSet& fieldSet);
    static QString tableName();
};

} // namespace entities

#endif // ENTITIES_PARAMETER_H
