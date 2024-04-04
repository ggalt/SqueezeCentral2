#ifndef SONGLISTMODEL_H
#define SONGLISTMODEL_H

#include <QAbstractListModel>
#include <QObject>

//////////////////////////////////////////////////////////////////
/// \brief The SongObject class
///

class SongObject : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString Album READ Album WRITE setAlbum NOTIFY AlbumChanged FINAL)
    Q_PROPERTY(QString Artist READ Artist WRITE setArtist NOTIFY ArtistChanged FINAL)
    Q_PROPERTY(QString artworkID READ artworkID WRITE setArtworkID NOTIFY artworkIDChanged FINAL)
    Q_PROPERTY(QString coverID READ coverID WRITE setCoverID NOTIFY coverIDChanged FINAL)
    Q_PROPERTY(double Duration READ Duration WRITE setDuration NOTIFY DurationChanged FINAL)
    Q_PROPERTY(int SongID READ songID WRITE setSongID NOTIFY songIDChanged FINAL)
    Q_PROPERTY(int PlayListIndex READ PlayListIndex WRITE setPlayListIndex NOTIFY PlayListIndexChanged FINAL)
    Q_PROPERTY(QString Title READ Title WRITE setTitle NOTIFY TitleChanged FINAL)
    Q_PROPERTY(QString TrackNumber READ TrackNumber WRITE setTrackNumber NOTIFY TrackNumberChanged FINAL)

public:
    SongObject( QObject *parent = nullptr);

    QString Album() const;
    void setAlbum(const QString &newAlbum);

    QString Artist() const;
    void setArtist(const QString &newArtist);

    QString artworkID() const;
    void setArtworkID(const QString &newArtworkID);

    QString coverID() const;
    void setCoverID(const QString &newCoverID);

    double Duration() const;
    void setDuration(double newDuration);

    int songID() const;
    void setSongID(int newSongID);

    int PlayListIndex() const;
    void setPlayListIndex(int newPlayListIndex);

    QString Title() const;
    void setTitle(const QString &newTitle);

    QString TrackNumber() const;
    void setTrackNumber(const QString &newTrackNumber);

signals:

    void AlbumChanged();
    void ArtistChanged();
    void artworkIDChanged();
    void coverIDChanged();
    void DurationChanged();
    void songIDChanged();
    void PlayListIndexChanged();
    void TitleChanged();
    void TrackNumberChanged();

private:
    QString m_Album;
    QString m_Artist;
    QString m_artworkID;
    QString m_coverID;
    double m_Duration;
    int m_SongID;
    int m_PlayListIndex;
    QString m_Title;
    QString m_TrackNumber;
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
