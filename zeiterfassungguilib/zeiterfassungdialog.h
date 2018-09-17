#pragma once

#include <QDialog>

#include "zeiterfassungguilib_global.h"

class ZEITERFASSUNGGUILIB_EXPORT ZeiterfassungDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ZeiterfassungDialog(QWidget *parent = Q_NULLPTR);
};
