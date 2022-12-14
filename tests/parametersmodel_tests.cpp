#include "models/parametersmodel.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QString>

#include <list>

struct ConnectionMock : db::IConnection
{
    MOCK_METHOD2(createInsertionQuery, std::unique_ptr<db::IQuery>(const QString& tableName, const db::FieldSet& fieldSet));
    MOCK_METHOD2(createUpdateQuery, std::unique_ptr<db::IQuery>(const QString& tableName, const db::FieldSet& fieldSet));
    MOCK_METHOD2(createSelectQuery, std::unique_ptr<db::IQuery>(const QString& tableName, const db::FieldSet& fieldSet));
    MOCK_METHOD2(createSelectQuery, std::unique_ptr<db::IQuery>(const QString& tableName, const QString& fieldList));
    MOCK_METHOD2(createDeleteQuery, std::unique_ptr<db::IQuery>(const QString& tableName, const db::FieldSet& fieldSet));
};

struct QueryMock : db::IQuery
{
    MOCK_METHOD0(execute, void());
    MOCK_METHOD0(getNextResult, std::unique_ptr<db::FieldSet>());

    std::list<db::FieldSet> results;
    QueryMock()
    {
        ON_CALL(*this, getNextResult())
            .WillByDefault(::testing::Invoke(this, &QueryMock::nextResult));
    }

    std::unique_ptr<db::FieldSet> nextResult()
    {
        if (results.empty())
        {
            return nullptr;
        }
        auto result = results.front();
        results.pop_front();
        return std::make_unique<db::FieldSet>(result);
    }
};

TEST(ParameterModelTest, shouldFillFromDbWhenCreated)
{
    QueryMock* queryMock = new QueryMock();
    std::unique_ptr<db::IQuery> q(queryMock);

    ConnectionMock connection;
    EXPECT_CALL(connection, createSelectQuery(QString("parameter"), QString("id, name, device_id, type_id")))
        .WillOnce((::testing::Return(::testing::ByMove(std::move(q)))));

    db::FieldSet first;
    first.append("id", 1);
    first.append("name", "Parameter_1");
    first.append("device_id", 7);
    first.append("type_id", 2);

    queryMock->results.push_back(first);
    {
        ::testing::InSequence seq;
        EXPECT_CALL(*queryMock, execute());
        EXPECT_CALL(*queryMock, getNextResult())
            .Times(2);
    }
    models::ParametersModel model(connection);
    EXPECT_EQ(model.rowCount({}), 1);
}

TEST(ParameterModelTest, shouldAddParameter)
{
    QueryMock* firstSelectQueryMock = new QueryMock();
    std::unique_ptr<db::IQuery> q1(firstSelectQueryMock);

    ConnectionMock connection;
        EXPECT_CALL(connection, createSelectQuery(QString("parameter"), QString("id, name, device_id, type_id")))
        .WillOnce((::testing::Return(::testing::ByMove(std::move(q1)))));

    EXPECT_CALL(*firstSelectQueryMock, execute());
    EXPECT_CALL(*firstSelectQueryMock, getNextResult())
        .Times(1);

    models::ParametersModel model(connection);

    QueryMock* insertionQueryMock = new QueryMock();
    std::unique_ptr<db::IQuery> q2(insertionQueryMock);

    EXPECT_CALL(connection, createInsertionQuery(QString("parameter"), ::testing::_))
        .WillOnce((::testing::Return(::testing::ByMove(std::move(q2)))));

    EXPECT_CALL(*insertionQueryMock, execute());

    QueryMock* secondSelectQueryMock = new QueryMock();
    db::FieldSet first;
    first.append("id", 1);

    secondSelectQueryMock->results.push_back(first);
    std::unique_ptr<db::IQuery> q3(secondSelectQueryMock);
    EXPECT_CALL(connection, createSelectQuery(QString("parameter"), QString("max(id) AS id")))
        .WillOnce((::testing::Return(::testing::ByMove(std::move(q3)))));

    EXPECT_CALL(*secondSelectQueryMock, execute());
    EXPECT_CALL(*secondSelectQueryMock, getNextResult());

    ASSERT_EQ(model.rowCount({}), 0);
    model.addParameter(1);
    EXPECT_EQ(model.rowCount({}), 1);
}

TEST(ParameterModelTest, shouldDeleteParameterFromDb)
{
    QueryMock* queryMock = new QueryMock();
    std::unique_ptr<db::IQuery> q1(queryMock);

    ConnectionMock connection;
    EXPECT_CALL(connection, createSelectQuery(QString("parameter"), QString("id, name, device_id, type_id")))
        .WillOnce((::testing::Return(::testing::ByMove(std::move(q1)))));

    db::FieldSet first;
    first.append("id", 1);
    first.append("name", "Parameter_1");
    first.append("device_id", 7);
    first.append("type_id", 2);

    queryMock->results.push_back(first);
    EXPECT_CALL(*queryMock, execute());
    EXPECT_CALL(*queryMock, getNextResult())
        .Times(2);
    models::ParametersModel model(connection);

    db::FieldSet parameters;
    parameters.append("id", 1);
    QueryMock* secondQueryMock = new QueryMock();
    std::unique_ptr<db::IQuery> q2(queryMock);

    EXPECT_CALL(connection, createDeleteQuery(QString("parameter"), parameters))
        .WillOnce((::testing::Return(::testing::ByMove(std::move(q2)))));

    EXPECT_CALL(*secondQueryMock, execute());
    model.deleteParameter(model.indexById(1));
}
//
//TEST(DeviceModelTest, shouldUpdateDeviceNameInDb)
//{
//    QueryMock* queryMock = new QueryMock();
//    std::unique_ptr<db::IQuery> q1(queryMock);
//
//    ConnectionMock connection;
//    EXPECT_CALL(connection, createSelectQuery(QString("device"), QString("id, name")))
//        .WillOnce((::testing::Return(::testing::ByMove(std::move(q1)))));
//
//    db::FieldSet first;
//    first.append("id", 1);
//    first.append("name", "Device_1");
//
//    queryMock->results.push_back(first);
//    EXPECT_CALL(*queryMock, execute());
//    EXPECT_CALL(*queryMock, getNextResult())
//        .Times(2);
//    models::DevicesModel model(connection);
//
//    db::FieldSet parameters;
//    parameters.append("id", 1);
//    parameters.append("name", "Device_2");
//    QueryMock* secondQueryMock = new QueryMock();
//    std::unique_ptr<db::IQuery> q2(queryMock);
//
//    EXPECT_CALL(connection, createUpdateQuery(QString("device"), parameters))
//        .WillOnce((::testing::Return(::testing::ByMove(std::move(q2)))));
//
//    EXPECT_CALL(*secondQueryMock, execute());
//    model.updateDeviceName(model.indexById(1), "Device_2");
//}
//
