#pragma once

#include <memory>

#include <QDate>
#include <QTime>
#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungnetworklib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGNETWORKLIB_EXPORT GetTimeAssignmentsReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetTimeAssignmentsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct TimeAssignment
    {
        int id;
        QDate date;
        QTime time;
        QTime timespan;
        QString text;
        QString project;
        QString workpackage;
    };

    const QVector<TimeAssignment> &timeAssignments() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<TimeAssignment> m_timeAssignments;
};
