#include "slimp3player2.h"
#include "songlistmodel.h"
#include <QDebug>
#include <QRegularExpression>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QQmlContext>
#include <QStringList>
#include "squeezedefines.h"

#ifdef SLIMP3PLAYER_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif


Slimp3Player2::Slimp3Player2(QObject *parent)
    : QObject{parent}
{
    // m_songListModel = new SongListModel(this);
}

QString Slimp3Player2::macAddress() const
{
    return m_macAddress;
}

void Slimp3Player2::setMacAddress(const QString &newMacAddress)
{
    if (m_macAddress == newMacAddress)
        return;
    m_macAddress = newMacAddress;
    emit macAddressChanged();
}

QString Slimp3Player2::playerName() const
{
    return m_playerName;
}

void Slimp3Player2::setPlayerName(const QString &newPlayerName)
{
    if (m_playerName == newPlayerName)
        return;
    m_playerName = newPlayerName;
    emit playerNameChanged();
}

QString Slimp3Player2::playerIP() const
{
    return m_playerIP;
}

void Slimp3Player2::setPlayerIP(const QString &newPlayerIP)
{
    if (m_playerIP == newPlayerIP)
        return;
    m_playerIP = newPlayerIP;
    emit playerIPChanged();
}

int Slimp3Player2::playerID() const
{
    return m_playerID;
}

void Slimp3Player2::setPlayerID(int newPlayerID)
{
    if (m_playerID == newPlayerID)
        return;
    m_playerID = newPlayerID;
    emit playerIDChanged();
}

int Slimp3Player2::playerIndex() const
{
    return m_playerIndex;
}

void Slimp3Player2::setPlayerIndex(int newPlayerIndex)
{
    if (m_playerIndex == newPlayerIndex)
        return;
    m_playerIndex = newPlayerIndex;
    emit playerIndexChanged();
}

QString Slimp3Player2::playerMode() const
{
    return m_playerMode;
}

void Slimp3Player2::setPlayerMode(const QString &newPlayerMode)
{
    if (m_playerMode == newPlayerMode)
        return;
    m_playerMode = newPlayerMode;
    if(m_playerMode == "pause" )
        setIsPlaying(false);
    else
        setIsPlaying(true);
    emit playerModeChanged();
}

bool Slimp3Player2::isPlaying() const
{
    return m_isPlaying;
}

void Slimp3Player2::setIsPlaying(bool newIsPlaying)
{
    if (m_isPlaying == newIsPlaying)
        return;
    m_isPlaying = newIsPlaying;
    emit isPlayingChanged();
}

QString Slimp3Player2::playlistMode() const
{
    return m_playlistMode;
}

void Slimp3Player2::setPlaylistMode(const QString &newPlaylistMode)
{
    if (m_playlistMode == newPlaylistMode)
        return;
    m_playlistMode = newPlaylistMode;
    emit playlistModeChanged();
}

int Slimp3Player2::repeatPlaylist() const
{
    return m_repeatPlaylist;
}

void Slimp3Player2::setRepeatPlaylist(int newRepeatPlaylist)
{
    if (m_repeatPlaylist == newRepeatPlaylist)
        return;
    m_repeatPlaylist = newRepeatPlaylist;
    emit repeatPlaylistChanged();
}

int Slimp3Player2::shufflePlaylist() const
{
    return m_shufflePlaylist;
}

void Slimp3Player2::setShufflePlaylist(int newShufflePlaylist)
{
    if (m_shufflePlaylist == newShufflePlaylist)
        return;
    m_shufflePlaylist = newShufflePlaylist;
    emit shufflePlaylistChanged();
}

int Slimp3Player2::playlistCurIndex() const
{
    return m_playlistCurIndex;
}

void Slimp3Player2::setPlaylistCurIndex(int newPlaylistCurIndex)
{
    if (m_playlistCurIndex == newPlaylistCurIndex)
        return;
    m_playlistCurIndex = newPlaylistCurIndex;
    if( m_songListModel != nullptr ) {
        setCurrentSong( songListModel()->getRow(m_playlistCurIndex));
        DEBUGF("setting current song to:" << currentSong()->Title());
        setSongProgress(0.0);
        setSongDuration(currentSong()->Duration());
        setCoverArtSource(currentSong()->artworkID());
        setCurrentArtist(currentSong()->Artist());
        setCurrentAlbum(currentSong()->Album());
        setCurrentTitle(currentSong()->Title());

    } else {
        DEBUGF("SONG LIST MODEL IS NULL");
        setSongListModel(new SongListModel(this));
        m_songListModel->clear();
    }

    emit playlistCurIndexChanged();
}

int Slimp3Player2::playlistTrackCount() const
{
    return m_playlistTrackCount;
}

void Slimp3Player2::setPlaylistTrackCount(int newPlaylistTrackCount)
{
    if (m_playlistTrackCount == newPlaylistTrackCount)
        return;
    m_playlistTrackCount = newPlaylistTrackCount;
    emit playlistTrackCountChanged();
}

SongListModel *Slimp3Player2::songListModel() const
{
    return m_songListModel;
}

void Slimp3Player2::setSongListModel(SongListModel *newSongListModel)
{
    if (m_songListModel == newSongListModel)
        return;
    m_songListModel = newSongListModel;
    emit songListModelChanged();
}

SongObject *Slimp3Player2::currentSong() const
{
    return m_currentSong;
}

void Slimp3Player2::setCurrentSong(SongObject *newCurrentSong)
{
    if (m_currentSong == newCurrentSong)
        return;
    m_currentSong = newCurrentSong;
    emit currentSongChanged();
}

void Slimp3Player2::setCurrentSong()
{
    setCurrentSong( songListModel()->getRow(playlistCurIndex()) );
}

double Slimp3Player2::songProgress() const
{
    return m_songProgress;
}

void Slimp3Player2::setSongProgress(double newSongProgress)
{
    if (qFuzzyCompare(m_songProgress, newSongProgress))
        return;
    m_songProgress = newSongProgress;
}

void Slimp3Player2::tickSongProgress()
{
    m_songProgress += 1;
    emit songProgressChanged();
}

int Slimp3Player2::mixerVolume() const
{
    return m_mixerVolume;
}

void Slimp3Player2::setMixerVolume(int newMixerVolume)
{
    if (m_mixerVolume == newMixerVolume)
        return;
    m_mixerVolume = newMixerVolume;
    emit mixerVolumeChanged();
}

void Slimp3Player2::UpdatePlayerValues(Slimp3Player2 *updatedPlayer)
{

}

void Slimp3Player2::UpdatePlayerValues(QJsonDocument &doc)
{
    // result section
    // DEBUGF("JSON DOC:" << doc);
    QJsonValue playerResult(doc["result"]);
    setMixerVolume( playerResult["mixer volume"].toInt(-1) );
    setPlayerMode( playerResult["mode"].toString("pause") );
    setPlayerName( playerResult["player_name"].toString("name-error") );
    setPlayerIP( playerResult["player_ip"].toString("bad-IP-address") );
    setPlaylistMode( playerResult["playlist mode"].toString("off") );
    if(  playerResult["playlist_cur_index"].isString() ) {
        int idx = playerResult["playlist_cur_index"].toString("0").toInt();
        setPlaylistCurIndex(idx);
    } else
        setPlaylistCurIndex( playerResult["playlist_cur_index"].toInt(-1) );
    setRepeatPlaylist( playerResult["playlist repeat"].toInt(0) );
    setShufflePlaylist( playerResult["playlist shuffle"].toInt(0) );
    setSongProgress( playerResult["time"].toDouble() );
    // DEBUGF("song progress info:" << playerResult["time"].toDouble() << playerResult["time"].toString() );

    if( m_playerMode == "play" )
        setIsPlaying( true );
    else
        setIsPlaying( false );

    // playlist_loop
    QJsonArray playlistLoop = playerResult["playlist_loop"].toArray();
    int playlistCount = playlistLoop.count();
    setPlaylistTrackCount( playlistCount );

    if( m_songListModel == nullptr )
        this->setSongListModel(new SongListModel(this));
    m_songListModel->clear();

    for(int i = 0; i < playlistCount; i++ ) {
        QJsonObject item = playlistLoop[i].toObject();

        DEBUGF("SETTING SONG INFO FOR TRACK" << i << "OF ALBUM" << item["album"].toString(nullptr));

        SongObject *songObj = new SongObject(this);
        songObj->setAlbum( item["album"].toString(nullptr) );
        songObj->setArtist( item["artist"].toString(nullptr) );
        songObj->setArtworkID( item["artwork_track_id"].toString(nullptr) );
        songObj->setDuration( item["duration"].toDouble(-1) );
        // DEBUGF("SONG DURATION:" << item["duration"].toDouble(-1) << item["duration"].toString());
        songObj->setSongID( item["id"].toInt(0) );
        songObj->setPlayListIndex( item["playlist index"].toInt(-1) );
        songObj->setTitle( item["title"].toString("no title") );
        songObj->setTrackNumber( item["tracknum"].toString("0") );
        m_songListModel->addSong(songObj);
        DEBUGF("TEMPORARY SONG LIST MODEL SIZE:" << songListModel()->rowCount() << "or" << m_songListModel->rowCount());
    }
    DEBUGF("SONG LIST MODEL SIZE:" << songListModel()->rowCount());
    setCurrentSong();
}

void Slimp3Player2::UpdatePlayerPlaylistIndex()
{

}

void Slimp3Player2::Tick()
{
    if( isPlaying() )
        tickSongProgress();   // increment song progress by 1 second
}


QString Slimp3Player2::coverArtSource() const
{
    if( m_currentSong == nullptr ) {
        DEBUGF("CURRENT SONG POINT IS NULL");
        return "";
    }
    return currentSong()->artworkID();
}

QString Slimp3Player2::currentArtist() const
{
    if( m_currentSong == nullptr )
        return "";
    return currentSong()->Artist();
}

QString Slimp3Player2::currentAlbum() const
{
    if( m_currentSong == nullptr )
        return "";
    return currentSong()->Album();
}

QString Slimp3Player2::currentTitle() const
{
    if( m_currentSong == nullptr )
        return "";
    return currentSong()->Title();
}

double Slimp3Player2::songDuration() const
{
    if( m_currentSong == nullptr )
        return 0.0;
    return currentSong()->Duration();
}

void Slimp3Player2::setSongDuration(double newSongDuration)
{
    if (qFuzzyCompare(m_songDuration, newSongDuration))
        return;
    m_songDuration = newSongDuration;
    DEBUGF("SONG DURATION OF PLAYER:" << m_songDuration);
    emit songDurationChanged();
}

void Slimp3Player2::setCoverArtSource(const QString &newCoverArtSource)
{
    if (m_coverArtSource == newCoverArtSource)
        return;
    m_coverArtSource = newCoverArtSource;
    emit coverArtSourceChanged();
}

void Slimp3Player2::setCurrentArtist(const QString &newCurrentArtist)
{
    if (m_currentArtist == newCurrentArtist)
        return;
    m_currentArtist = newCurrentArtist;
    emit currentArtistChanged();
}

void Slimp3Player2::setCurrentAlbum(const QString &newCurrentAlbum)
{
    if (m_currentAlbum == newCurrentAlbum)
        return;
    m_currentAlbum = newCurrentAlbum;
    emit currentAlbumChanged();
}

void Slimp3Player2::setCurrentTitle(const QString &newCurrentTitle)
{
    if (m_currentTitle == newCurrentTitle)
        return;
    m_currentTitle = newCurrentTitle;
    emit currentTitleChanged();
}
