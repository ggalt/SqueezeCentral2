#include "squeezeserver.h"
#include <QDebug>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlContext>
#include <QStringList>


#ifdef SQUEEZESERVER_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif

QRegularExpression SqueezeServer::macRegex = QRegularExpression("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");


SqueezeServer::SqueezeServer(QObject *parent, QQmlApplicationEngine *myEngine, QUrl mainURL, QString serverAddress)
    : QObject{parent}
{
    m_PlayerModel = new Slimp3PlayerModel(this);
    m_ServerAddress = serverAddress;
    m_slimp3CLI = new Slimp3CLI(this, m_ServerAddress);
    m_slimp3JSON = new Slimp3JSON(this, m_ServerAddress);
    engine = myEngine;
    url = mainURL;

    tock.setInterval(1000);
    tock.start();
}

void SqueezeServer::Init()
{
    DEBUGF("SQUEEZESERVER INIT");
    m_slimp3CLI->Init();
    m_slimp3JSON->Init();

    connect(m_slimp3JSON, SIGNAL(PlayersAdded(QJsonValue)),
            this, SLOT(AddPlayers(QJsonValue)));

    m_slimp3JSON->sendJSONCommand("", {"players","0","100"});
}

QString SqueezeServer::ServerAddress() const
{
    return m_ServerAddress;
}

quint16 SqueezeServer::cliPort() const
{
    return m_cliPort;
}

quint16 SqueezeServer::jsonPort() const
{
    return m_jsonPort;
}

Slimp3CLI *SqueezeServer::slimp3CLI() const
{
    return m_slimp3CLI;
}

Slimp3JSON *SqueezeServer::slimp3JSON() const
{
    return m_slimp3JSON;
}

void SqueezeServer::AddPlayers(QJsonValue jValue)
{
    int pCount = jValue["count"].toInt(-1);

    if( pCount <= 0 ) {
        DEBUGF("ERROR, NO PLAYERS");
        return;
    }

    QJsonValue playerLoopArray(jValue["players_loop"]);

    for( int c = 0; c < pCount; c++ ) {
        DEBUGF(playerLoopArray[c]["playerid"]);
        Slimp3Player2 *tempPlayer = new Slimp3Player2(this);
        tempPlayer->setPlayerName(playerLoopArray[c]["name"].toString(nullptr));
        tempPlayer->setMacAddress(playerLoopArray[c]["playerid"].toString(nullptr));
        tempPlayer->setIsPlaying(playerLoopArray[c]["isplaying"].toInt(-1));
        tempPlayer->setPlayerIndex(c);
        PlayerModel()->addPlayer(tempPlayer);
        connect(&tock, SIGNAL(timeout()), tempPlayer, SLOT(Tick()));
        // connect(tempPlayer, SIGNAL(playlistCurIndexChanged()), this, SLOT(songChange()));
        m_slimp3JSON->sendJSONCommand(tempPlayer->macAddress(), {"status","0", "100", "tags:a,c,d,J,l,t"});
    }
}

void SqueezeServer::UpdatePlayer(QString mac, QJsonDocument &doc)
{
    int c = 0;
    DEBUGF("UPDATE PLAYER");
    if( doc["params"][1][0].toString() == "status" ) {
        Slimp3Player2* player = m_PlayerModel->find(mac.toLatin1());
        if( player == nullptr ) {
            DEBUGF("NULL POINTER");
        }
        player->UpdatePlayerValues(doc);
        DEBUGF("PLAYER INDEX:" << player->playerIndex() << "PLAYER MODEL COUNT" << m_PlayerModel->rowCount());
        if( !uiDone() && player->playerIndex() == m_PlayerModel->rowCount() -1 ){    // last player updated
            DEBUGF("LAST PLAYER UPDATED:" << player->playerIndex() );
            this->setCurPlayer(player);
            engine->rootContext()->setContextProperty("CurrentPlayer", this->curPlayer());
            engine->rootContext()->setContextProperty("tock", &tock);
            engine->rootContext()->setContextProperty("squeezeServer", this);
            engine->load(url);
            QObject *rootObj = engine->rootObjects().at(0);

            connect(rootObj, SIGNAL(pauseButton(QString,int)), this, SLOT(pauseButton(QString,int)));
            connect(rootObj, SIGNAL(forwardButton(QString)), this, SLOT(forwardButton(QString)));
            connect(rootObj, SIGNAL(rewindButton(QString)), this, SLOT(rewindButton(QString)));
            setUiDone(true);
        }
    }

}

void SqueezeServer::UpdatePlayer(QString mac, playerUpdateField field, QString val)
{
    Slimp3Player2* player = m_PlayerModel->findPlayerByMAC(mac);

    if( field == MODESTATUS && val == "pause 0" ) {
        player->setPlayerMode("play");
    } else if( field == MODESTATUS && val == "pause 1" ) {
        player->setPlayerMode("pause");
    }
}

void SqueezeServer::receivePlayerCLICommand(QString mac, QString cmd)
{
    DEBUGF("Player:" << mac << "Command:" << cmd);
    QStringList cmdList = cmd.split(" ", Qt::SkipEmptyParts);
    Slimp3Player2* player = m_PlayerModel->findPlayerByMAC(mac);

    DEBUGF("COMMANDS:" << cmdList);
    if( cmdList[0] == "playlist" ) {
        DEBUGF("PLAYLIST COMMAND");

        if( cmdList[1] == "pause" ) {
            if( cmdList[2] == "0" ) {  //  play
                player->setPlayerMode("play");
                DEBUGF("PLAY");
            } else if( cmdList[2] == "1") {
                player->setPlayerMode("pause");
                DEBUGF("PAUSE");
            }

        } else if( cmdList[1] == "newsong" ) {
            int newsongIndex = cmdList[cmdList.size()-1 ].toInt();
            DEBUGF("new song at index:" << newsongIndex);
            player->setPlaylistCurIndex( newsongIndex );

        } else if( cmdList[1] == "repeat" ) {
            player->setRepeatPlaylist( cmdList[2].toInt() );

        } else if( cmdList[1] == "shuffle" ) {
            player->setShufflePlaylist( cmdList[2].toInt() );

        } else if( cmdList[1] == "stop" ) {
            DEBUGF("STOP");
            player->setPlayerMode("pause");
        } else if( cmdList[1] == "loadtracks" ) {  // loading new playlist
            DEBUGF("LOADING TRACKS");
            m_slimp3JSON->sendJSONCommand(player->macAddress(), {"status","0", "100", "tags:a,c,d,J,l,t"});
        } else if( cmdList[1]== "addtracks") { // adding to existing playlist
            DEBUGF("ADDING TRACKS");
            m_slimp3JSON->sendJSONCommand(player->macAddress(), {"status","0", "100", "tags:a,c,d,J,l,t"});
        }
        else {
            DEBUGF("UNKNOWN PLAYLIST COMMAND" << cmdList);
        }

    } else if( cmdList[0] == "mixer" ) {
        DEBUGF("MIXER COMMAND" << cmdList );
        QString vol = QByteArray::fromPercentEncoding( cmdList[2].toLatin1() );
        int currVol = player->mixerVolume();
        if( vol.contains("+") ) {
            vol = vol.sliced( vol.lastIndexOf("+") );   // get rid of the "+" which should leave a number
            player->setMixerVolume( currVol + vol.toInt() );
        } else if( vol.contains( "-" ) ) {
            vol = vol.sliced( vol.lastIndexOf("-") );   // get rid of the "-" which should leave a number
            player->setMixerVolume( currVol - vol.toInt() );
        } else {
            player->setMixerVolume( vol.toInt() );
        }

    } else if( cmdList[0] == "pause" ) {
        DEBUGF("PAUSE COMMAND" << cmdList );
        player->setPlayerMode("pause");

    } else if( cmdList[0] == "play" ) {
        DEBUGF("PAUSE COMMAND" << cmdList );
        player->setPlayerMode("play");

    } else {
        DEBUGF("UNKNOWN COMMAND" << cmdList);

    }
}

void SqueezeServer::receivePlayerJSONCommand(QString mac, QString cmd)
{
    QJsonDocument req;
    QByteArray jsonText = "{'id':1,'method':'slim.request','params':[";
    jsonText + "'" + mac +"'";
    jsonText + "['" + cmd + "']]}";
    req.fromJson(jsonText);
    DEBUGF(req);

}

void SqueezeServer::receiveCLIResponse(QByteArray msg)
{
    DEBUGF(msg);
    if( hasMacAddress(msg)) {
        // player specific response
        QString mac = QByteArray::fromPercentEncoding( msg.first(27));
        QString cmd = msg.sliced(28).trimmed();
        if( cmd.endsWith('\n') )
            cmd.removeLast();
        if( cmd.contains("%") )
            cmd = QByteArray::fromPercentEncoding(cmd.toLatin1());
        receivePlayerCLICommand(mac, cmd);
    } else {
        if(msg.startsWith("player count ")) {
            int pCount = msg.sliced(13, msg.length() - 13).toInt();
            if(pCount > 0) {
                m_slimp3JSON->sendJSONCommand("", {"players", "0", "500"});
            }
            // slimp3CLI()->sendStandardCommand(C_SUBSCRIBE);
        }
    }

}

void SqueezeServer::receiveJSONResponse(QByteArray msg)
{

}

void SqueezeServer::makePlayerCurrent(QString mac)
{
    setCurPlayer(m_PlayerModel->findPlayerByMAC(mac));
}

Slimp3PlayerModel* SqueezeServer::PlayerModel() const
{
    return m_PlayerModel;
}

bool SqueezeServer::hasMacAddress(QString mac)
{
    QString cleanMac;
    if( mac.contains("%") ) {   // uuencoded string
        cleanMac = QByteArray::fromPercentEncoding(mac.first(27).toLatin1());
    } else {
        cleanMac = mac;
    }
    QRegularExpressionMatch match = macRegex.match(cleanMac);
    bool isMatch = match.hasMatch();
    DEBUGF("original:" << mac << "clean:" << cleanMac << "MATCHED?" << isMatch);
    return isMatch;
}


Slimp3Player2 *SqueezeServer::curPlayer()
{
    return m_curPlayer;
}

void SqueezeServer::pauseButton(QString mac, int val)
{
    DEBUGF("PAUSEBUTTON PUSHED WITH:" << val << mac);
    if( val == 1 )
        m_slimp3CLI->sendStandardCommand(C_PLAY, mac.toLatin1());
    else
        m_slimp3CLI->sendStandardCommand(C_PAUSE, mac.toLatin1());
}

void SqueezeServer::forwardButton(QString mac)
{
    DEBUGF("FORWARD BUTTON PUSHED WITH:" << mac);
    m_slimp3CLI->sendStandardCommand(C_NEXTTRACK, mac.toLatin1());
}

void SqueezeServer::rewindButton(QString mac)
{
    DEBUGF("REWIND BUTTON PUSHED WITH:" << mac);
    m_slimp3CLI->sendStandardCommand(C_PREVIOUSTRACK, mac.toLatin1());
}

void SqueezeServer::songChange()
{
    // emit this->PlayerModel()->dataChanged(QModelIndex(), QModelIndex());
    emit songChanged();
}

void SqueezeServer::setCurPlayer(Slimp3Player2 *newCurPlayer)
{
    if (m_curPlayer == newCurPlayer)
        return;
    m_curPlayer = newCurPlayer;
    emit curPlayerChanged();
}

bool SqueezeServer::cliConnected() const
{
    return m_cliConnected;
}

void SqueezeServer::setCliConnected(bool newCliConnected)
{
    if (m_cliConnected == newCliConnected)
        return;
    m_cliConnected = newCliConnected;
    emit cliConnectedChanged();
}

bool SqueezeServer::uiDone() const
{
    return m_uiDone;
}

void SqueezeServer::setUiDone(bool newUiDone)
{
    if (m_uiDone == newUiDone)
        return;
    m_uiDone = newUiDone;
    emit uiDoneChanged();
}
