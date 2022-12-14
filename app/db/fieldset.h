#ifndef RESULT_H
#define RESULT_H

#include <QVector>
#include <QPair>
#include <QVariant>
#include <QString>

namespace db {

struct FieldSet
{
    QVector<QPair<QString, QVariant>> fields;

public:
    QVariant value(const QString& name) const
    {
        for (auto f : fields)
        {
            if (f.first == name)
            {
                return f.second;
            }
        }

        return {};
    }

    void append(const QString& name, const QVariant& value)
    {
        fields.push_back(QPair<QString, QVariant>(name, value));
    }

    bool operator==(const FieldSet& right) const
    {
        return fields == right.fields;
    }
};
}

#endif // RESULT_H
