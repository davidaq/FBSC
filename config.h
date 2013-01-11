#ifndef CONFIG_H
#define CONFIG_H

#include <QHash>
#include <QString>
#include <QStringList>

class Config
{
public:
    static Config& getConfig();
    static void test();
    void read(QString filename);
    void write(QString filename);
    void set(QString key, QString value);
    QString get(QString key);
    QString getMarket(int index);
    QStringList players();
private:
    QStringList markets;
    explicit Config();
    static Config config;
    QHash<QString,QString> hash;
};

#endif // CONFIG_H


