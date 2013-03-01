#ifndef PLAYERMANAGER_H
#define PLAYERMANAGER_H

#include <QHash>
#include <player.h>

class PlayerManager
{
public:
    static PlayerManager& getManager();
    void flush(const QString& filename);
    void write(QDataStream &stream);
    void write(const QString& filename);
    void read(QDataStream& stream);
    void read(const QString& filename);
    void addPlayer(const Player& player);
    void removePlayer(QString name);
    Player& getPlayer(QString name);
    int playerCount() const;
    Player& operator [](QString name);
private:
    PlayerManager();
    static PlayerManager manager;
    QHash<QString, Player> players;
};

#endif // PLAYERMANAGER_H
