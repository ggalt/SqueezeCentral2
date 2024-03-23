#ifndef SONGLISTMODEL_H
#define SONGLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

//////////////////////////////////////////////////////////////////
/// \brief The SongObject class
///

class SongObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString Title READ Title WRITE setTitle NOTIFY TitleChanged FINAL)
    Q_PROPERTY(QString Artist READ Artist WRITE setArtist NOTIFY ArtistChanged FINAL)
    Q_PROPERTY(QString Album READ Album WRITE setAlbum NOTIFY AlbumChanged FINAL)
    Q_PROPERTY(QString Duration READ Duration WRITE setDuration NOTIFY DurationChanged FINAL)
    // Q_PROPERTY(float Duration READ Duration WRITE setDuration NOTIFY DurationChanged FINAL)
    Q_PROPERTY(QString TrackNumber READ TrackNumber WRITE setTrackNumber NOTIFY TrackNumberChanged FINAL)
    Q_PROPERTY(QString artworkID READ artworkID WRITE setArtworkID NOTIFY artworkIDChanged FINAL)
    Q_PROPERTY(int PlayListIndex READ PlayListIndex WRITE setPlayListIndex NOTIFY PlayListIndexChanged FINAL)

public:
    SongObject( QObject *parent = nullptr);

    QString Title() const;
    void setTitle(const QString &newTitle);

    QString Artist() const;
    void setArtist(const QString &newArtist);

    QString Album() const;
    void setAlbum(const QString &newAlbum);

    QString Duration() const;
    void setDuration(QString newDuration);

    QString TrackNumber() const;
    void setTrackNumber(const QString &newTrackNumber);

    QString artworkID() const;
    void setArtworkID(const QString &newArtworkID);

    int PlayListIndex() const;
    void setPlayListIndex(int newPlayListIndex);

signals:
    void TitleChanged();
    void ArtistChanged();
    void AlbumChanged();
    void DurationChanged();
    void TrackNumberChanged();
    void artworkIDChanged();
    void PlayListIndexChanged();

private:
    QString m_Title;
    QString m_Artist;
    QString m_Album;
    QString m_Duration;
    QString m_TrackNumber;
    QString m_artworkID;
    int m_PlayListIndex;
};

/////////////////////////////////////////////////////////////////////
/// \brief The SongListModel class
///

class SongListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SongListModelRoles {
        SongTitle = Qt::UserRole + 10,
        ArtistName,
        AlbumTitle,
        Duration,
        TrackNumber,
        TrackObject
    };

    explicit SongListModel(QObject *parent = nullptr);

    void addSong(SongObject *item);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void appendRow(SongObject* item);
    void appendRows(const QList<SongObject*> &items);
    void insertRow(int row, SongObject* item);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    SongObject* takeRow(int row);
    SongObject* find(const QString &name) const;
    SongObject* getRow(int row);
    QModelIndex indexFromItem( const SongObject* item) const;
    void clear();

signals:

    // public slots:
    //     void handleItemChange();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<SongObject *> m_list;

};

#endif // SONGLISTMODEL_H
