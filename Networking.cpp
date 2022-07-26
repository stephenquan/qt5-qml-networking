#include "Networking.h"

#ifdef Q_OS_IOS
#include <QtWebView>
#endif

Networking::Networking(QObject* parent) :
    QObject(parent)
{
#ifdef Q_OS_IOS
    QtWebView::initialize();
#endif
}

bool Networking::useSystemProxy() const
{
    return QNetworkProxyFactory::usesSystemConfiguration();
}

void Networking::setUseSystemProxy(bool useSystemProxy)
{
    bool before = Networking::useSystemProxy();
    QNetworkProxyFactory::setUseSystemConfiguration(useSystemProxy);
    bool after = Networking::useSystemProxy();

    if (after != before)
    {
        emit useSystemProxyChanged();
    }
}

QUrl Networking::proxy() const
{
    if (QNetworkProxyFactory::usesSystemConfiguration())
    {
        QNetworkProxyQuery query(QUrl("https://www.arcgis.com"));
        QList<QNetworkProxy> proxies = QNetworkProxyFactory::proxyForQuery(query);
        if (!proxies.length())
        {
            return QUrl();
        }
        QUrl url = QNetworkProxyToQUrl(proxies[0]);
        return url;
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
