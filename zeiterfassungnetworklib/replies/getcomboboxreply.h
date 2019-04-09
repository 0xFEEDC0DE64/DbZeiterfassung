#pragma once

#include <memory>

#include <QNetworkReply>
#include <QVector>

#include "zeiterfassungnetworklib_global.h"
#include "zeiterfassungreply.h"

class ZeiterfassungApi;

class ZEITERFASSUNGNETWORKLIB_EXPORT GetComboboxReply : public ZeiterfassungReply
{
    Q_OBJECT

public:
    explicit GetComboboxReply(std::unique_ptr<QNetworkReply> &&reply, ZeiterfassungApi *zeiterfassung);

    struct Item
    {
        QString label;
        QString value;
    };

    const QVector<Item> &items() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    QVector<Item> m_items;
};
