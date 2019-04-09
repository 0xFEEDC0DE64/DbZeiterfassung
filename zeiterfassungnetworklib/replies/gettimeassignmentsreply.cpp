#include "gettimeassignmentsreply.h"

#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "zeiterfassungapi.h"

GetTimeAssignmentsReply::GetTimeAssignmentsReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetTimeAssignmentsReply::requestFinished);
}

const QVector<GetTimeAssignmentsReply::TimeAssignment> &GetTimeAssignmentsReply::timeAssignments() const
{
    return m_timeAssignments;
}

void GetTimeAssignmentsReply::requestFinished()
{
    if(m_reply->error() != QNetworkReply::NoError)
    {
        setSuccess(false);
        setMessage(tr("Request error occured: %0").arg(m_reply->errorString()));
        goto end;
    }

    {
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(m_reply->readAll(), &error);
        if(error.error != QJsonParseError::NoError)
        {
            setSuccess(false);
            setMessage(tr("Parsing JSON failed: %0").arg(error.errorString()));
            goto end;
        }

        if(!document.isArray())
        {
            setSuccess(false);
            setMessage(tr("JSON document is not an array!"));
            goto end;
        }

        auto arr = document.array();

        setSuccess(true);
        m_timeAssignments.clear();
        m_timeAssignments.reserve(arr.count());
        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            auto koWertList = obj.value(QStringLiteral("koWertList")).toArray();

            m_timeAssignments.append({
                 .id = obj.value(QStringLiteral("bookingNr")).toInt(),
                 .date = parseDate(obj.value(QStringLiteral("bookingDate"))),
                 .time = parseTime(obj.value(QStringLiteral("bookingTime"))),
                 .timespan = parseTime(obj.value(QStringLiteral("bookingTimespan"))),
                 .text = obj.value(QStringLiteral("text")).toString(),
                 .project = koWertList.at(0).toObject().value(QStringLiteral("value")).toString(),
                 .workpackage = koWertList.at(1).toObject().value(QStringLiteral("value")).toString()
            });
        }
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
