#include "playermanager.h"
#include <QFile>
#include <QDataStream>
#include <QTextStream>

QHash<QString, QString> Player::globalRecord;

PlayerManager PlayerManager::manager;

PlayerManager::PlayerManager()
{
}

PlayerManager& PlayerManager::getManager() {
    return manager;
}

void PlayerManager::write(QDataStream &stream)
{
    stream<<Player::globalRecord;
    foreach(Player player, players.values()) {
        stream<<player.name<<player.homeMarket<<player.cash<<player.loan<<player.marketAgents<<player.record;
    }
}

void PlayerManager::write(const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::WriteOnly)) {
        QDataStream stream(&file);
        write(stream);
        file.close();
    }
}

void PlayerManager::read(const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::ReadOnly)) {
        QDataStream stream(&file);
        read(stream);
        file.close();
    }
}

void PlayerManager::read(QDataStream& stream)
{
    players.clear();
    stream>>Player::globalRecord;
    while(!stream.atEnd()) {
        Player player;
        stream>>player.name>>player.homeMarket>>player.cash>>player.loan>>player.marketAgents>>player.record;
        addPlayer(player);
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

int PlayerManager::playerCount() const {
    return players.size();
}

Player& PlayerManager::operator [](QString name) {
    return getPlayer(name);
}

void PlayerManager::flush(const QString &filename)
{
    QFile file(filename);
    if(file.open(QFile::WriteOnly)) {
        QTextStream out(&file);
        foreach(QString key, Player::globalRecord.keys()) {
            out<<key<<":"<<Player::globalRecord[key]<<"\r\n";
        }
        foreach(Player player, players.values()) {
            out<<"\r\n"<<'['<<player.name<<" $"<<player.cash<<']'<<"\r\n";
            out<<"Home:"<<player.homeMarket<<"\r\n";
            out<<"Loan:"<<player.loan<<"\r\n";
            out<<"MarketAgents:"<<"\r\n";
            foreach(int marketId, player.marketAgents.keys()) {
                out<<'\t'<<marketId<<':'<<player.marketAgents[marketId]<<"\r\n";
            }
            foreach(QString key, player.record.keys()) {
                out<<key<<":"<<player.record[key]<<"\r\n";
            }
        }
    }
    file.close();
}
