#pragma once

#include <memory>

#include <QFrame>
#include <QTime>

#include "zeiterfassungguilib_global.h"
#include "replies/getdayinforeply.h"
#include "replies/getbookingsreply.h"
#include "replies/gettimeassignmentsreply.h"

class QBoxLayout;
class QLabel;
template <typename T> class QVector;

class MainWindow;

class ZEITERFASSUNGGUILIB_EXPORT StripsWidget : public QFrame
{
    Q_OBJECT

public:
    explicit StripsWidget(MainWindow &mainWindow, QWidget *parent = Q_NULLPTR);

    MainWindow &mainWindow() const;

    QBoxLayout *headerLayout() const;
    QBoxLayout *stripsLayout() const;

    QLabel *label0() const;
    QLabel *label1() const;

    const QDate &date() const;
    void setDate(const QDate &date);

    bool highlighted() const;
    void setHighlighted(bool highlighted);

    const GetDayinfoReply::Dayinfo &dayinfo() const;
    const QVector<GetBookingsReply::Booking> &bookings() const;
    const QVector<GetTimeAssignmentsReply::TimeAssignment> &timeAssignments() const;

    const QTime &timeAssignmentTime() const;
    const QTime &lastTimeAssignmentStart() const;
    const QTime &minimumTime() const;
    bool refreshing() const;
    bool refreshingDayinfo() const;
    bool refreshingBookings() const;
    bool refreshingTimeAssignments() const;
    bool startEnabled() const;
    bool endEnabled() const;

    void refresh();
    void refreshDayinfo();
    void refreshBookings(bool createLabel = true);
    void refreshTimeAssignments(bool createLabel = true);
    bool createStrips();
    void clearStrips();

Q_SIGNALS:
    void dateChanged(const QDate &date);
    void highlightedChanged(bool highlighted);

    void dayinfoChanged(const GetDayinfoReply::Dayinfo &dayinfo);
    void bookingsChanged(const QVector<GetBookingsReply::Booking> &bookings);
    void timeAssignmentsChanged(const QVector<GetTimeAssignmentsReply::TimeAssignment> &timeAssignments);

    void timeAssignmentTimeChanged(const QTime &timeAssignmentTime);
    void lastTimeAssignmentStartChanged(const QTime &lastTimeAssignmentStart);
    void minimumTimeChanged(const QTime &minimumTime);
    void refreshingChanged(bool refreshing);
    void refreshingDayinfoChanged(bool refreshingDayinfo);
    void refreshingBookingsChanged(bool refreshingBookings);
    void refreshingTimeAssignmentsChanged(bool refreshingTimeAssignments);
    void startEnabledChanged(bool startEnabled);
    void endEnabledChanged(bool endEnabled);

private Q_SLOTS:
    void getDayinfoFinished();
    void getBookingsFinished();
    void getTimeAssignmentsFinished();

private:
    void invalidateValues();    
    QString buildProjectString(const QString &project) const;

    QWidget *appendBookingStartStrip(int id, const QTime &time);
    QWidget *appendBookingEndStrip(int id, const QTime &time, const QTime &duration);
    QWidget *appendTimeAssignmentStrip(int id, const QTime &duration, const QString &project, const QString &subproject,
                                       const QString &workpackage, const QString &text);

    MainWindow &m_mainWindow;

    QBoxLayout *m_headerLayout;
    QBoxLayout *m_stripsLayout;

    QLabel *m_label[2];

    QDate m_date;
    bool m_highlighted;

    GetDayinfoReply::Dayinfo m_dayinfo;
    QVector<GetBookingsReply::Booking> m_bookings;
    QVector<GetTimeAssignmentsReply::TimeAssignment> m_timeAssignments;

    QTime m_timeAssignmentTime;
    QTime m_lastTimeAssignmentStart;
    QTime m_minimumTime;
    bool m_refreshing;
    bool m_refreshingDayinfo;
    bool m_refreshingBookings;
    bool m_refreshingTimeAssignments;
    bool m_startEnabled;
    bool m_endEnabled;

    std::unique_ptr<GetDayinfoReply> m_getDayinfoReply;
    std::unique_ptr<GetBookingsReply> m_getBookingsReply;
    std::unique_ptr<GetTimeAssignmentsReply> m_getTimeAssignmentsReply;
};
