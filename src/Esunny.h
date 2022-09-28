#ifndef ESUNNY_ESUNNY_H
#define ESUNNY_ESUNNY_H

#include "QuoteSpi.h"
#include "TradeSpi.h"
#include "SimpleEvent.h"
using namespace std;

class Esunny
{
public:
    Esunny();

    ~Esunny();

    void EsunnyStart();

public:

    QuoteSpi* quote;
    TradeSpi* trade;
    SimpleEvent    m_Event;
};

#endif //ESUNNY_ESUNNY_H
