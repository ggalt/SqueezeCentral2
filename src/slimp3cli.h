#ifndef SLIMP3CLI_H
#define SLIMP3CLI_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QTcpSocket>
#include "squeezedefines.h"

class Slimp3CLI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ServerAddress READ ServerAddress CONSTANT)
    Q_PROPERTY(quint16 CliPort READ CliPort  CONSTANT)

public:
    explicit Slimp3CLI(QObject *parent = nullptr,
                       QString serverAdd = "127.0.0.1",
                       quint16 cliport = 9090);
    ~Slimp3CLI();

    void Init(void);
    void Connect(void);

    QString ServerAddress() const;
    quint16 CliPort() const;

signals:
    void Error(QString message);
    void initialized();
    void ResponseReady(QByteArray msg);


public slots:
    void sendStandardCommand(CliCommand cmd);
    void sendStandardCommand(CliCommand cmd, QByteArray mac);
    void sendCommand( QByteArray cmd );
    void sendCommand( QByteArray cmd, QByteArray mac );
    void messageWaiting(void);
    void Connected(void);
    void Disconnected(void);
    void ConnectionError(QAbstractSocket::SocketError err);

private:
    SqueezeServer *squeezeServer;   // convenience pointer
    QString m_ServerAddress;
    quint16 m_CliPort;
    QTcpSocket *m_cliSocket;
};

#endif // SLIMP3CLI_H
