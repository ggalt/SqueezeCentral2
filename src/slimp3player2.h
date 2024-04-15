#ifndef SLIMP3PLAYER2_H
#define SLIMP3PLAYER2_H

#include <QQuickItem>
#include <QString>
#include <QByteArray>
#include "songlistmodel.h"
#include <QObject>
#include <QTimer>

class Slimp3Player2 : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString macAddress READ macAddress WRITE setMacAddress NOTIFY macAddressChanged FINAL)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged FINAL)
    Q_PROPERTY(QString playerIP READ playerIP WRITE setPlayerIP NOTIFY playerIPChanged FINAL)
    Q_PROPERTY(int playerID READ playerID WRITE setPlayerID NOTIFY playerIDChanged FINAL)
    Q_PROPERTY(int playerIndex READ playerIndex WRITE setPlayerIndex NOTIFY playerIndexChanged FINAL)

    Q_PROPERTY(QString playerMode READ playerMode WRITE setPlayerMode NOTIFY playerModeChanged FINAL)
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged FINAL)
    Q_PROPERTY(QString playlistMode READ playlistMode WRITE setPlaylistMode NOTIFY playlistModeChanged FINAL)
    Q_PROPERTY(int repeatPlaylist READ repeatPlaylist WRITE setRepeatPlaylist NOTIFY repeatPlaylistChanged FINAL)
    Q_PROPERTY(int shufflePlaylist READ shufflePlaylist WRITE setShufflePlaylist NOTIFY shufflePlaylistChanged FINAL)
    Q_PROPERTY(int playlistCurIndex READ playlistCurIndex WRITE setPlaylistCurIndex NOTIFY playlistCurIndexChanged FINAL)
    Q_PROPERTY(int playlistTrackCount READ playlistTrackCount WRITE setPlaylistTrackCount NOTIFY playlistTrackCountChanged FINAL)

    Q_PROPERTY(SongListModel* songListModel READ songListModel WRITE setSongListModel NOTIFY songListModelChanged FINAL)
    Q_PROPERTY(SongObject* currentSong READ currentSong WRITE setCurrentSong NOTIFY currentSongChanged FINAL)
    Q_PROPERTY(double songProgress READ songProgress WRITE setSongProgress NOTIFY songProgressChanged FINAL)
    Q_PROPERTY(double songDuration READ songDuration WRITE setSongDuration NOTIFY songDurationChanged FINAL)

    Q_PROPERTY(int mixerVolume READ mixerVolume WRITE setMixerVolume NOTIFY mixerVolumeChanged FINAL)
    Q_PROPERTY(QString coverArtSource READ coverArtSource CONSTANT)
    Q_PROPERTY(QString currentArtist READ currentArtist CONSTANT)
    Q_PROPERTY(QString currentAlbum READ currentAlbum CONSTANT)
    Q_PROPERTY(QString currentTitle READ currentTitle CONSTANT)

public:
    explicit Slimp3Player2(QObject *parent = nullptr);

    QString macAddress() const;
    void setMacAddress(const QString &newMacAddress);

    QString playerName() const;
    void setPlayerName(const QString &newPlayerName);

    QString playerIP() const;
    void setPlayerIP(const QString &newPlayerIP);

    int playerID() const;
    void setPlayerID(int newPlayerID);

    int playerIndex() const;
    void setPlayerIndex(int newPlayerIndex);

    QString playerMode() const;
    void setPlayerMode(const QString &newPlayerMode);

    bool isPlaying() const;
    void setIsPlaying(bool newIsPlaying);

    QString playlistMode() const;
    void setPlaylistMode(const QString &newPlaylistMode);

    int repeatPlaylist() const;
    void setRepeatPlaylist(int newRepeatPlaylist);

    int shufflePlaylist() const;
    void setShufflePlaylist(int newShufflePlaylist);

    int playlistCurIndex() const;
    void setPlaylistCurIndex(int newPlaylistCurIndex);

    int playlistTrackCount() const;
    void setPlaylistTrackCount(int newPlaylistTrackCount);

    SongListModel *songListModel() const;
    void setSongListModel(SongListModel *newSongListModel);

    SongObject *currentSong() const;
    void setCurrentSong(SongObject *newCurrentSong);
    void setCurrentSong(void);

    double songProgress() const;
    void setSongProgress(double newSongProgress);
    void tickSongProgress( void );

    int mixerVolume() const;
    void setMixerVolume(int newMixerVolume);

    QString coverArtSource() const;
    QString currentArtist() const;
    QString currentAlbum() const;
    QString currentTitle() const;

    double songDuration() const;
    void setSongDuration(double newSongDuration);

public slots:
    void UpdatePlayerValues( Slimp3Player2 *updatedPlayer);
    void UpdatePlayerValues( QJsonDocument &doc);
    void UpdatePlayerPlaylistIndex();
    void Tick(void);

signals:
    void sendCLICommand(QByteArray mac, QString command);
    void sendJSONCommand(QByteArray mac, QString command);

    void macAddressChanged();
    void playerNameChanged();
    void playerIPChanged();
    void playerIDChanged();
    void playerIndexChanged();
    void playerModeChanged();
    void isPlayingChanged();
    void playlistModeChanged();
    void repeatPlaylistChanged();
    void shufflePlaylistChanged();
    void playlistCurIndexChanged();
    void playlistTrackCountChanged();
    void songListModelChanged();
    void currentSongChanged();
    void songProgressChanged();
    void mixerVolumeChanged();

    void songDurationChanged();

private:
    QString m_macAddress = nullptr;
    QString m_playerName = nullptr;
    QString m_playerIP = nullptr;
    int m_playerID = 0;
    int m_playerIndex = 0;
    QString m_playerMode = nullptr;
    bool m_isPlaying = false;
    QString m_playlistMode = nullptr;
    int m_repeatPlaylist = 0;
    int m_shufflePlaylist = 0;
    int m_playlistCurIndex = 0;
    int m_playlistTrackCount = 0;
    SongListModel *m_songListModel = nullptr;
    SongObject *m_currentSong = nullptr;
    double m_songProgress = 0.0;
    int m_mixerVolume = 0;

    QString m_coverArtSource;
    QString m_currentArtist;
    QString m_currentAlbum;
    QString m_currentTitle;
    double m_songDuration;
};

#endif // SLIMP3PLAYER2_H
