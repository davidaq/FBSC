#ifndef CONFIG_H
#define CONFIG_H

#include <QHash>
#include <QString>


class Config
{
public:
    static const Config& getConfig();
    static void test();
    void read(QString filename);
    void write(QString filename);
    void set(QString key, QString value);
    QString get(QString key);
private:
    explicit Config();
    static Config config;
    QHash<QString,QString> hash;
};

#endif // CONFIG_H


