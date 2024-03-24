#include "slimp3player2.h"

Slimp3Player2::Slimp3Player2(QObject *parent)
    : QObject{parent}
{}

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

double Slimp3Player2::songProgress() const
{
    return m_songProgress;
}

void Slimp3Player2::setSongProgress(double newSongProgress)
{
    if (qFuzzyCompare(m_songProgress, newSongProgress))
        return;
    m_songProgress = newSongProgress;
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

void Slimp3Player2::UpdatePlayerValues(QJsonDocument *doc)
{

}

void Slimp3Player2::UpdatePlayerPlaylistIndex()
{

}

void Slimp3Player2::Tick()
{

}
