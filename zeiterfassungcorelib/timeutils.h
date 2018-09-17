#pragma once

#include <QTime>

#include "zeiterfassungcorelib_global.h"

int ZEITERFASSUNGCORELIB_EXPORT timeToSeconds(const QTime &time);
QTime ZEITERFASSUNGCORELIB_EXPORT timeBetween(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGCORELIB_EXPORT timeAdd(const QTime &l, const QTime &r);
QTime ZEITERFASSUNGCORELIB_EXPORT timeNormalise(const QTime &time);
