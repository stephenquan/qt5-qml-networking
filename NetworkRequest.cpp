#include "NetworkRequest.h"
#include "UrlBuilder.h"

const QString NetworkRequest::kMethodGET = QStringLiteral("GET");
const QString NetworkRequest::kMethodPOST = QStringLiteral("POST");

NetworkRequest::NetworkRequest(QObject* parent) :
    QObject(parent),
    m_Manager(nullptr),
    m_Reply(nullptr),
    m_Method(kMethodGET),
    m_Error(NetworkErrorEnum::NoError),
    m_ReadyState(ReadyStateEnum::UNSENT)
{
}

void NetworkRequest::send(const QVariant& data)
{
    bool usePOST = false;

    if (m_Method.compare(kMethodPOST, Qt::CaseInsensitive) == 0)
    {
        usePOST = true;
    }

    if (m_Reply)
    {
        qDebug() << "Abort. A request is already in progress";
        return;
    }

    setReadyState(ReadyStateEnum::OPENED);

    if (usePOST)
    {
        m_Reply = post(data);
    }
    else
    {
        m_Reply = get(data);
    }

    connect(m_Reply, &QNetworkReply::finished, this, &NetworkRequest::onFinished);
}

void NetworkRequest::onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    if (reply != m_Reply)
    {
        return;
    }

    authenticator->setUser(m_User);
    authenticator->setPassword(m_Password);
    authenticator->setRealm(m_Realm);
}

void NetworkRequest::onFinished()
{
    if (!m_Reply)
    {
        return;
    }

    setResponse(m_Reply->readAll());
    setError(static_cast<NetworkErrorEnum::NetworkError>(m_Reply->error()));
    setErrorString(m_Reply->errorString());

    disconnect(m_Reply, &QNetworkReply::finished, this, &NetworkRequest::onFinished);

    m_Reply->deleteLater();
    m_Reply = nullptr;

    setReadyState(ReadyStateEnum::DONE);
}

void NetworkRequest::setError(NetworkErrorEnum::NetworkError error)
{
    if (m_Error == error)
    {
        return;
    }

    m_Error = error;

    emit errorChanged();
}

void NetworkRequest::setErrorString(const QString& errorString)
{
    if (m_ErrorString == errorString)
    {
        return;
    }

    m_ErrorString = errorString;

    emit errorStringChanged();
}

void NetworkRequest::setReadyState(ReadyStateEnum::ReadyState readyState)
{
    if (m_ReadyState == readyState)
    {
        return;
    }

    m_ReadyState = readyState;

    emit readyStateChanged();
}

bool NetworkRequest::running() const
{
    return m_ReadyState == ReadyStateEnum::OPENED
            || m_ReadyState == ReadyStateEnum::LOADING;
}

QNetworkAccessManager* NetworkRequest::manager()
{
    if (m_Manager)
    {
        return m_Manager;
    }

    m_Manager = qmlEngine(this)->networkAccessManager();
    connect(m_Manager, &QNetworkAccessManager::authenticationRequired, this, &NetworkRequest::onAuthenticationRequired);
    return m_Manager;
}

void NetworkRequest::setResponse(const QByteArray& response)
{
    if (m_Response == response)
    {
        return;
    }

    m_Response = response;

    emit responseChanged();
}

QNetworkReply* NetworkRequest::get(const QVariant& data)
{
    UrlBuilder urlBuilder(m_Url, data);
    QUrl url = urlBuilder.toEncoded();
    QNetworkRequest request(url);
    QNetworkReply* reply = manager()->get(request);
    qDebug() << Q_FUNC_INFO << url << reply;
    return reply;
}

QNetworkReply* NetworkRequest::post(const QVariant& data)
{
    UrlBuilder urlBuilder(m_Url, data);
    QUrl url = urlBuilder.rootUrl();
    QByteArray body = urlBuilder.query().toUtf8();
    QNetworkRequest request(url);
    QNetworkReply* reply = manager()->post(request, body);
    qDebug() << Q_FUNC_INFO << url << body << reply;
    return reply;
}
