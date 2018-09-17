#pragma once

#include <memory>

#include <QString>
#include <QDate>
#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungcorelib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGCORELIB_EXPORT GetDayinfoReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetDayinfoReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct Dayinfo
    {
        QString className;
        int userId;
        QDate date;
        QTime ist;
        QTime soll;
        QString compositeId;
    };

    const QVector<Dayinfo> &dayinfos() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<Dayinfo> m_dayinfos;
};
