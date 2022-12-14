#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "core/interfaceregistry.h"
#include "db/iconnection.h"
#include "models/devicesmodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(const core::InterfaceRegistry& registry, QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_addPushButton_clicked();
    void on_removePushButton_clicked();

    void on_lineEdit_editingFinished();

private:
    void setCurrentId(int currentId);

private:
    Ui::Dialog *_ui;
    db::IConnection& _connection;
    models::DevicesModel _devicesModel;
    int _currentId = -1;
};
#endif // DIALOG_H
