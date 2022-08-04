#ifndef NETWORKINGIOS_H
#define NETWORKINGIOS_H

#include <QNetworkProxy>

//------------------------------------------------------------------------------

class NetworkingIOS
{
public:
    static QNetworkProxy systemProxy();
};

#endif // NETWORKINGIOS_H
