#ifndef __NetworkRequest__
#define __NetworkRequest__

#include <QObject>
#include <QNetworkAccessManager>
#include <QQmlEngine>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QString>
#include <qqml.h>

#include "NetworkError.h"
#include "ReadyState.h"

class NetworkRequest : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString method MEMBER m_Method NOTIFY methodChanged)
    Q_PROPERTY(QUrl url MEMBER m_Url NOTIFY urlChanged)
    Q_PROPERTY(QString user MEMBER m_User NOTIFY userChanged)
    Q_PROPERTY(QString password MEMBER m_Password NOTIFY passwordChanged)
    Q_PROPERTY(QString realm MEMBER m_Realm NOTIFY realmChanged)
    Q_PROPERTY(QByteArray response READ response NOTIFY responseChanged)
    Q_PROPERTY(QString responseText READ responseText NOTIFY responseChanged)
    Q_PROPERTY(NetworkErrorEnum::NetworkError error READ error NOTIFY errorChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(ReadyStateEnum::ReadyState readyState READ readyState NOTIFY readyStateChanged)
    Q_PROPERTY(bool running READ running NOTIFY readyStateChanged)

    QML_ELEMENT

public:
    NetworkRequest(QObject* parent = nullptr);

    Q_INVOKABLE void send(const QVariant& data = QVariant());

    static const QString kMethodGET;
    static const QString kMethodPOST;

signals:
    void urlChanged();
    void userChanged();
    void passwordChanged();
    void realmChanged();
    void responseChanged();
    void errorChanged();
    void errorStringChanged();
    void readyStateChanged();
    void methodChanged();

protected slots:
    void onAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);
    void onFinished();

protected:
    QNetworkAccessManager* m_Manager;
    QNetworkReply* m_Reply;
    QString m_Method;
    QUrl m_Url;
    QString m_User;
    QString m_Password;
    QString m_Realm;
    QByteArray m_Response;
    NetworkErrorEnum::NetworkError m_Error;
    QString m_ErrorString;
    ReadyStateEnum::ReadyState m_ReadyState;

    QByteArray response() const { return m_Response; }
    QString responseText() const { return QString::fromUtf8(m_Response); }

    NetworkErrorEnum::NetworkError error() const { return m_Error; }
    void setError(NetworkErrorEnum::NetworkError error);

    QString errorString() const { return m_ErrorString; }
    void setErrorString(const QString& errorString);

    ReadyStateEnum::ReadyState readyState() const { return m_ReadyState; }
    void setReadyState(ReadyStateEnum::ReadyState readyState);

    bool running() const;

    QNetworkAccessManager* manager();

    void setResponse(const QByteArray& response);

    QNetworkReply* get(const QVariant& data);
    QNetworkReply* post(const QVariant& data);

};

#endif
