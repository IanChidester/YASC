#include "yascclient.h"

YASCClient::YASCClient(QObject *parent) : QObject(parent)
{
    crypto.setKey(0xf792525d4e470448); //set the encryptoin key
}

YASCClient::~YASCClient()
{
    socket->disconnectFromHost();
}

bool YASCClient::run(QString ip, QString port, QString nickname) {
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << "connecting from client";

    this->nickname = nickname;

    socket->connectToHost(QHostAddress(ip), port.toInt());

    if(!socket->waitForConnected(5000)) {
        qDebug() << "Error: " << socket->errorString();
        return false;
    }
    return true;
}

void YASCClient::connected() {
    qDebug() << "connected...";

    QByteArray nick = crypto.encryptToByteArray(nickname);
    socket->write(nick);
}

void YASCClient::disconnected() {
    qDebug() << "disconnected...";
}

void YASCClient::bytesWritten(qint64 bytes) {
    qDebug() << bytes << " bytes written...";
}

void YASCClient::readyRead() {
    QByteArray message = crypto.decryptToByteArray(socket->readAll());
    emit incomingMessage(QString(message));
    qDebug() << QString(message);
}

void YASCClient::sendMessage(QString message) {
    qDebug() << "sending message...";

    QByteArray msgToSend = crypto.encryptToByteArray(message);
    socket->write(msgToSend);
}

QString YASCClient::getNickname() {
    return this->nickname;
}

bool YASCClient::audioRun(QString ip, QString port) {
    return false;
    // i need to set up reciving audio before connecting, then set up sending audio after connecting.
    if (setupAudioSocket(ip, port)) {
        setupAudioInputOutput();
        return true;
    }
    return false;
}

bool YASCClient::setupAudioSocket(QString ip, QString port) {
    audioSocket = new QTcpSocket(this);

    connect(audioSocket, SIGNAL(readyRead()), this, SLOT(incommingAudioReadyRead()));

    audioSocket->connectToHost(QHostAddress(ip), port.toInt());

    if(!audioSocket->waitForConnected(5000)) {
        qDebug() << "Audio Socket Error: " << audioSocket->errorString();
        return false;
    }
    return true;
}

void YASCClient::setupAudioInputOutput() {
    m_device = QAudioDeviceInfo::defaultInputDevice();
    m_buffer = QByteArray(BufferSize, 0);

    m_audioInput = new QAudioInput(m_device, getFormat());
    m_audioOutput = new QAudioOutput(m_device, getFormat());

    m_input = m_audioInput->start();
    m_input->open(QIODevice::ReadWrite);
    connect(m_input, SIGNAL(readyRead()), this, SLOT(audioReadMore()));

    m_output = m_audioOutput->start();
}

void YASCClient::audioReadMore() {
    qDebug() << "readMore()";
    if (!(m_audioInput && m_input))
        return;
    qint64 len = m_audioInput->bytesReady();
    if (len > BufferSize)
        len = BufferSize;
    qint64 l = m_input->read(m_buffer.data(), len);
    if (l > 0) {
        qDebug() << " Got " << l << " data for writing";

        //m_audioInfo->write(m_buffer.constData(), l);

        //m_output->write(m_buffer.constData(), l);

        audioSocket->write(m_buffer, l);

        qDebug() << "m_buffer.size " << QString::number(m_buffer.length());
    }
}

void YASCClient::incommingAudioReadyRead() {
    auto audioBuffer = audioSocket->readAll();

    if (m_output)
        m_output->write(audioBuffer, audioSocket->bytesToWrite());
}

QAudioFormat YASCClient::getFormat() {
    QAudioFormat m_format;

    m_format.setSampleRate(8000);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setSampleType(QAudioFormat::SignedInt);
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    return m_format;
}
