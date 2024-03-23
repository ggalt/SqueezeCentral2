#include "slimp3player.h"
#include "songlistmodel.h"

#ifdef SLIMP3PLAYER_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif

Slimp3Player::Slimp3Player(QObject *parent)
    : QObject(parent)
{
    m_songListModel = new SongListModel(this);
    m_macAddress = "";
    m_playerName = "";
    m_connectedStatus = 0;
    m_playerIP = "";
    bool m_isPlaying = false;
    int m_volume = 0;
    m_currentPlaylistIndex = "";
    m_playlistTrackCount = 0;
    m_coverArtSource = "";
    m_currentArtist = "";
    m_currentAlbum = "";
    m_currentTitle = "";
    m_playerIndex = "";
    m_songDuration = "";
    m_elapsedTime = "";
    m_songDurationInt = 0;
    m_elapsedTimeInt = 0;
}

void Slimp3Player::UpdatePlayerValues(Slimp3Player *updatedPlayer)
{
    DEBUGF("UPDATING PLAYER:" << this->macAddress());
    DEBUGF("UPDATED INFO:" << updatedPlayer);
    if( !updatedPlayer->currentPlaylistIndex().isEmpty() )
        m_currentPlaylistIndex = updatedPlayer->currentPlaylistIndex();
    if( !updatedPlayer->macAddress().isEmpty() )
        m_macAddress = updatedPlayer->macAddress();
    if( !updatedPlayer->playerName().isEmpty() )
        m_playerName = updatedPlayer->playerName();
    if( !updatedPlayer->playerIP().isEmpty() )
        m_playerIP = updatedPlayer->playerIP();
    // if( !updatedPlayer->playerMode().isEmpty() )
    //     m_playerMode = updatedPlayer->playerMode();
    if( !updatedPlayer->songDuration().isEmpty())
        m_songDuration = updatedPlayer->songDuration();
    if( updatedPlayer->volume() > -1 )
        m_volume = updatedPlayer->volume();
    if( updatedPlayer->repeatStatus() != MAX_REPEAT_MODES )
        this->setRepeatStatus(updatedPlayer->repeatStatus());
    if( updatedPlayer->shuffleStatus() != MAX_SHUFFLE_MODES )
        this->setShuffleStatus(updatedPlayer->shuffleStatus());
    if( !updatedPlayer->currentPlaylistIndex().isEmpty() )
        m_currentPlaylistIndex = updatedPlayer->currentPlaylistIndex();
    if( updatedPlayer->playlistTrackCount() > -1 )
        m_playlistTrackCount = updatedPlayer->playlistTrackCount();
    m_songListModel = updatedPlayer->songListModel();

    UpdatePlayerPlaylistIndex();
}

void Slimp3Player::UpdatePlayerPlaylistIndex()
{
    SongObject *s = getCurrentSong();

    if( s != nullptr ) {
        m_coverArtSource = s->artworkID();
        m_currentArtist = s->Artist();
        m_currentAlbum = s->Album();
        m_currentTitle = s->Title();
    } else {
        m_coverArtSource = "";
        m_currentArtist = "empty";
        m_currentAlbum = "empty";
        m_currentTitle = "empty";
    }
}

void Slimp3Player::Tick()
{
    // receive signal at 1 second intervals
    if(!isPlaying())
        return;
    setElapsedTime(m_elapsedTimeInt + 1);
}

SongObject *Slimp3Player::getCurrentSong()
{
    int currentIndex = currentPlaylistIndex().toInt();
    return songListModel()->getRow(currentIndex);
}

QString Slimp3Player::macAddress() const
{
    return m_macAddress;
}

void Slimp3Player::setMacAddress(const QString &newMacAddress)
{
    if (m_macAddress == newMacAddress)
        return;
    m_macAddress = newMacAddress;
    emit macAddressChanged();
}

QString Slimp3Player::playerName() const
{
    return m_playerName;
}

void Slimp3Player::setPlayerName(const QString &newPlayerName)
{
    if (m_playerName == newPlayerName)
        return;
    m_playerName = newPlayerName;
    emit playerNameChanged();
}

int Slimp3Player::connectedStatus() const
{
    return m_connectedStatus;
}

void Slimp3Player::setConnectedStatus(int newConnectedStatus)
{
    if (m_connectedStatus == newConnectedStatus)
        return;
    m_connectedStatus = newConnectedStatus;
    emit connectedStatusChanged();
}

QString Slimp3Player::playerIP() const
{
    return m_playerIP;
}

void Slimp3Player::setPlayerIP(const QString &newPlayerIP)
{
    if (m_playerIP == newPlayerIP)
        return;
    m_playerIP = newPlayerIP;
    emit playerIPChanged();
}

bool Slimp3Player::isPlaying() const
{
    DEBUGF("PLAYER MODE IS:" << m_playerMode <<":" << playerName());
    return m_isPlaying;
}

void Slimp3Player::setIsPlaying(bool newIsPlaying)
{
    if (m_isPlaying == newIsPlaying)
        return;
    m_isPlaying = newIsPlaying;
    emit isPlayingChanged();
}

QString Slimp3Player::songDuration() const
{
    return m_songDuration;
}

void Slimp3Player::setSongDuration(QString newSongDuration)
{
    int duration = newSongDuration.toInt();
    setSongDuration(duration);
}

void Slimp3Player::setSongDuration(int duration)
{
    DEBUGF("SETTING SONG DURATION:" << duration << "current duration is:" << m_songDurationInt << "Player:" << playerName());
    if( m_songDurationInt == duration )
        return;
    m_songDurationInt = duration;

    int minutes = m_songDurationInt / 60;
    int seconds = m_songDurationInt % 60;

    m_songDuration = QString("%1:%2")
                         .arg(minutes, 2, 10, QChar('0'))
                         .arg(seconds, 2, 10, QChar('0'));


    DEBUGF("SONG DURATION IS NOW:" << m_songDuration << m_songDurationInt << this->currentProgress());
    emit songDurationChanged();
}

QString Slimp3Player::elapsedTime() const
{
    return m_elapsedTime;
}

void Slimp3Player::setElapsedTime(const QString &newElapsedTime)
{
    int duration = newElapsedTime.toInt();
    setElapsedTime(duration);
}

void Slimp3Player::setElapsedTime(const int elapsedTime)  // assume # of seconds
{
    if( m_elapsedTimeInt == elapsedTime )
        return;
    m_elapsedTimeInt = elapsedTime;

    int minutes = m_elapsedTimeInt / 60;
    int seconds = m_elapsedTimeInt % 60;

    m_elapsedTime = QString("%1:%2")
                         .arg(minutes, 2, 10, QChar('0'))
                         .arg(seconds, 2, 10, QChar('0'));


    emit elapsedTimeChanged();
    DEBUGF("ELAPSED TIME:" << m_elapsedTime << m_elapsedTimeInt);
    DEBUGF("PLAYER INFO:" << "1" << m_playerMode << "2" << m_volume << "3" << m_repeatStatus << "4" << m_shuffleStatus
                          << "5" << m_playlistTrackCount << "6" << m_currentPlaylistIndex << "7" << m_playerName);
}

int Slimp3Player::volume() const
{
    return m_volume;
}

void Slimp3Player::setVolume(int newVolume)
{
    if (m_volume == newVolume)
        return;
    m_volume = newVolume;
    emit volumeChanged();
}

QString Slimp3Player::currentPlaylistIndex() const
{
    return m_currentPlaylistIndex;
}

void Slimp3Player::setCurrentPlaylistIndex(const QString &newCurrentPlaylistIndex)
{
    if (m_currentPlaylistIndex == newCurrentPlaylistIndex)
        return;
    m_currentPlaylistIndex = newCurrentPlaylistIndex;
    emit currentPlaylistIndexChanged();
}

int Slimp3Player::playlistTrackCount() const
{
    return m_playlistTrackCount;
}

void Slimp3Player::setPlaylistTrackCount(int newPlaylistTrackCount)
{
    if (m_playlistTrackCount == newPlaylistTrackCount)
        return;
    m_playlistTrackCount = newPlaylistTrackCount;
    emit playlistTrackCountChanged();
}

SongListModel *Slimp3Player::songListModel() const
{
    return m_songListModel;
}

void Slimp3Player::setSongListModel(SongListModel *newSongListModel)
{
    if (m_songListModel == newSongListModel)
        return;
    m_songListModel = newSongListModel;
    emit songListModelChanged();
}

QString Slimp3Player::coverArtSource() const
{
    return m_coverArtSource;
}

QString Slimp3Player::currentArtist() const
{
    return m_currentArtist;
}

QString Slimp3Player::currentAlbum() const
{
    return m_currentAlbum;
}

QString Slimp3Player::currentTitle() const
{
    return m_currentTitle;
}

int Slimp3Player::currentProgress() const
{
    int progress = 0;
    if( m_songDurationInt > 0 )
        progress = m_elapsedTimeInt / m_songDurationInt;
    DEBUGF("PLAYER SONG PROGRESS:" << progress << m_elapsedTimeInt << m_songDurationInt);
    return progress;
}

QString Slimp3Player::playerIndex() const
{
    return m_playerIndex;
}

void Slimp3Player::setPlayerIndex(const QString &newPlayerIndex)
{
    if (m_playerIndex == newPlayerIndex)
        return;
    m_playerIndex = newPlayerIndex;
    emit playerIndexChanged();
}

playMode Slimp3Player::playerMode() const
{
    return m_playerMode;
}

void Slimp3Player::setPlayerMode(int newPlayerMode)
{
    playMode newPlMode;
    if( newPlayerMode == 0)
        newPlMode = PLAY;
    else if( newPlayerMode == 1 )
        newPlMode = PAUSE;
    else if( newPlayerMode == 2 )
        newPlMode = STOP;
    else
        newPlMode = MAX_PLAY_MODES;

    setPlayerMode(newPlMode);
}

void Slimp3Player::setPlayerMode(playMode newPlayerMode)
{
    if (m_playerMode == newPlayerMode)
        return;
    m_playerMode = newPlayerMode;
    emit playerModeChanged();

}

RepeatMode Slimp3Player::repeatStatus() const
{
    return m_repeatStatus;
}

void Slimp3Player::setRepeatStatus(int newRepeatStatus)
{
    RepeatMode newRepMode;
    if( newRepeatStatus == 0 )
        newRepMode = REPEAT_OFF;
    else if( newRepeatStatus == 1 )
        newRepMode = REPEAT_TRACK;
    else if( newRepeatStatus == 2 )
        newRepMode = REPEAT_PLAYLIST;
    else
        newRepMode = MAX_REPEAT_MODES;

    setRepeatStatus( newRepMode );
}

void Slimp3Player::setRepeatStatus(RepeatMode newRepeatStatus)
{
    if (m_repeatStatus == newRepeatStatus)
        return;
    m_repeatStatus = newRepeatStatus;
    emit repeatStatusChanged();
    DEBUGF("REPEAT STATUS:" << m_repeatStatus);
    DEBUGF("REPEAT VALUES:" << REPEAT_OFF << REPEAT_TRACK << REPEAT_PLAYLIST << MAX_REPEAT_MODES);
}


ShuffleMode Slimp3Player::shuffleStatus() const
{
    return m_shuffleStatus;
}

void Slimp3Player::setShuffleStatus(int newShuffleStatus)
{
    ShuffleMode newShufMode;
    if( newShuffleStatus == 0 )
        newShufMode = SHUFFLE_OFF;
    else     if( newShuffleStatus == 1 )
        newShufMode = SHUFFLE_BY_SONG;
    else     if( newShuffleStatus == 2 )
        newShufMode = SHUFFLE_BY_ALBUM;
    else
        newShufMode = MAX_SHUFFLE_MODES;

    setShuffleStatus(newShufMode);
}

void Slimp3Player::setShuffleStatus(ShuffleMode newShuffleStatus)
{
    if (m_shuffleStatus == newShuffleStatus)
        return;
    m_shuffleStatus = newShuffleStatus;
    emit shuffleStatusChanged();
    DEBUGF("SHUFFLE STATUS:" << m_shuffleStatus);
    DEBUGF("SHUFFLE VALUES:" << SHUFFLE_OFF << SHUFFLE_BY_SONG << SHUFFLE_BY_ALBUM << MAX_SHUFFLE_MODES);
}

