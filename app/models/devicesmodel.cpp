#include "devicesmodel.h"

namespace models {

DevicesModel::DevicesModel(db::IConnection& connection)
    : _connection(connection)
{
    auto query = _connection.createSelectQuery(entities::Device::tableName(), "id, name");
    query->execute();
    while (auto result = query->getNextResult())
    {
        _devices.push_back(entities::Device::fromFieldSet(*result));
    }
}

void DevicesModel::addDevice()
{
    const QString name = "<Новое устройство>";
    db::FieldSet set;
    set.append("name", name);
    auto query = _connection.createInsertionQuery(entities::Device::tableName(), set);
    query->execute();
    query =_connection.createSelectQuery(entities::Device::tableName(), "max(id) AS id");
    query->execute();

    if (auto result = query->getNextResult())
    {
        beginInsertRows({}, _devices.size(), _devices.size());
        _devices.push_back(entities::Device::fromFieldSet(*result));
        _devices.back().name = name;
        endInsertRows();
    };
}

void DevicesModel::deleteDevice(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto id = _devices[index.row()].id;
        db::FieldSet set;
        set.append("id", id);
        auto query = _connection.createDeleteQuery(entities::Device::tableName(), set);
        query->execute();

        beginRemoveRows({}, index.row(), index.row());
        _devices.remove(index.row());
        endRemoveRows();
    }
}

void DevicesModel::updateDeviceName(const QModelIndex& index, const QString& name)
{
    if (index.isValid())
    {
        auto& device = _devices[index.row()];
        device.name = name;
        emit dataChanged(index, index);

        auto query = _connection.createUpdateQuery(entities::Device::tableName(), device.toFieldSet());
        query->execute();
    }
}

QModelIndex DevicesModel::indexById(int id)
{
    int row = 0;
    for (auto device : _devices)
    {
        if (device.id == id)
        {
            return createIndex(row, 0);
        }
        ++row;
    }
    return {};
}

int DevicesModel::idByIndex(const QModelIndex& index)
{
    if (index.isValid())
    {
        return _devices[index.row()].id;
    }
    return 0;
}

int DevicesModel::rowCount(const QModelIndex& /*parent*/) const
{
    return _devices.size();
}

QVariant DevicesModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid() && index.row() < _devices.size())
    {
        if (role == Qt::DisplayRole)
        {
            return _devices[index.row()].name;
        }

        if (role == IdRole)
        {
            return _devices[index.row()].id;
        }
    }

    return {};
}

} // namespace models
