#ifndef MODELS_DEVICESMODEL_H
#define MODELS_DEVICESMODEL_H

#include "db/iconnection.h"
#include "entities/device.h"

#include <QAbstractListModel>
#include <QVector>

namespace models {

class DevicesModel : public QAbstractListModel
{
public:
    enum UserRoles
    {
        IdRole = Qt::UserRole
    };

public:
    explicit DevicesModel(db::IConnection& connection);

    void addDevice();
    void deleteDevice(const QModelIndex& index);
    void updateDeviceName(const QModelIndex& index, const QString& name);

    QModelIndex indexById(int id);
    int idByIndex(const QModelIndex &index);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    db::IConnection& _connection;
    QVector<entities::Device> _devices;
};

} // namespace models

#endif // MODELS_DEVICESMODEL_H
