#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include <QTime>
#include "../server/yascserver.h"
#include "../client/yascclient.h"

void delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

class YASCTestSuiteTest : public QObject
{
    Q_OBJECT

public:
    YASCTestSuiteTest();

private Q_SLOTS:
    void testServerOpen();
    void testClientConnect();
    void testClientDisconnected();
};

YASCTestSuiteTest::YASCTestSuiteTest()
{
}

void YASCTestSuiteTest::testServerOpen()
{
    YASCServer server(this);
    QVERIFY2(server.startServer(), "Failed to open connection");
}

void YASCTestSuiteTest::testClientConnect()
{
    YASCServer server(this);
    QVERIFY2(server.startServer(), "Failed to open connection");
    delay(1000);
    YASCClient client(this);
    QVERIFY2(client.run("127.0.0.1", "12345", "nickname"), "Failed to connect to server");
}

void YASCTestSuiteTest::testClientDisconnected()
{
    YASCServer server;
    QVERIFY2(server.startServer(), "Failed to open connection");
    delay(5000);
    QSignalSpy spy(&server, SIGNAL(clientDisconnected()));
    {
        YASCClient* client = new YASCClient(this);
        QVERIFY2(client->run("127.0.0.1", "12345", "nickname"), "Failed to connect to server");
        client->sendMessage("new message");
        delay(1000);
        delete client;
    }
    QCOMPARE(spy.count(), 1);
    delay(1000);
}

QTEST_APPLESS_MAIN(YASCTestSuiteTest)

#include "tst_yasctestsuitetest.moc"
