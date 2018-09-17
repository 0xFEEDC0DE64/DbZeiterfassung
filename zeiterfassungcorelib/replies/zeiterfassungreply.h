#pragma once

#include <QObject>

#include "zeiterfassungcorelib_global.h"

class QJsonValue;

class ZeiterfassungApi;

class ZEITERFASSUNGCORELIB_EXPORT ZeiterfassungReply : public QObject
{
    Q_OBJECT

public:
    explicit ZeiterfassungReply(ZeiterfassungApi *zeiterfassung);

    bool success() const;
    const QString &message() const;

    void waitForFinished();

    static QDate parseDate(const QJsonValue &value);
    static QTime parseTime(const QJsonValue &value);

Q_SIGNALS:
    void finished();

protected:
    ZeiterfassungApi *zeiterfassung() const;
    void setSuccess(bool success);
    void setMessage(const QString &message);

private:
    ZeiterfassungApi *m_zeiterfassung;
    bool m_success;
    QString m_message;
};
