#include <Arduino.h>

volatile uint8_t FHSSptr = 0;

typedef enum
{
    RF_915,
    RF_433
} RFfreqs_;

RFfreqs_ RFfreq;

void ICACHE_RAM_ATTR FHSSsetCurrIndex(uint8_t value)
{ // get the current index of the FHSS pointer
    FHSSptr = value;
}

uint8_t ICACHE_RAM_ATTR FHSSgetCurrIndex()
{ // get the current index of the FHSS pointer
    return FHSSptr;
}

const uint32_t FHSSfreqs433[5] = {
    433420000,
    433670000,
    433920000,
    434170000,
    434420000};

const uint32_t FHSSfreqs915[32] = {
    918000000,
    918250000,
    918500000,
    918750000,

    919000000,
    919250000,
    919500000,
    919750000,

    920000000,
    920250000,
    920500000,
    920750000,

    921000000,
    921250000,
    921500000,
    921750000,

    922000000,
    922250000,
    922500000,
    922750000,

    923000000,
    923250000,
    923500000,
    923750000,

    924000000,
    924250000,
    924500000,
    924750000,

    925000000,
    925250000,
    925500000,
    925750000};

const uint8_t FHSSsequence915[255] = {17, 31, 16, 7, 8, 1, 6, 21, 29, 9, 30, 4, 13, 20, 27, 6, 2, 17, 30, 4, 3, 16, 11, 31, 11, 7, 0, 15, 9, 5, 19, 30, 17, 13, 3, 15,
                                      6, 4, 21, 6, 16, 19, 25, 28, 11, 12, 9, 29, 30, 31, 6, 21, 25, 26, 17, 28, 21, 3, 17, 19, 21, 24, 11, 19, 14, 9, 7, 23, 30, 6, 23, 15,
                                      26, 9, 29, 26, 0, 23, 9, 23, 19, 0, 6, 26, 14, 25, 18, 17, 16, 20, 28, 29, 16, 20, 1, 29, 14, 26, 1, 15, 13, 17, 0, 27, 29, 19, 18, 27,
                                      0, 22, 19, 6, 2, 8, 0, 11, 24, 29, 23, 11, 2, 27, 30, 28, 12, 4, 28, 22, 27, 10, 30, 27, 21, 17, 4, 23, 18, 20, 16, 29, 31, 23, 28, 1,
                                      20, 28, 7, 20, 1, 13, 0, 6, 12, 15, 26, 18, 26, 17, 7, 1, 27, 5, 15, 0, 11, 5, 31, 17, 26, 16, 24, 7, 19, 4, 3, 27, 26, 2, 19, 21,
                                      13, 0, 27, 26, 23, 4, 5, 22, 29, 4, 15, 18, 10, 2, 19, 30, 24, 10, 8, 4, 16, 1, 12, 0, 25, 17, 21, 15, 16, 29, 20, 13, 25, 17, 13, 21,
                                      26, 1, 12, 21, 18, 20, 7, 13, 7, 13, 29, 28, 8, 10, 17, 30, 9, 17, 15, 5, 3, 17, 15, 16, 3, 28, 4, 1, 25, 19, 5, 30, 9, 16, 26, 13,
                                      7, 17, 30};

const uint8_t FHSSsequence433[255] = {4, 2, 1, 0, 3, 1, 4, 0, 2, 0, 3, 0, 4, 3, 2, 4, 2, 1, 0, 3, 4, 2, 3, 0, 3, 2, 1, 3, 2, 4, 3, 2, 1, 3, 0, 1,
                                      0, 4, 3, 1, 4, 0, 4, 1, 2, 1, 0, 4, 1, 4, 2, 3, 0, 2, 0, 2, 1, 0, 2, 4, 2, 1, 4, 2, 4, 1, 4, 3, 1, 2, 0, 2,
                                      4, 0, 4, 1, 2, 4, 1, 4, 2, 4, 1, 4, 2, 1, 4, 1, 0, 1, 2, 3, 4, 1, 3, 4, 2, 3, 4, 2, 1, 0, 3, 2, 0, 3, 0, 2,
                                      4, 0, 4, 0, 2, 1, 3, 1, 0, 4, 3, 4, 1, 0, 3, 1, 4, 3, 1, 2, 4, 1, 0, 3, 1, 4, 1, 4, 2, 3, 1, 4, 1, 0, 3, 0,
                                      2, 4, 3, 1, 3, 1, 0, 4, 2, 3, 0, 3, 0, 1, 2, 1, 2, 1, 2, 0, 3, 4, 2, 3, 1, 4, 0, 1, 2, 3, 0, 4, 2, 0, 4, 0,
                                      4, 0, 3, 4, 2, 3, 2, 1, 0, 4, 2, 4, 0, 3, 1, 0, 1, 4, 2, 3, 2, 0, 2, 1, 4, 1, 0, 3, 1, 4, 1, 2, 0, 4, 1, 2,
                                      4, 1, 3, 2, 4, 1, 3, 4, 1, 2, 4, 1, 3, 0, 1, 3, 1, 2, 1, 2, 4, 0, 1, 2, 3, 4, 1, 2, 3, 1, 0, 4, 0, 2, 4, 2,
                                      4, 0, 2};

uint32_t ICACHE_RAM_ATTR GetInitialFreq()
{
    if (RFfreq == RF_915)
    {
        return FHSSfreqs915[0];
    }
    if (RFfreq == RF_433)
    {
        return FHSSfreqs433[0];
    }
}

void FHSSsetFreqMode(int freq)
{
    if (freq == 915)
    {
        RFfreq = RF_915;
    }
    if (freq == 433)
    {
        RFfreq = RF_433;
    }
}

uint32_t ICACHE_RAM_ATTR FHSSgetNextFreq()
{

    FHSSptr++;

    if (RFfreq == RF_915)
    {
        return FHSSfreqs915[FHSSsequence915[FHSSptr]];
    }
    if (RFfreq == RF_433)
    {
        return FHSSfreqs433[FHSSsequence433[FHSSptr]];
    }

    return 1;
}

uint32_t ICACHE_RAM_ATTR FHSSgetCurrFreq()
{
    if (RFfreq == RF_915)
    {
        return FHSSfreqs915[FHSSsequence915[FHSSptr]];
    }
    if (RFfreq == RF_433)
    {
        return FHSSfreqs433[FHSSsequence433[FHSSptr]];
    }
    return 0;
}