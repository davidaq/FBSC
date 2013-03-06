#include "fbpreport.h"
#include "player.h"
#include "playermanager.h"
#include "config.h"
#include <QFile>
#include <QDir>

int FBPReport::run()
{
    // clean HTMLs
    QDir dir;
    foreach(QString file, dir.entryList())
    {
        if(file.endsWith(".html"))
            dir.remove(file);
    }

    // start the report process
    PlayerManager& manager = PlayerManager::getManager();
    Config& config = Config::getConfig();

    QFile tpl(":/template/header.html");
    if(!tpl.open(QFile::ReadOnly))
        return 1;
    QByteArray headerTemplate = tpl.readAll();
    tpl.close();
    tpl.setFileName(":/template/player.html");
    if(!tpl.open(QFile::ReadOnly))
        return 1;
    QByteArray playerTemplate = tpl.readAll();
    tpl.close();
    tpl.setFileName(":/template/footer.html");
    if(!tpl.open(QFile::ReadOnly))
        return 1;
    QByteArray footerTemplate = tpl.readAll();
    tpl.close();
    tpl.setFileName(":/template/market.html");
    if(!tpl.open(QFile::ReadOnly))
        return 1;
    QByteArray marketTemplate = tpl.readAll();
    tpl.close();
    config.marketsCount();
    int p = 0, e, sz = manager.playerCount();
    QByteArray loopToken = "<!-- loop -->";
    while(-1 != (p = playerTemplate.indexOf(loopToken, p))) {
        p += loopToken.size();
        if(-1 != (e = playerTemplate.indexOf("<!-- end -->", p))) {
            QByteArray item = playerTemplate.mid(p, e - p);
            QByteArray replace;
            for(int i = 0; i < sz; i++) {
                QByteArray row = item;
                replace += row.replace("$$", QByteArray::number(i));
            }
            playerTemplate.replace(p, e - p, replace);
        }
    }
    p = 0;
    while(-1 != (p = marketTemplate.indexOf(loopToken, p))) {
        p += loopToken.size();
        if(-1 != (e = marketTemplate.indexOf("<!-- end -->", p))) {
            QByteArray item = marketTemplate.mid(p, e - p);
            QByteArray replace;
            for(int i = 0; i < sz; i++) {
                QByteArray row = item;
                replace += row.replace("$$", QByteArray::number(i));
            }
            marketTemplate.replace(p, e - p, replace);
        }
    }
    QRegExp var("\\$\\{([^\\}]+)\\}");

    QHash<int, QHash<QString, QHash<QString, QString> > > marketData;

    //generate market report
    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);
        for(int mid = 0, sz = manager.playerCount(); mid < sz; mid++) {
            if(player.marketAgents[mid] == 0)
                continue;
            QString midStr = QString::number(mid);
            marketData[mid][name]["agents"] = player.record["agents_" + midStr];
            marketData[mid][name]["share"] = player.record["marketShare_" + midStr];
            marketData[mid][name]["orders"]  = player.record["marketOrder_" + midStr];
            marketData[mid][name]["price"]  = player.record["price"];
        }
    }
    QHash<int, QString> marketReport;
    for(int mid = 0, sz = manager.playerCount(); mid < sz; mid++) {
        QHash<QString, QHash<QString, QString> >& market = marketData[mid];
        QHash<QString, QString> vars;
        vars["market_name"] = config.getMarket(mid);
        vars["penetration"] = config.get("Penetration");
        vars["population"] = config.get("Market population");
        vars["orders"] = QString::number((qint64)(config.get("Market population").toLongLong() * config.get("Penetration").toFloat()));
        vars["players"] = QString::number(market.size());
        int agents = 0, index = -1;
        foreach(QString name, config.getPlayers()) {
            index++;
            if(!market.contains(name))
                continue;
            QString indexStr = QString::number(index);
            agents += market[name]["agents"].toInt();
            vars["agents_" + indexStr] = market[name]["agents"];
            vars["price_" + indexStr] = market[name]["price"];
            vars["share_" + indexStr] = market[name]["share"];
            vars["orders_" + indexStr] = market[name]["orders"];
            vars["name_" + indexStr] = name;
        }
        vars["agents"] = QString::number(agents);
        QString report = marketTemplate;
        while(-1 != var.indexIn(report)) {
            QString key = var.cap(1);
            if(vars.contains(key)) {
                key = vars[key];
            } else {
                key = "0";
            }
            if(key.isEmpty())
                key = "0";
            bool isInt;
            qint64 num;
            if((num = key.toLongLong(&isInt)) && isInt) {
                bool neg = num < 0;
                if(neg)
                    num = -num;
                if(num > 999) {
                    key = "";
                    QString m = QString::number(num);
                    int len = m.length();
                    while(len > 0) {
                        key = m.right(3) + " " + key;
                        len -= 3;
                        m = m.left(len);
                    }
                    if(neg)
                        key = "-" + key;
                }
            }
            report = report.replace(var.cap(), key);
        }
        marketReport[mid] = report;
    }

    //generate player report

    foreach(QString name, config.getPlayers()) {
        Player& player = manager.getPlayer(name);
        player.record["team_name"] = name;
        player.record["debt"] = QString::number(player.loan);
        QString report = playerTemplate;
        while(-1 != var.indexIn(report)) {
            QString key = var.cap(1);
            if(player.record.contains(key)) {
                key = player.record[key];
            } else if(Player::globalRecord.contains(key)) {
                key = Player::globalRecord[key];
            } else if(config.has(key)) {
                key = config.get(key);
            } else {
                key = "0";
            }
            if(key.isEmpty())
                key = "0";
            bool isInt;
            qint64 num;
            if((num = key.toLongLong(&isInt)) && isInt) {
                bool neg = num < 0;
                if(neg)
                    num = -num;
                if(num > 999) {
                    key = "";
                    QString m = QString::number(num);
                    int len = m.length();
                    while(len > 0) {
                        key = m.right(3) + " " + key;
                        len -= 3;
                        m = m.left(len);
                    }
                    if(neg)
                        key = "-" + key;
                }
            }
            report = report.replace(var.cap(), key);
        }
        if(!player.record["marketsOrderedReport"].isEmpty()) {
            foreach(QString i, player.record["marketsOrderedReport"].split(";;")) {
                int mid = i.toInt();
                report += marketReport[mid];
            }
        }
        report = headerTemplate + report + footerTemplate;
        QFile fp(name.replace(" ", "_") + "_of_Round_" + Player::globalRecord["round"] + ".html");
        if(fp.open(QFile::WriteOnly)) {
            fp.write(report.toUtf8());
            fp.close();
        }
    }
    return 0;
}
