#include "dialog.h"
#include "ui_dialog.h"

#include <QStyledItemDelegate>
#include <QComboBox>

namespace
{
    class ComboBoxItemDelegate : public QStyledItemDelegate
    {
        // QAbstractItemDelegate interface
    public:
        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
        {
            auto result = new QComboBox(parent);
            for (auto pair : entities::Parameter::typeNamesMap())
            {
                result->addItem(pair.second, pair.first);
            }
            return result;
        }

        void setEditorData(QWidget* editor, const QModelIndex& index) const
        {
            auto comboBox = qobject_cast<QComboBox*>(editor);
            comboBox->setCurrentIndex(comboBox->findData(index.data(Qt::EditRole)));
        }
        void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
        {
            auto comboBox = qobject_cast<QComboBox*>(editor);
            model->setData(index, comboBox->currentData().toInt());
        }
    };
}

Dialog::Dialog(const core::InterfaceRegistry& registry, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::Dialog)
    , _connection(*registry.get<db::IConnection>())
    , _devicesModel(_connection)
    , _parametersModel(_connection)
{
    _ui->setupUi(this);

    _ui->devicesListView->setModel(&_devicesModel);
    _ui->parametersView->setModel(&_parametersModel);
    _ui->parametersView->setItemDelegateForColumn(1, new ComboBoxItemDelegate);
    auto parametersSelectionModel = _ui->parametersView->selectionModel();
    connect(parametersSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& /*deselected*/)
    {
        if (selected.indexes().empty())
        {
            setCurrentParameterId(0);
        }
        else
        {
            auto index = selected.indexes().first();
            if (index.isValid())
            {
                setCurrentParameterId(_parametersModel.idByIndex(index));
            }
            else
            {
                setCurrentParameterId(0);
            }
        }
    });
    auto devicesSelectionModel = _ui->devicesListView->selectionModel();
    connect(devicesSelectionModel, &QItemSelectionModel::selectionChanged, [this](const QItemSelection& selected, const QItemSelection& /*deselected*/)
    {
        if (selected.indexes().empty())
        {
            setCurrentId(0);
        }
        else
        {
            auto index = selected.indexes().first();
            if (index.isValid())
            {
                setCurrentId(_devicesModel.idByIndex(index));
            }
            else
            {
                setCurrentId(0);
            }
        }
    });
    setCurrentId(0);
    setCurrentParameterId(0);
}

Dialog::~Dialog()
{
    delete _ui;
}

void Dialog::on_addPushButton_clicked()
{
    _devicesModel.addDevice();
}

void Dialog::on_removePushButton_clicked()
{
    if (_ui->devicesListView->selectionModel()->hasSelection())
    {
        _devicesModel.deleteDevice(_ui->devicesListView->selectionModel()->selectedRows().first());
    }
}

void Dialog::setCurrentId(int currentId)
{
    if (_currentDeviceId == currentId)
    {
        return;
    }

    _currentDeviceId = currentId;

    _ui->removePushButton->setEnabled(_currentDeviceId != 0);
    _ui->stackedWidget->setCurrentIndex(_currentDeviceId == 0 ? 0 : 1);
    _parametersModel.setCurrentDeviceId(_currentDeviceId);

    if (_currentDeviceId != 0)
    {
        _ui->lineEdit->setText(_devicesModel.data(_devicesModel.indexById(_currentDeviceId), Qt::DisplayRole).toString());
    }
}

void Dialog::setCurrentParameterId(int currentId)
{
    if (_currentParameterId == currentId)
    {
        return;
    }

    _currentParameterId = currentId;

    _ui->removePushButton_2->setEnabled(_currentParameterId != 0);
}

void Dialog::on_lineEdit_editingFinished()
{
    if (_currentDeviceId != 0)
    {
        _devicesModel.updateDeviceName(_devicesModel.indexById(_currentDeviceId), _ui->lineEdit->text());
    }
}

void Dialog::on_addPushButton_2_clicked()
{
    if (_currentDeviceId != 0)
    {
        _parametersModel.addParameter(_currentDeviceId);
    }
}

void Dialog::on_removePushButton_2_clicked()
{
    if (_ui->parametersView->selectionModel()->hasSelection())
    {
        _parametersModel.deleteParameter(_ui->parametersView->selectionModel()->selectedRows().first());
    }
}
