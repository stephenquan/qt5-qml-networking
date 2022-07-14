#ifndef UrlBuilder_H

#include <QUrl>
#include <QString>
#include <QVariantMap>

class UrlBuilder
{
public:
    UrlBuilder(const QUrl& url = QUrl(), const QVariant& queryItems = QVariant());

    QUrl rootUrl() const { return m_RootUrl; }
    QString query() const;
    QVariantMap queryItems() const { return m_QueryItems; }
    QUrl toEncoded() const;

    void set(const QUrl& url, const QVariant& queryItems = QVariant());

protected:
    QUrl m_RootUrl;
    QVariantMap m_QueryItems;
};

#endif
