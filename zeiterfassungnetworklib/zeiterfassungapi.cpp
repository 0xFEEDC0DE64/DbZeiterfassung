#include "zeiterfassungapi.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QStringBuilder>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include "replies/loginpagereply.h"
#include "replies/loginreply.h"
#include "replies/getuserinforeply.h"
#include "replies/getbookingsreply.h"
#include "replies/createbookingreply.h"
#include "replies/updatebookingreply.h"
#include "replies/deletebookingreply.h"
#include "replies/gettimeassignmentsreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/updatetimeassignmentreply.h"
#include "replies/deletetimeassignmentreply.h"
#include "replies/getcomboboxreply.h"
#include "replies/getreportreply.h"
#include "replies/getpresencestatusreply.h"
#include "replies/getabsencesreply.h"
#include "replies/getdayinforeply.h"

//add support for pre cpp14 compilers
#include "cpp14polyfills.h"

ZeiterfassungApi::ZeiterfassungApi(const QUrl &url, QObject *parent) :
    QObject(parent),
    m_url(url),
    m_manager(new QNetworkAccessManager(this))
{
}

const QUrl &ZeiterfassungApi::url() const
{
    return m_url;
}

void ZeiterfassungApi::setUrl(const QUrl &url)
{
    m_url = url;
}

QNetworkAccessManager *ZeiterfassungApi::manager() const
{
    return m_manager;
}

std::unique_ptr<LoginPageReply> ZeiterfassungApi::doLoginPage()
{
    QNetworkRequest request(QUrl(m_url.toString() % "pages/login.jsp"));

    return std::make_unique<LoginPageReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<LoginReply> ZeiterfassungApi::doLogin(const QString &username, const QString &password)
{
    QNetworkRequest request(QUrl(m_url.toString() % "pages/j_spring_security_check"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/x-www-form-urlencoded"));
    request.setMaximumRedirectsAllowed(0);

    auto data = QStringLiteral("j_username=%0&j_password=%1&login=Anmelden").arg(username).arg(password).toUtf8();

    return std::make_unique<LoginReply>(std::unique_ptr<QNetworkReply>(m_manager->post(request, data)), this);
}

std::unique_ptr<GetUserInfoReply> ZeiterfassungApi::doUserInfo()
{
    QNetworkRequest request0(QUrl(m_url.toString() % "json/evoAppsUserInfoDialogController/load-EvoAppsUserInfoTO"));
    request0.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("home"));

    QNetworkRequest request1(QUrl(m_url.toString() % "json/persons"));
    request1.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetUserInfoReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request0)),
                                              std::unique_ptr<QNetworkReply>(m_manager->get(request1)), this);
}

std::unique_ptr<GetBookingsReply> ZeiterfassungApi::doGetBookings(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/bookings?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url.toString())
                                 .arg(formatDate(start))
                                 .arg(formatDate(end))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetBookingsReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<CreateBookingReply> ZeiterfassungApi::doCreateBooking(int userId, const QDate &date, const QTime &time, const QTime &timespan,
                                                                      const QString &type, const QString &text)
{
    QNetworkRequest request(QUrl(m_url.toString() % "json/booking"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = formatDate(date).toInt();
    obj[QStringLiteral("bookingTime")] = formatTime(time).toInt();
    obj[QStringLiteral("bookingTimespan")] = formatTime(timespan).toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    auto data = QJsonDocument(obj).toJson();

    return std::make_unique<CreateBookingReply>(std::unique_ptr<QNetworkReply>(m_manager->post(request, data)), this);
}

std::unique_ptr<UpdateBookingReply> ZeiterfassungApi::doUpdateBooking(int bookingId, int userId, const QDate &date, const QTime &time,
                                                                      const QTime &timespan, const QString &type, const QString &text)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1").arg(m_url.toString()).arg(bookingId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = bookingId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = formatDate(date).toInt();
    obj[QStringLiteral("bookingTime")] = formatTime(time).toInt();
    obj[QStringLiteral("bookingTimespan")] = formatTime(timespan).toInt();
    obj[QStringLiteral("bookingType")] = type;
    obj[QStringLiteral("hourCategory")] = QStringLiteral("");
    obj[QStringLiteral("empfEinh")] = QStringLiteral("");
    obj[QStringLiteral("bewEinh")] = QStringLiteral("");
    obj[QStringLiteral("text")] = text;

    auto data = QJsonDocument(obj).toJson();

    return std::make_unique<UpdateBookingReply>(std::unique_ptr<QNetworkReply>(m_manager->put(request, data)), this);
}

std::unique_ptr<DeleteBookingReply> ZeiterfassungApi::doDeleteBooking(int bookingId)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/booking/%1?text=")
                                 .arg(m_url.toString())
                                 .arg(bookingId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<DeleteBookingReply>(std::unique_ptr<QNetworkReply>(m_manager->deleteResource(request)), this);
}

std::unique_ptr<GetTimeAssignmentsReply> ZeiterfassungApi::doGetTimeAssignments(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url.toString())
                                 .arg(formatDate(start))
                                 .arg(formatDate(end))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetTimeAssignmentsReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<CreateTimeAssignmentReply> ZeiterfassungApi::doCreateTimeAssignment(int userId, const QDate &date, const QTime &time,
                                                                                    const QTime &timespan, const QString &project,
                                                                                    const QString &workpackage, const QString &text)
{
    QNetworkRequest request(QUrl(m_url.toString() % "json/azebooking"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = QJsonValue::Null;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = formatDate(date).toInt();
    obj[QStringLiteral("bookingTime")] = formatTime(time).toInt();
    obj[QStringLiteral("bookingTimespan")] = formatTime(timespan).toInt();
    obj[QStringLiteral("text")] = text;
    {
        QJsonArray koWertList;
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = project;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = workpackage;
            koWertList << obj;
        }
        obj[QStringLiteral("koWertList")] = koWertList;
    }

    auto data = QJsonDocument(obj).toJson();

    return std::make_unique<CreateTimeAssignmentReply>(std::unique_ptr<QNetworkReply>(m_manager->post(request, data)), this);
}

std::unique_ptr<UpdateTimeAssignmentReply> ZeiterfassungApi::doUpdateTimeAssignment(int timeAssignmentId, int userId, const QDate &date,
                                                                                    const QTime &time, const QTime &timespan, const QString &project,
                                                                                    const QString &workpackage, const QString &text)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1").arg(m_url.toString()).arg(timeAssignmentId)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/json"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    QJsonObject obj;
    obj[QStringLiteral("bookingNr")] = timeAssignmentId;
    obj[QStringLiteral("persNr")] = userId;
    obj[QStringLiteral("bookingDate")] = formatDate(date).toInt();
    obj[QStringLiteral("bookingTime")] = formatTime(time).toInt();
    obj[QStringLiteral("bookingTimespan")] = formatTime(timespan).toInt();
    obj[QStringLiteral("bookingType")] = QJsonValue::Null;
    obj[QStringLiteral("hourCategory")] = QJsonValue::Null;
    obj[QStringLiteral("bewEinh")] = QJsonValue::Null;
    obj[QStringLiteral("empfEinh")] = QJsonValue::Null;
    obj[QStringLiteral("einstuf")] = 0;
    obj[QStringLiteral("text")] = text;
    {
        QJsonArray koWertList;
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = project;
            koWertList << obj;
        }
        {
            QJsonObject obj;
            obj[QStringLiteral("value")] = workpackage;
            koWertList << obj;
        }
        obj[QStringLiteral("koWertList")] = koWertList;
    }

    auto data = QJsonDocument(obj).toJson();

    return std::make_unique<UpdateTimeAssignmentReply>(std::unique_ptr<QNetworkReply>(m_manager->put(request, data)), this);
}

std::unique_ptr<DeleteTimeAssignmentReply> ZeiterfassungApi::doDeleteTimeAssignment(int timeAssignmentId)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/azebooking/%1")
                                 .arg(m_url.toString())
                                 .arg(timeAssignmentId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<DeleteTimeAssignmentReply>(std::unique_ptr<QNetworkReply>(m_manager->deleteResource(request)), this);
}

std::unique_ptr<GetComboboxReply> ZeiterfassungApi::doGetCostcentres(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/combobox?persnr=%1&date=%2&dqkey=KOST")
                                 .arg(m_url.toString())
                                 .arg(userId)
                                 .arg(formatDate(date))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetComboboxReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<GetComboboxReply> ZeiterfassungApi::doGetProjects(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/combobox?persnr=%1&date=%2&dqkey=PROJEKT")
                                 .arg(m_url.toString())
                                 .arg(userId)
                                 .arg(formatDate(date))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetComboboxReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<GetComboboxReply> ZeiterfassungApi::doGetWorkpackages(int userId, const QDate &date, const QString &project)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/combobox?persnr=%1&date=%2&dqkey=WORKPACKAGE&kowert0=%3")
                                 .arg(m_url.toString())
                                 .arg(userId)
                                 .arg(formatDate(date))
                                 .arg(project)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetComboboxReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<GetReportReply> ZeiterfassungApi::doGetReport(int userId, const QDate &date)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/auswertung/month?persNr=%1&date=%2")
                                 .arg(m_url.toString())
                                 .arg(userId)
                                 .arg(formatDate(date))));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetReportReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<GetPresenceStatusReply> ZeiterfassungApi::doGetPresenceStatus()
{
    QNetworkRequest request(QUrl(m_url.toString() % "json/presencestatus"));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("presenceStatus"));

    return std::make_unique<GetPresenceStatusReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<GetAbsencesReply> ZeiterfassungApi::doGetAbsences(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/fulldayAbsences?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url.toString())
                                 .arg(formatDate(start))
                                 .arg(formatDate(end))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetAbsencesReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

std::unique_ptr<GetDayinfoReply> ZeiterfassungApi::doGetDayinfo(int userId, const QDate &start, const QDate &end)
{
    QNetworkRequest request(QUrl(QStringLiteral("%0json/dayinfo?start=%1&end=%2&pnrLst=%3")
                                 .arg(m_url.toString())
                                 .arg(formatDate(start))
                                 .arg(formatDate(end))
                                 .arg(userId)));
    request.setRawHeader(QByteArrayLiteral("sisAppName"), QByteArrayLiteral("bookingCalendar"));

    return std::make_unique<GetDayinfoReply>(std::unique_ptr<QNetworkReply>(m_manager->get(request)), this);
}

QString ZeiterfassungApi::formatDate(const QDate &date)
{
    return date.toString(QStringLiteral("yyyyMMdd"));
}

QString ZeiterfassungApi::formatTime(const QTime &time)
{
    return time.toString(QStringLiteral("Hmmss"));
}
