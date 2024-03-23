#include "squeezeserver.h"
#include "songlistmodel.h"
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

    // m_slimp3CLI->sendStandardCommand(C_SUBSCRIBE);      // We only use the CLI to get pushed messages from server

    m_slimp3JSON->sendJSONCommand("", {"players","0","100"});
    // connect(m_slimp3CLI, SIGNAL(ResponseReady(QByteArray)),
    //         this, SLOT(receiveCLIResponse(QByteArray)));
    connect(m_slimp3JSON, SIGNAL(PlayersAdded(QJsonValue)),
            this, SLOT(AddPlayers(QJsonValue)));

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

    if( pCount < 0 ) {
        DEBUGF("ERROR, NO PLAYERS");
        return;
    }

    QJsonValue playerLoopArray(jValue["players_loop"]);

    for( int c = 0; c < pCount; c++ ) {
        DEBUGF(playerLoopArray[c]["playerid"]);
        Slimp3Player *tempPlayer = new Slimp3Player(this);
        tempPlayer->setPlayerName(playerLoopArray[c]["name"].toString(nullptr));
        tempPlayer->setMacAddress(playerLoopArray[c]["playerid"].toString(nullptr));
        tempPlayer->setIsPlaying(playerLoopArray[c]["isplaying"].toInt(-1));
        tempPlayer->setPlayerIndex(QString::number(c));
        connect(&tock, SIGNAL(timeout()), tempPlayer, SLOT(Tick()));
        PlayerModel()->addPlayer(tempPlayer);
        m_slimp3JSON->sendJSONCommand(tempPlayer->macAddress(), {"status","0", "100", "tags:a,c,d,J,l,t"});
    }
}

void SqueezeServer::UpdatePlayer(QString mac, QJsonDocument &doc)
{
    Slimp3Player* t;
    Slimp3Player* player;
    int c = 0;
    DEBUGF("UPDATE PLAYER");
    if( doc["params"][1][0].toString() == "status" ) {
        player = m_PlayerModel->find(mac.toLatin1());
        if( player == nullptr ) {
            DEBUGF("NULL POINTER");
        }
        DEBUGF("UPDATING PLAYER WITH:" << doc);
        t = new Slimp3Player(this);
        QJsonValue playerResult(doc["result"]);
        t->setVolume(playerResult["mixer volume"].toInt(-1));
        t->setCurrentPlaylistIndex(playerResult["playlist_cur_index"].toString(nullptr));
        t->setPlaylistTrackCount(playerResult["playlist_tracks"].toInt(-1));
        t->setRepeatStatus(playerResult["playlist repeat"].toInt(-1));
        t->setShuffleStatus(playerResult["playlist shuffle"].toInt(-1));
        QJsonArray playlistLoop = playerResult["playlist_loop"].toArray();
        t->setPlayerName(playerResult["player_name"].toString(nullptr));
        t->setElapsedTime(static_cast<int>(playerResult["time"].toDouble(-1)));
        t->setSongDuration(static_cast<int>(playerResult["duration"].toDouble(-1)));
        // t->setElapsedTime(static_cast<QString>(playerResult["time"].toString("0")));
        // t->setSongDuration(static_cast<QString>(playerResult["duration"].toString("0")));
        // DEBUGF("SONG DURATION FROM JSON:" << playerResult["duration"].toDouble(-1) << playerResult["duration"].isDouble());
        int playlistCount = playlistLoop.count();
        for( int i = 0; i < playlistCount; i++ ) {
            QJsonObject playlistItem = playlistLoop[i].toObject();
            SongObject *songObj = new SongObject(t->songListModel());
            songObj->setTitle(playlistItem["title"].toString(nullptr));
            songObj->setArtist(playlistItem["artist"].toString(nullptr));
            songObj->setAlbum(playlistItem["album"].toString(nullptr));
            songObj->setPlayListIndex(playlistItem["playlist index"].toInt(-1));
            songObj->setDuration(playlistItem["duration"].toString(nullptr));
            songObj->setTrackNumber(playlistItem["tracknum"].toString(nullptr));
            songObj->setArtworkID(playlistItem["artwork_track_id"].toString(nullptr));
            t->songListModel()->addSong(songObj);
        }
        player->UpdatePlayerValues(t);
        DEBUGF("PLAYER INDEX:" << player->playerIndex() << "PLAYER MODEL COUNT" << m_PlayerModel->rowCount());
        if( player->playerIndex().toInt() == m_PlayerModel->rowCount() -1 ){    // last player updated
            DEBUGF("LAST PLAYER UPDATED:" << player->playerIndex() );
            engine->load(url);
            engine->rootContext()->setContextProperty("CurrentPlayer", this->curPlayer());
            engine->rootContext()->setContextProperty("tock", &tock);
            QObject *rootObj = engine->rootObjects()[0];

            connect(rootObj, SIGNAL(pauseButton(QString, int)), this, SLOT(pauseButton(QString,int)));

            // QObject *rootItem = engine->rootContext()->contextObject();
            // QObject::connect(this->parent(), SIGNAL(findPlayerByMAC(QString)),
            //                  PlayerModel(), SLOT(findPlayerByMAC(QString)));
            // engine->loadFromModule("Slimp3Control", "Main");
        }
        t->deleteLater();
    }

}

void SqueezeServer::UpdatePlayer(QString mac, playerUpdateField field, QString val)
{
    Slimp3Player* player = m_PlayerModel->findPlayerByMAC(mac);

    if( field == MODESTATUS && val == "pause 0" ) {
        player->setPlayerMode( PLAY );
    } else if( field == MODESTATUS && val == "pause 1" ) {
        player->setPlayerMode( PAUSE );
    }
}

void SqueezeServer::receivePlayerCLICommand(QString mac, QString cmd)
{
    DEBUGF("Player:" << mac << "Command:" << cmd);
    QStringList cmdList = cmd.split(" ", Qt::SkipEmptyParts);

    DEBUGF("COMMANDS:" << cmdList);
    if( cmdList[0] == "playlist" ) {
        DEBUGF("PLAYLIST COMMAND");

        if( cmdList[1] == "pause" ) {
            if( cmdList[2] == "0" ) {  //  play
                UpdatePlayer( mac, MODESTATUS, "pause 0" );
                DEBUGF("PLAY");
            } else if( cmdList[2] == "1") {
                UpdatePlayer( mac, MODESTATUS, "pause 1" );
                DEBUGF("PAUSE");
            }

        } else if( cmdList[1] == "newsong" ) {
            DEBUGF("new song");

        } else if( cmdList[1] == "repeat" ) {
            if( cmdList[2] == "0" ) {
                DEBUGF("REPEAT OFF");
            } else if( cmdList[2] == "1" ) {
                DEBUGF("REPEAT SONG");
            } else if( cmdList[2] == "2" ) {
                DEBUGF("REPEAT PLAYLIST");
            }

        } else if( cmdList[1] == "shuffle" ) {
            if( cmdList[2] == "0" ) {
                DEBUGF("SHUFFLE OFF");
            } else if( cmdList[2] == "1" ) {
                DEBUGF("SHUFFLE BY SONG");
            } else if( cmdList[2] == "2" ) {
                DEBUGF("SHUFFLE BY PLAYLIST");
            }

        } else if( cmdList[1] == "stop" ) {
            DEBUGF("STOP");
        } else {
            DEBUGF("UNKNOWN PLAYLIST COMMAND" << cmdList);
        }

    } else if( cmdList[0] == "mixer" ) {
        DEBUGF("MIXER COMMAND" << cmdList );

    } else if( cmdList[0] == "pause" ) {
        DEBUGF("PAUSE COMMAND" << cmdList );

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


Slimp3Player *SqueezeServer::curPlayer()
{
    return m_curPlayer;
}

void SqueezeServer::pauseButton(QString mac, int val)
{
    DEBUGF("PAUSEBUTTON PUSHED WITH:" << val << mac);
    if( val == 1 )
        m_slimp3CLI->sendStandardCommand(C_PAUSE, mac.toLatin1());
    else
        m_slimp3CLI->sendStandardCommand(C_PLAY, mac.toLatin1());
}

void SqueezeServer::setCurPlayer(Slimp3Player *newCurPlayer)
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
