#include "Networking.h"
#include "NetworkingIOS.h"

Networking::Networking(QObject* parent) :
    QObject(parent)
{
}

bool Networking::useSystemProxy() const
{
    return QNetworkProxyFactory::usesSystemConfiguration();
}

void Networking::setUseSystemProxy(bool useSystemProxy)
{
    bool before = Networking::useSystemProxy();
    QUrl beforeProxy = Networking::proxy();
    QNetworkProxyFactory::setUseSystemConfiguration(useSystemProxy);
    bool after = Networking::useSystemProxy();
    QUrl afterProxy = Networking::proxy();

    if (after != before)
    {
        emit useSystemProxyChanged();
    }

    if (afterProxy != beforeProxy)
    {
        emit proxyChanged();
    }
}

QUrl Networking::proxy() const
{
    if (QNetworkProxyFactory::usesSystemConfiguration())
    {
#ifdef Q_OS_IOS
        QNetworkProxy iOSNetworkProxy = NetworkingIOS::systemProxy();
        return QNetworkProxyToQUrl(iOSNetworkProxy);
#else
        QNetworkProxyQuery query(QUrl("https://www.arcgis.com"));
        QList<QNetworkProxy> proxies = QNetworkProxyFactory::proxyForQuery(query);
        if (!proxies.length())
        {
            return QUrl();
        }
        QUrl url = QNetworkProxyToQUrl(proxies[0]);
        return url;
#endif
    }

    QNetworkProxy networkProxy = QNetworkProxy::applicationProxy();
    QUrl url = QNetworkProxyToQUrl(networkProxy);
    return url;
}

void Networking::setProxy(const QUrl& proxy)
{
    setUseSystemProxy(false);

    QString host = proxy.host();
    int port = proxy.port();
    if (!host.isEmpty() && port)
    {
        QNetworkProxy networkProxy(QNetworkProxy::HttpProxy, host, port);
        QNetworkProxy::setApplicationProxy(networkProxy);
    }
    else
    {
        QNetworkProxy networkProxy(QNetworkProxy::NoProxy);
        QNetworkProxy::setApplicationProxy(networkProxy);
    }

    emit proxyChanged();
}


QUrl Networking::QNetworkProxyToQUrl(const QNetworkProxy& networkProxy)
{
    if (networkProxy.type() != QNetworkProxy::HttpProxy)
    {
        return QUrl();
    }

    QString host = networkProxy.hostName();
    int port = networkProxy.port();
    QUrl url;
    url.setScheme("http");
    url.setHost(host);
    url.setPort(port);
    return url;
}
