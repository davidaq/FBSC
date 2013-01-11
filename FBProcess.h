#ifndef FBPROCESS_H
#define FBPROCESS_H

#include <QDebug>
class FBProcess {
public:
    virtual int run() = 0;
    virtual ~FBProcess(){}
};

#endif // FBPROCESS_H
