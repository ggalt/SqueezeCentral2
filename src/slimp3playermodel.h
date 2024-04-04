#ifndef SLIMP3PLAYERMODEL_H
#define SLIMP3PLAYERMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include "slimp3player2.h"

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

    void addPlayer(Slimp3Player2 *item);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void UpdateIndex(void);

    void appendRow(Slimp3Player2* item);
    void appendRows(const QList<Slimp3Player2*> &items);
    void insertRow(int row, Slimp3Player2* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    Slimp3Player2* takeRow(int row);
    Slimp3Player2* find(const QString &name) const;
    Slimp3Player2* find(const QByteArray &mac) const;
    Slimp3Player2* getRow(int row);
    QModelIndex indexFromItem( const Slimp3Player2* item) const;
    void clear();

    // QString getPlayerFromMac(QString mac);
    // Slimp3Player2* findPlayerByMAC(QString mac);

signals:
    void modelUpdated();

public slots:
    Slimp3Player2* findPlayerByMAC(QString mac);

    QString getPlayerName(QString mac);
    QString getSongTitle(QString mac);
    QString getArtistName(QString mac);
    QString getAlbumName(QString mac);
    QString getAlbumArt(QString mac);
    double getDuration(QString mac);
    QString getTimeRemaining(QString mac);
    double getProgress(QString mac);


protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Slimp3Player2 *> m_list;
};

#endif // SLIMP3PLAYERMODEL_H
