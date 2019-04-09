#pragma once

#include <QSettings>
#include <QLocale>
#include <QUrl>
#include <QString>
#include <QStringList>
#include <QDate>

#include "zeiterfassungcorelib_global.h"

class ZEITERFASSUNGCORELIB_EXPORT ZeiterfassungSettings : public QSettings
{
    Q_OBJECT
    Q_PROPERTY(QLocale::Language language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString username READ username WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)
    Q_PROPERTY(QStringList projects READ projects WRITE setProjects NOTIFY projectsChanged)
    Q_PROPERTY(QStringList workpackages READ workpackages WRITE setWorkpackages NOTIFY workpackagesChanged)
    Q_PROPERTY(QStringList texts READ texts WRITE setTexts NOTIFY textsChanged)
    Q_PROPERTY(QString theme READ theme WRITE setTheme NOTIFY themeChanged)

public:
    explicit ZeiterfassungSettings(const QString &organization,
                                   const QString &application = QString(), QObject *parent = Q_NULLPTR);
    ZeiterfassungSettings(Scope scope, const QString &organization,
                          const QString &application = QString(), QObject *parent = Q_NULLPTR);
    ZeiterfassungSettings(Format format, Scope scope, const QString &organization,
                          const QString &application = QString(), QObject *parent = Q_NULLPTR);
    ZeiterfassungSettings(const QString &fileName, Format format, QObject *parent = Q_NULLPTR);
    explicit ZeiterfassungSettings(QObject *parent = Q_NULLPTR);

    QLocale::Language language() const;
    bool setLanguage(QLocale::Language language);

    QUrl url() const;
    bool setUrl(const QUrl &url);

    QString username() const;
    bool setUsername(const QString &username);

    QString password() const;
    bool setPassword(const QString &password);

    QStringList projects() const;
    bool setProjects(const QStringList &projects);
    bool prependProject(const QString &project);

    QStringList workpackages() const;
    bool setWorkpackages(const QStringList &workpackages);
    bool prependWorkpackage(const QString &workpackage);

    QStringList texts() const;
    bool setTexts(const QStringList &texts);
    bool prependText(const QString &text);

    QString theme() const;
    bool setTheme(const QString &theme);

Q_SIGNALS:
    void saveErrorOccured();

    void languageChanged(QLocale::Language language);
    void urlChanged(const QUrl &url);
    void usernameChanged(const QString &username);
    void passwordChanged(const QString &password);
    void projectsChanged(const QStringList &projects);
    void workpackagesChanged(const QStringList &workpackages);
    void textsChanged(const QStringList &texts);
    void themeChanged(const QString &theme);

private:
    QStringList prependItem(QStringList list, const QString &item);

    static const QString m_language;
    static const QString m_url;
    static const QString m_username;
    static const QString m_password;
    static const QString m_projects;
    static const QString m_workpackages;
    static const QString m_texts;
    static const QString m_theme;
    static const QLocale::Language m_defaultLanguage;
    static const QUrl m_defaultUrl;
};
