#ifndef CARSTORE_H
#define CARSTORE_H

#include <QString>
#include "provide.h"

class carstore {
public:
    static bool exportto(const parkingsystem& sys, const QString& filepath);
    static bool importfrom(parkingsystem& sys, const QString& filepath);

};

#endif // CARSTORE_H
