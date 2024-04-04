#include "slimp3cli.h"
#include "squeezedefines.h"
#include "squeezeserver.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

#ifdef SLIMCLI_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif


Slimp3CLI::Slimp3CLI(QObject *parent, QString serverAdd, quint16 cliport)
    : QObject{parent}
{
    m_ServerAddress = serverAdd;
    m_CliPort = cliport;
    squeezeServer = static_cast<SqueezeServer*>(parent); // establish convenience pointer
}

Slimp3CLI::~Slimp3CLI()
{
    disconnect(m_cliSocket, SIGNAL(disconnected()));
    sendCommand("exit");
    m_cliSocket->flush();
    delete m_cliSocket;
}

void Slimp3CLI::Init()
{
    m_cliSocket = new QTcpSocket(this);
    connect( m_cliSocket, SIGNAL(connected()), this, SLOT(Connected()));
    squeezeServer->setCliConnected(false);

    Connect();
}

void Slimp3CLI::Connect()
{
    DEBUGF("Connecting CLI at:" << m_ServerAddress << "and" << m_CliPort);
    m_cliSocket->connectToHost(m_ServerAddress, m_CliPort);
}

QString Slimp3CLI::ServerAddress() const
{
    return m_ServerAddress;
}

quint16 Slimp3CLI::CliPort() const
{
    return m_CliPort;
}

void Slimp3CLI::sendStandardCommand(CliCommand cmd)
{
    // only allow standard commands that DO NOT use a mac address (i.e., general server commands)
    QByteArray tempCommand;
    DEBUGF("SENDING STANDARD COMMAND:" << cmd);
    switch(cmd) {
    case C_GETPLAYERCOUNT:
        tempCommand = "player count ?";
        sendCommand(tempCommand);
        break;
    case C_SUBSCRIBE:
        // tempCommand = "subscribe playlist,mixer,pause \n";
        tempCommand = "subscribe playlist,mixer,pause \n";
        sendCommand(tempCommand);
        break;
    default:
        break;
    }
}

void Slimp3CLI::sendStandardCommand(CliCommand cmd, QByteArray mac)
{
    switch(cmd) {
    case C_NEXTTRACK:
        sendCommand(QByteArray("button fwd.single\n"), mac);
        break;
    case C_PREVIOUSTRACK:
        sendCommand(QByteArray("button rew\n"), mac);
        break;
    case C_PLAY:
        sendCommand(QByteArray("play"), mac);
        break;
    case C_PAUSE:
        sendCommand(QByteArray("pause"), mac);
        break;
    case C_SHUFFLE_BY_SONG:
        sendCommand(QByteArray("playlist shuffle 1\n"), mac);
        break;
    case C_SHUFFLE_BY_PLAYLIST:
        sendCommand(QByteArray("playlist shuffle 2\n"), mac);
        break;
    case C_SHUFFLE_OFF:
        sendCommand(QByteArray("playlist shuffle 0\n"), mac);
        break;
    case C_REPEAT_SONG:
        sendCommand(QByteArray("playlist repeat 1\n"), mac);
        break;
    case C_REPEAT_PLAYLIST:
        sendCommand(QByteArray("playlist repeat 2\n"), mac);
        break;
    case C_REPEAT_OFF:
        sendCommand(QByteArray("playlist repeat 0\n"), mac);
        break;
    case C_MUTE:
        sendCommand( QByteArray("mixer muting 1"), mac);
    case C_UNMUTE:
        sendCommand( QByteArray("mixer muting 0"), mac);
    case C_MUTE_TOGGLE:
        sendCommand( QByteArray("mixer muting toggle"), mac);
    case C_ISMUTED:
        sendCommand( QByteArray("mixer muting ?"), mac);
    default:
        break;
    }
}

void Slimp3CLI::sendCommand(QByteArray cmd)
{
    if(!cmd.trimmed().endsWith("\n")) {     // all commands need to end with a newline
        cmd = cmd.trimmed() + "\n";
    }
    if(m_cliSocket->write(cmd) > 0) {
        m_cliSocket->flush();
    }
}

void Slimp3CLI::sendCommand(QByteArray cmd, QByteArray mac)
{
    cmd = mac + " " + cmd;
    sendCommand(cmd);
}

void Slimp3CLI::messageWaiting()
{
    while (m_cliSocket->bytesAvailable()) {
        if( m_cliSocket->canReadLine()) {
            QByteArray msg = m_cliSocket->readLine();
            DEBUGF(msg);
            emit ResponseReady(msg);
        }
    }
}

void Slimp3CLI::Connected()
{
    DEBUGF("CONNECTED!!");
    connect( m_cliSocket, SIGNAL(readyRead()), this, SLOT(messageWaiting()));
    connect( m_cliSocket, SIGNAL( errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(ConnectionError(QAbstractSocket::SocketError)));

    connect(this, SIGNAL(ResponseReady(QByteArray)),
            squeezeServer, SLOT(receiveCLIResponse(QByteArray)));

    sendStandardCommand(C_SUBSCRIBE);
    emit initialized();
}

void Slimp3CLI::Disconnected()
{

}

void Slimp3CLI::ConnectionError(QAbstractSocket::SocketError m_err)
{
}
