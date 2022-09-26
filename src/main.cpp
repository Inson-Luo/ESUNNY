#include <iostream>
#include "TapQuoteAPI.h"
#include "TapAPIError.h"
#include "QuoteSpi.h"
#include <cstring>
#include "Esunny.h"
using namespace std;
int main()
{
    Esunny* e = new Esunny();
    e->EsunnyStart();

    return 0;
}
