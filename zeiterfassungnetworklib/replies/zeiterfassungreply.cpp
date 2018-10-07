#include "zeiterfassungreply.h"

#include <QEventLoop>
#include <QJsonValue>

#include "zeiterfassungapi.h"

ZeiterfassungReply::ZeiterfassungReply(ZeiterfassungApi *zeiterfassung) :
    QObject(zeiterfassung),
    m_zeiterfassung(zeiterfassung),
    m_success(false)
{

}

bool ZeiterfassungReply::success() const
{
    return m_success;
}

const QString &ZeiterfassungReply::message() const
{
    return m_message;
}

void ZeiterfassungReply::waitForFinished()
{
    QEventLoop eventLoop;
    connect(this, &ZeiterfassungReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
}

QDate ZeiterfassungReply::parseDate(const QJsonValue &value)
{
    if(value.isNull())
        return QDate();

    return QDate::fromString(QString::number(value.toInt()), QStringLiteral("yyyyMMdd"));
}

QTime ZeiterfassungReply::parseTime(const QJsonValue &value)
{
    if(value.isNull())
        return QTime();

    return QTime::fromString(QStringLiteral("%0").arg(value.toInt(), 6, 10, QChar('0')), QStringLiteral("HHmmss"));
}

ZeiterfassungApi *ZeiterfassungReply::zeiterfassung() const
{
    return m_zeiterfassung;
}

void ZeiterfassungReply::setSuccess(bool success)
{
    m_success = success;
}

void ZeiterfassungReply::setMessage(const QString &message)
{
    m_message = message;
}
