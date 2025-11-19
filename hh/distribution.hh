#ifndef DISTRIBUTION_HH
#define DISTRIBUTION_HH

#include "mvector.hh"

class Distribution {
public:
    Mvector<Entry> group1;
    Mvector<Entry> group2;
    int marginSum;
    int overlap;
    int areaSum;
};

#endif
