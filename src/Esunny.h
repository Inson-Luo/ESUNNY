#ifndef ESUNNY_ESUNNY_H
#define ESUNNY_ESUNNY_H

#include "QuoteSpi.h"
using namespace std;

class Esunny
{
public:
    Esunny();

    ~Esunny();

    void EsunnyStart();

public:

    QuoteSpi* quote;
};

#endif //ESUNNY_ESUNNY_H
