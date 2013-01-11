#include "playermanager.h"
#include <QFile>
#include <QDataStream>

PlayerManager PlayerManager::manager;

PlayerManager::PlayerManager()
{
}

PlayerManager& PlayerManager::getManager() {
    return manager;
}

void PlayerManager::write(const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::WriteOnly)) {
        QDataStream stream(&file);
        foreach(Player player, players.values()) {
            stream<<player.name<<player.cash<<player.loan<<player.marketAgents<<player.record;
        }
        file.close();
    }
}

void PlayerManager::read(const QString &filename)
{
    QFile file(filename);
    players.clear();
    if(file.open(QFile::ReadOnly)) {
        QDataStream stream(&file);
        while(!stream.atEnd()) {
            Player player;
            stream>>player.name>>player.cash>>player.loan>>player.marketAgents>>player.record;
            addPlayer(player);
        }
        file.close();
    }
}

void PlayerManager::addPlayer(const Player &player)
{
    players[player.name] = player;
}


void PlayerManager::removePlayer(QString name)
{
    players.remove(name);
}

static Player dummy;
Player& PlayerManager::getPlayer(QString name) {
    if(players.contains(name))
        return players[name];
    return dummy;
}

Player& PlayerManager::operator [](QString name) {
    return getPlayer(name);
}
