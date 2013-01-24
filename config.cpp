#include "config.h"
#include <QString>
#include <QHash>
#include <QDataStream>
#include <QFile>
#include <QDebug>


Config Config::config;

Config::Config()
{

}

Config &Config::getConfig()
{
    return config;
}

void Config::read(QDataStream& in)
{
    in >> hash;
    markets.clear();
}

void Config::read(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    read(in);
    file.close();
}

void Config::write(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << hash;
    file.close();
}

void Config::set(QString key, QString value)
{
    hash[key] = value;
}

QString Config::get(QString key)
{
    return hash[key];
}

void Config::test()
{
    Config conf = Config::getConfig();

    conf.set("a", "ss");
    conf.set("b", "ssssss");
    conf.write("a.conf");
    conf.read("a.conf");
    qDebug() << conf.get("a");
    qDebug() << conf.get("b");
}

QString Config::getMarket(int index) {
    if(markets.isEmpty()) {
        markets = get("markets").split(";;");
    }
    return markets[index];
}

QStringList Config::getPlayers() {
    return get("players").split(";;");
}
