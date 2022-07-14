#ifndef __Networking__
#define __Networking__

#include <QObject>
#include <QNetworkAccessManager>
#include <QQmlEngine>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QNetworkProxyFactory>
#include <QNetworkProxy>
#include <QQmlEngine>
#include <QJSEngine>
#include <qqml.h>
#include <QDebug>

class Networking : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl proxy READ proxy WRITE setProxy NOTIFY proxyChanged)
    Q_PROPERTY(bool useSystemProxy READ useSystemProxy WRITE setUseSystemProxy NOTIFY useSystemProxyChanged)

    QML_ELEMENT
    QML_SINGLETON

public:
    Networking(QObject* parent = nullptr) :
        QObject(parent),
        m_UseSystemProxy(false)
    {
    }

    static QObject* singletonProvider(QQmlEngine* engine, QJSEngine* jsEngine)
    {
        Q_UNUSED(engine)
        Q_UNUSED(jsEngine)
        return new Networking();
    }

signals:
    void proxyChanged();
    void useSystemProxyChanged();

protected:
    bool m_UseSystemProxy;
    QUrl m_Proxy;

    bool useSystemProxy() const { return m_UseSystemProxy; }

    void setUseSystemProxy(bool useSystemProxy)
    {
        QNetworkProxyFactory::setUseSystemConfiguration(useSystemProxy);

        m_UseSystemProxy = useSystemProxy;

        emit useSystemProxyChanged();
    }

    QUrl proxy() const { return m_Proxy; }

    void setProxy(const QUrl& proxy)
    {
        m_UseSystemProxy = false;
        emit useSystemProxyChanged();

        m_Proxy = proxy;

        QQmlEngine* engine = qmlEngine(this);
        QNetworkAccessManager* manager = engine->networkAccessManager();
        manager->setProxy(QNetworkProxy(QNetworkProxy::HttpProxy, proxy.host(), proxy.port()));
        emit proxyChanged();
    }

};

#endif
