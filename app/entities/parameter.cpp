#include "parameter.h"

namespace entities {

db::FieldSet Parameter::toFieldSet() const
{
    db::FieldSet set;
    set.append("id", id);
    set.append("name", name);
    set.append("device_id", deviceId);
    set.append("type_id", typeId);
    return set;
}

Parameter Parameter::fromFieldSet(const db::FieldSet& fieldSet)
{
    Parameter parameter;
    parameter.id = fieldSet.value("id").toInt();
    parameter.name = fieldSet.value("name").toString();
    parameter.deviceId = fieldSet.value("device_id").toInt();
    parameter.typeId = fieldSet.value("type_id").toInt();
    return parameter;
}

QString Parameter::tableName()
{
    return "parameter";
}

} // namespace entities
