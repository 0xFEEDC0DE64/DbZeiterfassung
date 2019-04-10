#pragma once

#include <memory>
#include <vector>

#include <QNetworkReply>

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
        Item(const QString &label, const QString &value) :
            label(label), value(value)
        {}

        QString label;
        QString value;
    };

    const std::vector<Item> &items() const;

private Q_SLOTS:
    void requestFinished();

private:
    std::unique_ptr<QNetworkReply> m_reply;
    std::vector<Item> m_items;
};
