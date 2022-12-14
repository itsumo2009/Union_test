#include "device.h"

namespace entities {

db::FieldSet Device::toFieldSet() const
{
    db::FieldSet set;
    set.append("id", id);
    set.append("name", name);
    return set;
}

Device Device::fromFieldSet(const db::FieldSet& fieldSet)
{
    Device device;
    device.id = fieldSet.value("id").toInt();
    device.name = fieldSet.value("name").toString();
    return device;
}

QString Device::tableName()
{
    return "device";
}

}
