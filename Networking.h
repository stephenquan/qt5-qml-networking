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
    Networking(QObject* parent = nullptr);

signals:
    void proxyChanged();
    void useSystemProxyChanged();

protected:
    bool useSystemProxy() const;
    void setUseSystemProxy(bool useSystemProxy);
    QUrl proxy() const;
    void setProxy(const QUrl& proxy);

    static QUrl QNetworkProxyToQUrl(const QNetworkProxy& networkProxy);

};

#endif
