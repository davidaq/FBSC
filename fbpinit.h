#ifndef FBPINIT_H
#define FBPINIT_H

#include "FBProcess.h"

class FBPInit : public FBProcess
{
public:
    FBPInit();
    virtual ~FBPInit();
    virtual int run();
};

#endif // FBPINIT_H
