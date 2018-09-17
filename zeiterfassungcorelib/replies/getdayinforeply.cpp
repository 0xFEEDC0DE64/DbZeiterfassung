#include "getdayinforeply.h"

#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>

#include "zeiterfassungapi.h"

GetDayinfoReply::GetDayinfoReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung) :
    ZeiterfassungReply(zeiterfassung),
    m_reply(std::move(reply))
{
    connect(m_reply.get(), &QNetworkReply::finished, this, &GetDayinfoReply::requestFinished);
}

const QVector<GetDayinfoReply::Dayinfo> &GetDayinfoReply::dayinfos() const
{
    return m_dayinfos;
}

void GetDayinfoReply::requestFinished()
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
        m_dayinfos.clear();
        m_dayinfos.reserve(arr.count());
        for(const auto &val : arr)
        {
            auto obj = val.toObject();

            m_dayinfos.append({
                obj.value(QStringLiteral("className")).toString(),
                obj.value(QStringLiteral("persNr")).toInt(),
                parseDate(obj.value(QStringLiteral("date"))),
                parseTime(obj.value(QStringLiteral("ist"))),
                parseTime(obj.value(QStringLiteral("soll"))),
                obj.value(QStringLiteral("compositeId")).toString()
            });
        }
    }

    end:
    m_reply = Q_NULLPTR;

    Q_EMIT finished();
}
