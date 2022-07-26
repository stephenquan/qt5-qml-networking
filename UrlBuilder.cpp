#include "UrlBuilder.h"
#include <QUrlQuery>

UrlBuilder::UrlBuilder(const QUrl& url, const QVariant& queryItems)
{
    set(url, queryItems);
}

QString UrlBuilder::query() const
{
    QStringList queryItems;
    for (QVariantMap::const_iterator iter = m_QueryItems.begin(); iter != m_QueryItems.end(); ++iter)
    {
        QString key = iter.key();
        QVariant values = iter.value();
        if (values.canConvert<QVariantList>())
        {
            foreach (QVariant value, values.toList())
            {
                queryItems.append(key + "=" + QUrl::toPercentEncoding(value.toString()));
            }
        }
        else
        {
            queryItems.append(key + "=" + QUrl::toPercentEncoding(values.toString()));
        }
    }
    QString result = queryItems.join("&");
    return result;
}

void UrlBuilder::set(const QUrl &url, const QVariant &queryItems)
{
    m_QueryItems.clear();

    QVariantMap _queryItems;
    if (queryItems.canConvert<QVariantMap>())
    {
        _queryItems = queryItems.toMap();
    }

    QUrlQuery urlQuery(url);
    QPair<QString, QString> item;
    foreach (item, urlQuery.queryItems(QUrl::FullyDecoded))
    {
        m_QueryItems[item.first] = item.second;
    }

    for (QVariantMap::iterator iter = _queryItems.begin(); iter != _queryItems.end(); ++iter)
    {
        QString key = iter.key();
        QVariant value = iter.value();
        if (!m_QueryItems.contains(key))
        {
            m_QueryItems[key] = value;
            continue;
        }

        QVariant oldValue = m_QueryItems[key];
        QVariantList list;
        if (oldValue.canConvert<QVariantList>())
        {
            list = oldValue.toList();
        }
        else
        {
            list.append(oldValue);
        }

        if (value.canConvert<QVariantList>())
        {
            foreach (QVariant newItem, value.toList())
            {
                list.append(newItem);
            }
        }
        else
        {
            list.append(value);
        }

        m_QueryItems[key] = list;
    }

    m_RootUrl = url;
    m_RootUrl.setQuery(QString());
}

QUrl UrlBuilder::toEncoded() const
{
    QString _rootUrl = m_RootUrl.toString();
    QString _query = query();
    return (_query.isEmpty() || _query.isNull())
            ? _rootUrl
            : _rootUrl + "?" + _query;
}
