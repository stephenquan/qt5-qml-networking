#include "NetworkingIOS.h"
#import <Foundation/Foundation.h>

#include <QUrl>

#include <CoreServices/CoreServices.h>
#include <SystemConfiguration/SystemConfiguration.h>
#include <CFNetwork/CFNetwork.h>

QNetworkProxy NetworkingIOS::systemProxy()
{
    NSDictionary* proxySettings =  CFBridgingRelease(CFNetworkCopySystemProxySettings());
    bool isHTTPEnable = [proxySettings[(__bridge NSString*)kCFNetworkProxiesHTTPEnable] boolValue];
    if (!isHTTPEnable)
    {
        return QNetworkProxy();
    }

    NSString* nsHttpProxy =  proxySettings[(__bridge NSString*)kCFNetworkProxiesHTTPProxy];
    if (!nsHttpProxy)
    {
        return QNetworkProxy();
    }
    QString httpProxy = QString::fromNSString(nsHttpProxy);

    NSUInteger nsHttpPort = [proxySettings[(__bridge NSString*)kCFNetworkProxiesHTTPPort] unsignedIntegerValue];
    int httpPort = static_cast<int>(nsHttpPort);
 
    return QNetworkProxy(QNetworkProxy::HttpProxy, httpProxy, httpPort);
}
