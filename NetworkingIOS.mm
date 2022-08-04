#include "NetworkingIOS.h"
#import <Foundation/Foundation.h>

#include <QDebug>
#include <QUrl>

#include <CoreServices/CoreServices.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <CFNetwork/CFNetwork.h>

QNetworkProxy NetworkingIOS::systemProxy()
{
    NSDictionary* proxySettings =  CFBridgingRelease(CFNetworkCopySystemProxySettings());
    bool isHTTPEnable = [proxySettings[(__bridge NSString*)kCFNetworkProxiesHTTPEnable] boolValue];
    qDebug() << Q_FUNC_INFO << "isHTTPEnable: " << isHTTPEnable;
    if (!isHTTPEnable)
    {
        //return QNetworkProxy();
    }

    NSString* nsHttpProxy =  proxySettings[(__bridge NSString*)kCFNetworkProxiesHTTPProxy];
    if (!nsHttpProxy)
    {
        return QNetworkProxy();
    }
    QString httpProxy = QString::fromNSString(nsHttpProxy);
    qDebug() << Q_FUNC_INFO << "HttpProxy: " << httpProxy;

    NSUInteger nsHttpPort = [proxySettings[(__bridge NSString*)kCFNetworkProxiesHTTPPort] unsignedIntegerValue];
    int httpPort = static_cast<int>(nsHttpPort);
    qDebug() << Q_FUNC_INFO << "HttpPort: " << httpPort;

    return QNetworkProxy(QNetworkProxy::HttpProxy, httpProxy, httpPort);
}
