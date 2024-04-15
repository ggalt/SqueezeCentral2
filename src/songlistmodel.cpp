#include "songlistmodel.h"
#include "squeezedefines.h"

#ifdef SONGMODEL_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif

SongListModel::SongListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void SongListModel::addSong(SongObject *item)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_list << item;
    endInsertRows();
    DEBUGF("ADDED SONG" << item->Album() << item->Title() << "COUNT IS:" << rowCount() );
}

int SongListModel::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant SongListModel::data(const QModelIndex &index, int role) const
{
    if( index.row() < 0 || index.row() >= m_list.size() ) {
        return QVariant();
    }

    const SongObject *item = m_list[ index.row() ];

    if( role == SongTitle )
        return item->Title();
    else if( role == AlbumTitle )
        return item->Album();
    else if( role == ArtistName )
        return item->Artist();
    else if( role == Duration )
        return item->Duration();
    else if( role == TrackNumber )
        return item->TrackNumber();
    else if( role == TrackObject ) {
        QVariant retVal;
        retVal.setValue(item);
        return retVal;
    }
    else
        return QVariant();
}

void SongListModel::appendRow(SongObject *item)
{
    appendRows(QList<SongObject*>() << item);
}

void SongListModel::appendRows(const QList<SongObject *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount()+items.size()-1);
    foreach(SongObject *item, items) {
        m_list.append(item);
    }
    endInsertRows();
}

void SongListModel::insertRow(int row, SongObject *item)
{
    beginInsertRows(QModelIndex(), row, row);
    m_list.insert(row, item);
    endInsertRows();
}

bool SongListModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_list.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    endRemoveRows();
    return true;
}

bool SongListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || (row+count) > m_list.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row+count-1);
    for(int i=0; i<count; ++i) {
        delete m_list.takeAt(row);
    }
    endRemoveRows();
    return true;
}

SongObject *SongListModel::takeRow(int row)
{
    if(row < 0 || row >= m_list.size() || m_list.isEmpty())
        return (SongObject *)nullptr;
    beginRemoveRows(QModelIndex(), row, row);
    SongObject* item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}

SongObject *SongListModel::find(const QString &name) const
{
    if(m_list.isEmpty())
        return (SongObject *)nullptr;
    foreach(SongObject* item, m_list) {
        if(item->Title() == name)
            return item;
    }
    return (SongObject *)nullptr;
}

SongObject *SongListModel::getRow(int row)
{
    if(row < 0 || row >= m_list.size() || m_list.isEmpty())
        return (SongObject *)nullptr;
    SongObject *item=m_list.at(row);
    return item;
}

QModelIndex SongListModel::indexFromItem(const SongObject *item) const
{
    Q_ASSERT(item);
    if(m_list.isEmpty())
        return QModelIndex();
    for(int row=0; row<m_list.size(); ++row) {
        if(m_list.at(row) == item)
            return index(row);
    }
    return QModelIndex();
}

void SongListModel::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}

QHash<int, QByteArray> SongListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SongTitle] = "SongTitle";
    roles[ArtistName] = "ArtistName";
    roles[AlbumTitle] = "AlbumTitle";
    roles[Duration] = "Duration";
    roles[TrackNumber] = "TrackNumber";
    return roles;
}

SongObject::SongObject(QObject *parent)
    : QObject(parent)
{

}


QString SongObject::Album() const
{
    return m_Album;
}

void SongObject::setAlbum(const QString &newAlbum)
{
    if (m_Album == newAlbum)
        return;
    m_Album = newAlbum;
    emit AlbumChanged();
}

QString SongObject::Artist() const
{
    return m_Artist;
}

void SongObject::setArtist(const QString &newArtist)
{
    if (m_Artist == newArtist)
        return;
    m_Artist = newArtist;
    emit ArtistChanged();
}

QString SongObject::artworkID() const
{
    return m_artworkID;
}

void SongObject::setArtworkID(const QString &newArtworkID)
{
    if (m_artworkID == newArtworkID)
        return;
    m_artworkID = newArtworkID;
    emit artworkIDChanged();
}

QString SongObject::coverID() const
{
    return m_coverID;
}

void SongObject::setCoverID(const QString &newCoverID)
{
    if (m_coverID == newCoverID)
        return;
    m_coverID = newCoverID;
    emit coverIDChanged();
}

double SongObject::Duration() const
{
    return m_Duration;
}

void SongObject::setDuration(double newDuration)
{
    if (qFuzzyCompare(m_Duration, newDuration))
        return;
    m_Duration = newDuration;
    emit DurationChanged();
}

int SongObject::songID() const
{
    return m_SongID;
}

void SongObject::setSongID(int newSongID)
{
    if (m_SongID == newSongID)
        return;
    m_SongID = newSongID;
    emit songIDChanged();
}

int SongObject::PlayListIndex() const
{
    return m_PlayListIndex;
}

void SongObject::setPlayListIndex(int newPlayListIndex)
{
    if (m_PlayListIndex == newPlayListIndex)
        return;
    m_PlayListIndex = newPlayListIndex;
    emit PlayListIndexChanged();
}

QString SongObject::Title() const
{
    return m_Title;
}

void SongObject::setTitle(const QString &newTitle)
{
    if (m_Title == newTitle)
        return;
    m_Title = newTitle;
    emit TitleChanged();
}

QString SongObject::TrackNumber() const
{
    return m_TrackNumber;
}

void SongObject::setTrackNumber(const QString &newTrackNumber)
{
    if (m_TrackNumber == newTrackNumber)
        return;
    m_TrackNumber = newTrackNumber;
    emit TrackNumberChanged();
}
