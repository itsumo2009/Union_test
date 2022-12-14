#ifndef MODELS_PARAMETERSMODEL_H
#define MODELS_PARAMETERSMODEL_H

#include <QAbstractTableModel>

#include "db/iconnection.h"
#include "entities/parameter.h"

namespace models {

class ParametersModel : public QAbstractTableModel
{
public:
    explicit ParametersModel(db::IConnection& connection);

    void addParameter(int deviceId);
    void deleteParameter(const QModelIndex& index);

    QModelIndex indexById(int id);
    int idByIndex(const QModelIndex &index);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
    db::IConnection& _connection;
    QVector<entities::Parameter> _parameters;
};

} // namespace models

#endif // MODELS_PARAMETERSMODEL_H
