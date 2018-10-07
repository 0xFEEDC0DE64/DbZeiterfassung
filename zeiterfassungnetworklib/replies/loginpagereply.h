#pragma once

#include <memory>

#include <QNetworkReply>

#include "zeiterfassungnetworklib_global.h"
#include "zeiterfassungreply.h"

class ZEITERFASSUNGNETWORKLIB_EXPORT LoginPageReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit LoginPageReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
};
