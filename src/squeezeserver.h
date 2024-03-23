#ifndef SQUEEZESERVER_H
#define SQUEEZESERVER_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QHash>
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QJsonDocument>
#include <QTimer>
#include "slimp3cli.h"
#include "slimp3json.h"
#include "slimp3playermodel.h"
#include "squeezedefines.h"

class SqueezeServer : public QObject
{
    Q_OBJECT
    // QML_ELEMENT

    Q_PROPERTY(QString ServerAddress READ ServerAddress CONSTANT)
    Q_PROPERTY(quint16 cliPort READ cliPort CONSTANT)
    Q_PROPERTY(quint16 jsonPort READ jsonPort CONSTANT)
    Q_PROPERTY(Slimp3CLI* slimp3CLI READ slimp3CLI CONSTANT)
    Q_PROPERTY(Slimp3JSON* slimp3JSON READ slimp3JSON CONSTANT)
    Q_PROPERTY(Slimp3PlayerModel* PlayerModel READ PlayerModel NOTIFY PlayerModelChanged FINAL)
    Q_PROPERTY(Slimp3Player* curPlayer READ curPlayer WRITE setCurPlayer NOTIFY curPlayerChanged FINAL)
    Q_PROPERTY(bool cliConnected READ cliConnected WRITE setCliConnected NOTIFY cliConnectedChanged FINAL)


public:
    explicit SqueezeServer(QObject *parent, QQmlApplicationEngine *myEngine, QUrl url,
                           QString serverAddress = "127.0.0.1");

    void Init(void);

    QString ServerAddress() const;
    quint16 cliPort() const;
    quint16 jsonPort() const;
    Slimp3CLI *slimp3CLI() const;
    Slimp3JSON *slimp3JSON() const;
    Slimp3PlayerModel *PlayerModel() const;

    bool hasMacAddress(QString mac);

    // Slimp3Player *curPlayer() const;
    void setCurPlayer(Slimp3Player *newCurPlayer);

    bool cliConnected() const;
    void setCliConnected(bool newCliConnected);

public slots:
    void AddPlayers(QJsonValue doc);
    void UpdatePlayer(QString mac, QJsonDocument &doc);
    void UpdatePlayer(QString mac, playerUpdateField field, QString val);
    void receivePlayerCLICommand(QString mac, QString cmd);
    void receivePlayerJSONCommand(QString mac, QString cmd);
    void receiveCLIResponse(QByteArray msg);
    void receiveJSONResponse(QByteArray msg);

    void makePlayerCurrent(QString mac);
    Slimp3Player *curPlayer();

    void pauseButton( QString mac, int val );

signals:
    void ServerAddressChanged();
    void cliPortChanged();
    void jsonPortChanged();
    void slimp3CLIChanged();
    void slimp3JSONChanged();
    void PlayerModelChanged();

    void curPlayerChanged();

    void cliConnectedChanged();

private:
    QString m_ServerAddress;
    quint16 m_cliPort;
    quint16 m_jsonPort;
    Slimp3CLI *m_slimp3CLI = nullptr;
    Slimp3JSON *m_slimp3JSON = nullptr;
    Slimp3PlayerModel *m_PlayerModel = nullptr;
    static QRegularExpression macRegex;
    QQmlApplicationEngine *engine;
    QUrl url;
    Slimp3Player *m_curPlayer = nullptr;
    bool m_cliConnected;

    QTimer tock;
};

#endif // SQUEEZESERVER_H
