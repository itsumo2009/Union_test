#ifndef DEVICE_H
#define DEVICE_H

#include <QString>

#include "db/fieldset.h"

namespace entities {

struct Device
{
    int id = 0;
    QString name;

    db::FieldSet toFieldSet() const;
    static Device fromFieldSet(const db::FieldSet& fieldSet);
    static QString tableName();
};

}

#endif // DEVICE_H
