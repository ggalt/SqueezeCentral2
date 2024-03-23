#ifndef SLIMP3PLAYER_H
#define SLIMP3PLAYER_H

#include <QQuickItem>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include "songlistmodel.h"
#include "squeezedefines.h"

class Slimp3Player : public QObject //QQuickItem
{
    Q_OBJECT
    // QML_ELEMENT

    Q_PROPERTY(QString macAddress READ macAddress WRITE setMacAddress NOTIFY macAddressChanged FINAL)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged FINAL)
    Q_PROPERTY(int connectedStatus READ connectedStatus WRITE setConnectedStatus NOTIFY connectedStatusChanged FINAL)
    Q_PROPERTY(QString playerIP READ playerIP WRITE setPlayerIP NOTIFY playerIPChanged FINAL)
    Q_PROPERTY(QString playerIndex READ playerIndex WRITE setPlayerIndex NOTIFY playerIndexChanged FINAL)

    Q_PROPERTY(playMode playerMode READ playerMode WRITE setPlayerMode NOTIFY playerModeChanged FINAL)
    Q_PROPERTY(bool isPlaying READ isPlaying WRITE setIsPlaying NOTIFY isPlayingChanged FINAL)
    Q_PROPERTY(QString songDuration READ songDuration WRITE setSongDuration NOTIFY songDurationChanged FINAL)
    Q_PROPERTY(QString elapsedTime READ elapsedTime WRITE setElapsedTime NOTIFY elapsedTimeChanged FINAL)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged FINAL)
    Q_PROPERTY(RepeatMode repeatStatus READ repeatStatus WRITE setRepeatStatus NOTIFY repeatStatusChanged FINAL)
    Q_PROPERTY(ShuffleMode shuffleStatus READ shuffleStatus WRITE setShuffleStatus NOTIFY shuffleStatusChanged FINAL)
    Q_PROPERTY(QString currentPlaylistIndex READ currentPlaylistIndex WRITE setCurrentPlaylistIndex NOTIFY currentPlaylistIndexChanged FINAL)
    Q_PROPERTY(int playlistTrackCount READ playlistTrackCount WRITE setPlaylistTrackCount NOTIFY playlistTrackCountChanged FINAL)
    Q_PROPERTY(SongListModel* songListModel READ songListModel WRITE setSongListModel NOTIFY songListModelChanged FINAL)

    Q_PROPERTY(QString coverArtSource READ coverArtSource CONSTANT)
    Q_PROPERTY(QString currentArtist READ currentArtist CONSTANT)
    Q_PROPERTY(QString currentAlbum READ currentAlbum CONSTANT)
    Q_PROPERTY(QString currentTitle READ currentTitle CONSTANT)

public:
    Slimp3Player(QObject *parent = nullptr);

    Slimp3Player(const Slimp3Player &) = delete;
    Slimp3Player(Slimp3Player &&) = delete;
    Slimp3Player &operator=(const Slimp3Player &) = delete;
    Slimp3Player &operator=(Slimp3Player &&) = delete;
    QString macAddress() const;
    void setMacAddress(const QString &newMacAddress);

    QString playerName() const;
    void setPlayerName(const QString &newPlayerName);

    int connectedStatus() const;
    void setConnectedStatus(int newConnectedStatus);

    QString playerIP() const;
    void setPlayerIP(const QString &newPlayerIP);

    bool isPlaying() const;
    void setIsPlaying(bool newIsPlaying);

    QString songDuration() const;
    void setSongDuration(QString newSongDuration);
    void setSongDuration(int duration);

    QString elapsedTime() const;
    void setElapsedTime(const QString &newElapsedTime);
    void setElapsedTime(const int duration);

    int volume() const;
    void setVolume(int newVolume);

    QString currentPlaylistIndex() const;
    void setCurrentPlaylistIndex(const QString &newCurrentPlaylistIndex);

    int playlistTrackCount() const;
    void setPlaylistTrackCount(int newPlaylistTrackCount);

    SongListModel *songListModel() const;
    void setSongListModel(SongListModel *newSongListModel);

    QString coverArtSource() const;

    QString currentArtist() const;

    QString currentAlbum() const;

    QString currentTitle() const;

    int currentProgress() const;

    QString playerIndex() const;
    void setPlayerIndex(const QString &newPlayerIndex);

    playMode playerMode() const;
    void setPlayerMode(int newPlayerMode);
    void setPlayerMode(playMode newPlayerMode);

    RepeatMode repeatStatus() const;
    void setRepeatStatus(int newRepeatStatus);
    void setRepeatStatus(RepeatMode newRepeatStatus);

    ShuffleMode shuffleStatus() const;
    void setShuffleStatus(int newShuffleStatus);
    void setShuffleStatus(ShuffleMode newShuffleStatus);

public slots:
    void UpdatePlayerValues( Slimp3Player *updatedPlayer);
    void UpdatePlayerPlaylistIndex();
    void Tick(void);

signals:
    void sendCLICommand(QByteArray mac, QString command);
    void sendJSONCommand(QByteArray mac, QString command);

    void macAddressChanged();
    void playerNameChanged();
    void connectedStatusChanged();
    void playerIPChanged();
    void isPlayingChanged();
    void songDurationChanged();
    void volumeChanged();
    void currentPlaylistIndexChanged();
    void playlistTrackCountChanged();
    void songListModelChanged();
    void playerIndexChanged();
    void repeatStatusChanged();
    void playerModeChanged();
    void shuffleStatusChanged();

    void elapsedTimeChanged();

private:
    SongObject *getCurrentSong();
    SongListModel *m_songListModel = nullptr;
    QString m_macAddress;
    QString m_playerName;
    int m_connectedStatus;
    QString m_playerIP;
    playMode m_playerMode;
    bool m_isPlaying;
    int m_volume;
    RepeatMode m_repeatStatus;
    ShuffleMode m_shuffleStatus;
    QString m_currentPlaylistIndex;
    int m_playlistTrackCount;
    QString m_coverArtSource;
    QString m_currentArtist;
    QString m_currentAlbum;
    QString m_currentTitle;
    QString m_playerIndex;
    QString m_songDuration;
    QString m_elapsedTime;
    int m_songDurationInt;
    int m_elapsedTimeInt;
};
Q_DECLARE_METATYPE(Slimp3Player*)
#endif // SLIMP3PLAYER_H
