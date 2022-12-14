#include "parametersmodel.h"

namespace models {

ParametersModel::ParametersModel(db::IConnection& connection)
    : _connection(connection)
{
    auto query = _connection.createSelectQuery(entities::Parameter::tableName(), "id, name, device_id, type_id");
    query->execute();
    while (auto result = query->getNextResult())
    {
        _parameters.push_back(entities::Parameter::fromFieldSet(*result));
    }
}

void ParametersModel::setCurrentDeviceId(int deviceId)
{
    _currentDeviceId = deviceId;

    beginResetModel();
    _parameters.clear();
    db::FieldSet set;
    set.append("id", {});
    set.append("name", {});
    set.append("type_id", {});
    set.append("device_id", _currentDeviceId);

    auto query = _connection.createSelectQuery(entities::Parameter::tableName(), set);
    query->execute();
    while (auto result = query->getNextResult())
    {
        _parameters.push_back(entities::Parameter::fromFieldSet(*result));
    }
    endResetModel();
}

void ParametersModel::addParameter(int deviceId)
{
    const QString name = "<Новый параметр>";
    db::FieldSet set;
    set.append("name", name);
    set.append("device_id", deviceId);

    auto query = _connection.createInsertionQuery(entities::Parameter::tableName(), set);
    query->execute();
    query =_connection.createSelectQuery(entities::Parameter::tableName(), "max(id) AS id");
    query->execute();

    if (auto result = query->getNextResult())
    {
        beginInsertRows({}, _parameters.size(), _parameters.size());
        _parameters.push_back(entities::Parameter::fromFieldSet(*result));
        _parameters.back().name = name;
        _parameters.back().deviceId = deviceId;

        endInsertRows();
    };
}

void ParametersModel::deleteParameter(const QModelIndex& index)
{
    if (index.isValid())
    {
        auto id = _parameters[index.row()].id;
        db::FieldSet set;
        set.append("id", id);
        auto query = _connection.createDeleteQuery(entities::Parameter::tableName(), set);
        query->execute();

        beginRemoveRows({}, index.row(), index.row());
        _parameters.remove(index.row());
        endRemoveRows();
    }
}

QModelIndex ParametersModel::indexById(int id)
{
    int row = 0;
    for (auto parameter : _parameters)
    {
        if (parameter.id == id)
        {
            return createIndex(row, 0);
        }
        ++row;
    }
    return {};
}

int ParametersModel::idByIndex(const QModelIndex &index)
{
    if (index.isValid())
    {
        return _parameters[index.row()].id;
    }
    return 0;
}

int ParametersModel::rowCount(const QModelIndex& /*parent*/) const
{
    return _parameters.size();
}

int ParametersModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 2;
}

QVariant ParametersModel::data(const QModelIndex& index, int role) const
{
    if (index.isValid())
    {
        if (index.column() == 0)
        {
            if (role == Qt::DisplayRole || role == Qt::EditRole)
            {
                {
                    return _parameters[index.row()].name;
                }
            }
        }
        if (index.column() == 1)
        {
            if (role == Qt::DisplayRole)
            {
                return entities::Parameter::typeNamesMap()[_parameters[index.row()].typeId];
            }
            else if (role == Qt::EditRole)
            {
                return _parameters[index.row()].typeId;
            }
        }
    }

    return QVariant();
}

bool ParametersModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid())
    {
        if (role == Qt::EditRole)
        {
            if (index.column() == 0)
            {
                _parameters[index.row()].name = value.toString();
            }
            else
            {
                _parameters[index.row()].typeId = value.toInt();
            }
            emit dataChanged(index, index);

            auto query = _connection.createUpdateQuery(entities::Parameter::tableName(), _parameters[index.row()].toFieldSet());
            query->execute();
            return true;
        }
    }
    return false;
}

Qt::ItemFlags ParametersModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
    {
        return QAbstractTableModel::flags(index) | Qt::ItemFlag::ItemIsEditable;
    }

    return QAbstractTableModel::flags(index);
}

QVariant ParametersModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    switch (section) {
        case 0: return "Название";
        case 1: return "Тип";
        default: return QVariant();
    }
}

} // namespace models
