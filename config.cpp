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

void Config::write(QDataStream& out)
{
    out << hash;
}

void Config::write(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    write(out);
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

bool Config::has(QString key)
{
    return hash.contains(key);
}

void Config::test()
{
    Config conf = Config::getConfig();

    conf.set("a", "ss");
    conf.set("b", "ssssss");
    conf.write("a.conf");
    conf.read("a.conf");
}

QString Config::getMarket(int index) {
    if(index >= 0 && index < marketsCount())
        return markets[index];
    return "Market" + QString::number(index);
}

int Config::marketsCount() {
    if(markets.isEmpty()) {
        markets = get("markets").split(";;");
        int i = 0;
        foreach(QString name, markets) {
            hash["marketName_" + QString::number(i++)] = name;
        }
    }
    return markets.size();
}

QStringList Config::getPlayers() {
    return get("players").split(";;");
}

void Config::genDefault()
{
    hash["players"] = "p1;;p2";
    hash["markets"] = "Hangzhou;;Wuhan";
    hash["Set of markets"] = "Hangzhou;;Wuhan;;Nanjing;;Shenyang;;Chengdu;;Xi'an";
    hash["Month in a quarter"] = QString::number(3);
    hash["Days in a month"] = QString::number(30);
    hash["Working hours in a month"] = QString::number(168);
    hash["Hours in a component"] = QString::number(4);
    hash["Workers in a component"] = QString::number(1);
    hash["Hours in a product"] = QString::number(1);
    hash["Engineers in a product"] = QString::number(4);
    hash["Hours in a product"] = QString::number(4);
    hash["Components in a product"] = QString::number(4);
    hash["Material cost per component"] = QString::number(200);
    hash["Material cost per product"] = QString::number(100);
    hash["Component storage cost per quarter"] = QString::number(50);
    hash["Product storage cost per quarter"] = QString::number(50);
    hash["Market population"] =  QString::number(1000000);
    hash["Penetration"] = QString::number(0.03);
    hash["Market report cost per market quarter"] = QString::number(500000);
    hash["Balance upper limit to have bank loans"] = QString::number(5000000);
    hash["Bank loan upper limit while positive balance"] = QString::number(5000000);
    hash["Bank loan upper limit while negative balance"] = QString::number(10000000);
    hash["Interest per quarter"] = QString::number(0.025);
    hash["Consultant cost per 30 minutes"] = QString::number(1000000);
    hash["Product price minimum"] = QString::number(1);
    hash["Product price maximum"] = QString::number(15000);
    hash["Initial cash"] = QString::number(10000000);
    hash["Salary minimum"] = QString::number(0);
    hash["Salary maximum"] = QString::number(30000);
    hash["Adding agent cost"] = QString::number(500000);
    hash["Removing agent cost"] = QString::number(250000);
    write("conf.d");
    hash.clear();
}
