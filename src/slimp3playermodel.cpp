#include "slimp3playermodel.h"
#include "squeezedefines.h"

#ifdef PLAYERMODEL_DEBUG
#define DEBUGF(...) qDebug() << this->objectName() << Q_FUNC_INFO << __VA_ARGS__;
#else
#define DEBUGF(...)
#endif

Slimp3PlayerModel::Slimp3PlayerModel(QObject *parent)  :
    QAbstractListModel(parent) {

}

void Slimp3PlayerModel::addPlayer(Slimp3Player2 *item)
{
    item->setParent(this);
    DEBUGF("adding player with id:" << item->macAddress());

    Slimp3Player2 *existingPlayer = find(item->macAddress().toLatin1());

    if( existingPlayer == nullptr) {
        DEBUGF("PLAYER DOES NOT EXIST");
        beginInsertRows( QModelIndex(), rowCount(), rowCount());
        m_list.append(item);
        // m_list << item;
        endInsertRows();
    } else {
        DEBUGF("EXISTING PLAYER" << existingPlayer->playerName());
        existingPlayer->UpdatePlayerValues(item);
        UpdateIndex();
    }

    // DEBUGF( "PLAYER ARTIST:" << item->currentArtist());
    DEBUGF("adding player with id:" << item->macAddress());
    emit modelUpdated();
}

int Slimp3PlayerModel::rowCount(const QModelIndex &parent) const
{
    return m_list.count();
}

QVariant Slimp3PlayerModel::data(const QModelIndex &index, int role) const
{
    // DEBUGF("Index row:" << index.row() << role)
    if( index.row() < 0 || index.row() >= m_list.size() ) {
        return QVariant();
    }

    Slimp3Player2 *item = m_list[ index.row() ];
    // DEBUGF("SIZE OF PLAYER LIST IS:" << m_list.count());
    // DEBUGF(QString("artist name is:") << item->currentArtist());

    DEBUGF( "***PLAYER INDEX:" << item->playerIndex() );
    DEBUGF( "***PLAYLIST INDEX:" << item->playlistCurIndex() );
    if( role == NameRole ) {
        DEBUGF("PLAYER NAME:" << item->playerName() );
        return item->playerName();
    }
    else if( role == MACRole ) {
        DEBUGF("MAC ADDRESS:" << item->macAddress() );
        return item->macAddress();
    }
    else if( role == PlayerRole ) {
        QVariant retVal;
        retVal.setValue(item);
        DEBUGF("PLAYER:" << retVal );
        return retVal;
    }
    else if( role == SongTitle ) {
        DEBUGF("Songtitle requested:" << item->currentTitle());
        return item->currentTitle();
    }
    else if( role == ArtistName ) {
        DEBUGF("ARTIST NAME:" << item->currentArtist() );
        return item->currentArtist();
    }
    else if( role == AlbumTitle ) {
        DEBUGF("ALBUM NAME:" << item->currentAlbum() );
        return item->currentAlbum();
    }
    else if( role == AlbumArt ) {
        DEBUGF("ALBUM ART ID:" << item->coverArtSource());
        return item->coverArtSource();
    }
    else if( role == Duration ) {
        DEBUGF("Duration requested:" << item->songDuration());
        return item->songDuration();
    }
    else if( role == TimeRemaining ) {
        DEBUGF("TIME REMAINING:" << item->songDuration() - item->songProgress() );
        return item->songDuration() - item->songProgress();
    }
    else if( role == CurrentProgress ){
        DEBUGF("PLAYER MODEL SONG PROGRESS:" << item->songProgress());
        return item->songProgress();
    }
    else if( role == PauseStatus ) {
        DEBUGF("PAUSE STATUS:" << item->playerMode() );
        return item->playerMode();
    }
    else if( role == RepeatStatus ) {
        DEBUGF("REPEAT STATUS:" << item->repeatPlaylist() );
        return item->repeatPlaylist();
    }
    else if( role == ShuffleStatus ) {
        DEBUGF("SHUFFLE STATUS:" << item->shufflePlaylist() );
        return item->shufflePlaylist();
    }
    else {
        DEBUGF("ERROR" );
        return QVariant();
    }
}

void Slimp3PlayerModel::UpdateIndex()
{
    QModelIndex newIdx = createIndex(0,0);
    emit dataChanged(newIdx, newIdx);
}

void Slimp3PlayerModel::appendRow(Slimp3Player2 *item)
{
    appendRows(QList<Slimp3Player2*>() << item);
}

void Slimp3PlayerModel::appendRows(const QList<Slimp3Player2 *> &items)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount() + items.size() - 1);
    foreach( Slimp3Player2 *item, items ) {
        m_list.append(item);
    }
    endInsertRows();
}

void Slimp3PlayerModel::insertRow(int row, Slimp3Player2 *item)
{

}

bool Slimp3PlayerModel::removeRow(int row, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    if(row < 0 || row >= m_list.size())
        return false;
    beginRemoveRows(QModelIndex(), row, row);
    delete m_list.takeAt(row);
    endRemoveRows();
    return true;
}

bool Slimp3PlayerModel::removeRows(int row, int count, const QModelIndex &parent)
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

Slimp3Player2 *Slimp3PlayerModel::takeRow(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    Slimp3Player2* item = m_list.takeAt(row);
    endRemoveRows();
    return item;
}

Slimp3Player2 *Slimp3PlayerModel::find(const QString &name) const
{
    foreach(Slimp3Player2* item, m_list) {
        if(item->playerName() == name)
            return item;
    }
    return nullptr;
}

Slimp3Player2 *Slimp3PlayerModel::find(const QByteArray &mac) const
{
    DEBUGF("LOOKING FOR MAC ADDRESS:" << mac);
    foreach(Slimp3Player2* item, m_list) {
        if(item->macAddress() == mac)
            return item;
    }
    return nullptr;
}

Slimp3Player2 *Slimp3PlayerModel::getRow(int row)
{
    Slimp3Player2 *item=m_list.at(row);
    return item;
}

QModelIndex Slimp3PlayerModel::indexFromItem(const Slimp3Player2 *item) const
{
    Q_ASSERT(item);
    for(int row=0; row<m_list.size(); ++row) {
        if(m_list.at(row) == item) return index(row);
    }
    return QModelIndex();
}

void Slimp3PlayerModel::clear()
{
    qDeleteAll(m_list);
    m_list.clear();
}

Slimp3Player2 *Slimp3PlayerModel::findPlayerByMAC(QString mac)
{
    return find(mac.toLatin1());
}

QString Slimp3PlayerModel::getPlayerName(QString mac)
{
    return findPlayerByMAC(mac)->playerName();
}

QString Slimp3PlayerModel::getSongTitle(QString mac)
{
    return findPlayerByMAC(mac)->currentTitle();
}

QString Slimp3PlayerModel::getArtistName(QString mac)
{
    return findPlayerByMAC(mac)->currentArtist();
}

QString Slimp3PlayerModel::getAlbumName(QString mac)
{
    return findPlayerByMAC(mac)->currentAlbum();
}

QString Slimp3PlayerModel::getAlbumArt(QString mac)
{
    return findPlayerByMAC(mac)->coverArtSource();
}

double Slimp3PlayerModel::getDuration(QString mac)
{
    return findPlayerByMAC(mac)->songDuration();
}

QString Slimp3PlayerModel::getTimeRemaining(QString mac)
{
    return "";
}

double Slimp3PlayerModel::getProgress(QString mac)
{
    return findPlayerByMAC(mac)->songProgress();
}

QHash<int, QByteArray> Slimp3PlayerModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "playerName";
    roles[MACRole] = "macAddress";
    roles[PlayerRole] = "playerObj";
    roles[SongTitle] = "songTitle";
    roles[ArtistName] = "artistName";
    roles[AlbumTitle] = "albumName";
    roles[AlbumArt] = "albumArtID";
    roles[Duration] = "duration";
    roles[TimeRemaining] = "timeRemaining";
    roles[CurrentProgress] = "currentProgress";
    roles[PauseStatus] = "pauseStatus";
    roles[RepeatStatus] = "repeatStatus";
    roles[ShuffleStatus] = "shuffleStatus";


    DEBUGF("ROLES RETURNED" << roles);

    return roles;
}
