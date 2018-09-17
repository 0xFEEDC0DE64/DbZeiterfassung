#pragma once

#include <QObject>
#include <QWidget>

#include "zeiterfassungguilib_global.h"

class ZEITERFASSUNGGUILIB_EXPORT SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(QWidget *parent = Q_NULLPTR);

    virtual bool isValid(QString &message) const { Q_UNUSED(message) return true; }

public Q_SLOTS:
    virtual bool apply() = 0;
};
