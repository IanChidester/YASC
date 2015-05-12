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

private:
    SimpleCrypt crypto;

private Q_SLOTS:
    void testServerOpen();
    void testEncryption();
    void testAudioFails();
    void testSetGetNick();
    void testSingleServer();
    void testServerReady();
};

YASCTestSuiteTest::YASCTestSuiteTest()
{
}

void YASCTestSuiteTest::testSetGetNick() {
    QString nickname = "NICKNAME";
    YASCClient* client = new YASCClient(this);
    client->setNickname(nickname);
    QVERIFY2(client->getNickname() == nickname, "Nickname was different");
}

void YASCTestSuiteTest::testAudioFails() {
    YASCClient* client = new YASCClient(this);
    QVERIFY2(!client->audioRun("127.0.0.1"), "Audio should have failed.");
}

void YASCTestSuiteTest::testEncryption() {
    crypto.setKey(0xf792525d4e470448); //set the encryptoin key

    QString original = "Hello World";

    QByteArray encrypted = crypto.encryptToByteArray(original);

    QString decrypted = crypto.decryptToString(encrypted);

    QVERIFY2(original == decrypted, "Encryption failed");
}

void YASCTestSuiteTest::testSingleServer() {
    YASCServer server1;
    QVERIFY2(server1.startServer(), "Server1 start server failed");
    YASCServer server2;
    QVERIFY2(!server2.startServer(), "Server2 start server succeeded--it shouldn't have");
}

void YASCTestSuiteTest::testServerReady() {
    YASCServer server(this);

    QSignalSpy spy(&server, SIGNAL(serverReady()));

    server.startServer();

    QCOMPARE(spy.count(), 1);
}

void YASCTestSuiteTest::testServerOpen()
{
    YASCServer server(this);
    QVERIFY2(server.startServer(), "Failed to open connection");
}

//void YASCTestSuiteTest::testClientDisconnected()
//{
//    YASCServer server;
//    QVERIFY2(server.startServer(), "Failed to open connection");
//    delay(5000);
//    QSignalSpy spy(&server, SIGNAL(clientDisconnected()));
//    {
//        YASCClient* client = new YASCClient(this);
//        QVERIFY2(client->run("127.0.0.1", "12345", "nickname"), "Failed to connect to server");
//        client->sendMessage("new message");
//        delay(1000);
//        delete client;
//    }
//    QCOMPARE(spy.count(), 1);
//    delay(1000);
//}

QTEST_APPLESS_MAIN(YASCTestSuiteTest)

#include "tst_yasctestsuitetest.moc"
