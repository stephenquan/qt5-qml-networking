#ifndef __ReadyState__
#define __ReadyState__

#include "EnumInfo.h"
#include <qqml.h>

class ReadyStateEnum : public EnumInfo
{
    Q_OBJECT

    QML_NAMED_ELEMENT(ReadyState)
    QML_SINGLETON

public:
    ReadyStateEnum(QObject* parent = nullptr) :
        EnumInfo(parent, QMetaEnum::fromType<ReadyState>()) { }

    enum ReadyState
    {
        UNSENT = 0,
        OPENED = 1,
        HEADERS_RECEIVED = 2,
        LOADING = 3,
        DONE = 4
    };

    Q_ENUM(ReadyState)

};

#endif
