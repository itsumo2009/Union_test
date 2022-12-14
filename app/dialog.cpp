#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(const core::InterfaceRegistry& registry, QWidget *parent)
    : QDialog(parent)
    , _ui(new Ui::Dialog)
    , _connection(*registry.get<db::IConnection>())
    , _devicesModel(_connection)
{
    _ui->setupUi(this);

    _ui->devicesListView->setModel(&_devicesModel);
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
    if (_currentId == currentId)
    {
        return;
    }

    _currentId = currentId;

    _ui->removePushButton->setEnabled(_currentId != 0);
    _ui->stackedWidget->setCurrentIndex(_currentId == 0 ? 0 : 1);

    if (_currentId != 0)
    {
        _ui->lineEdit->setText(_devicesModel.data(_devicesModel.indexById(_currentId), Qt::DisplayRole).toString());
    }
}

void Dialog::on_lineEdit_editingFinished()
{
    if (_currentId != 0)
    {
        _devicesModel.updateDeviceName(_devicesModel.indexById(_currentId), _ui->lineEdit->text());
    }
}
