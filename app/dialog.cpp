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
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
        {
            QStyleOptionComboBox comboBoxOption;
            comboBoxOption.rect = option.rect;
            comboBoxOption.currentText = index.data(Qt::DisplayRole).toString();
            QApplication::style()->drawComplexControl(QStyle::CC_ComboBox,
                                               &comboBoxOption, painter);
            QApplication::style()->drawControl(QStyle::CE_ComboBoxLabel,
                                               &comboBoxOption, painter);
        }

        QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
        {
            return new QComboBox(parent);
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
    if (_currentId != 0)
    {
        _devicesModel.updateDeviceName(_devicesModel.indexById(_currentId), _ui->lineEdit->text());
    }
}

void Dialog::on_addPushButton_2_clicked()
{
    if (_currentId != 0)
    {
        _parametersModel.addParameter(_currentId);
    }
}

void Dialog::on_removePushButton_2_clicked()
{
    if (_ui->parametersView->selectionModel()->hasSelection())
    {
        _parametersModel.deleteParameter(_ui->parametersView->selectionModel()->selectedRows().first());
    }
}
