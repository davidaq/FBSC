#ifndef FBPLOAN_H
#define FBPLOAN_H

#include "FBProcess.h"

class FBPLoan : public FBProcess
{
public:
    FBPLoan();
    virtual ~FBPLoan();
    virtual int run();
};

#endif // FBPLOAN_H
