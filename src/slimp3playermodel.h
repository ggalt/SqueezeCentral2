#ifndef SLIMP3PLAYERMODEL_H
#define SLIMP3PLAYERMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "slimp3player.h"

class Slimp3PlayerModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Slimp3PlayerRoles {
        NameRole = Qt::UserRole + 1,
        MACRole,
        PlayerRole,
        SongTitle,
        ArtistName,
        AlbumTitle,
        AlbumArt,
        Duration,
        TimeRemaining,
        CurrentProgress,
        PauseStatus,
        RepeatStatus,
        ShuffleStatus
    };

    Slimp3PlayerModel(QObject *parent = nullptr);

    void addPlayer(Slimp3Player *item);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void UpdateIndex(void);

    void appendRow(Slimp3Player* item);
    void appendRows(const QList<Slimp3Player*> &items);
    void insertRow(int row, Slimp3Player* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Slimp3Player* takeRow(int row);
    Slimp3Player* find(const QString &name) const;
    Slimp3Player* find(const QByteArray &mac) const;
    Slimp3Player* getRow(int row);
    QModelIndex indexFromItem( const Slimp3Player* item) const;
    void clear();

    // QString getPlayerFromMac(QString mac);
    // Slimp3Player* findPlayerByMAC(QString mac);

signals:
    void modelUpdated();

public slots:
    Slimp3Player* findPlayerByMAC(QString mac);

    QString getPlayerName(QString mac);
    QString getSongTitle(QString mac);
    QString getArtistName(QString mac);
    QString getAlbumName(QString mac);
    QString getAlbumArt(QString mac);
    QString getDuration(QString mac);
    QString getTimeRemaining(QString mac);
    float getProgress(QString mac);


protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Slimp3Player *> m_list;
};

#endif // SLIMP3PLAYERMODEL_H
