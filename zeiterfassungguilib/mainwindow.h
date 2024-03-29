#pragma once

#include <array>
#include <memory>

#include <QMainWindow>
#include <QMap>

#include "zeiterfassungguilib_global.h"
#include "replies/getuserinforeply.h"
#include "replies/getcomboboxreply.h"
#include "replies/getpresencestatusreply.h"
#include "projectsmodel.h"

class QMenu;
class QToolBar;
class QLabel;
class QBoxLayout;

namespace Ui { class MainWindow; }
class ZeiterfassungSettings;
class StripFactory;
class ZeiterfassungPlugin;
class StripsWidget;

class ZEITERFASSUNGGUILIB_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const GetUserInfoReply::UserInfo &userInfo,
                        StripFactory &stripFactory, const QSet<ZeiterfassungPlugin*> &plugins, QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    QMenu *menuFile() const;
    QMenu *menuView() const;
    QMenu *menuTools() const;
    QMenu *menuAbout() const;
    QToolBar *toolBar() const;

    ZeiterfassungSettings &settings() const;
    ZeiterfassungApi &erfassung() const;
    const GetUserInfoReply::UserInfo &userInfo() const;
    StripFactory &stripFactory() const;

    QDate date() const;
    void setDate(const QDate &date);

    const std::array<StripsWidget*, 7> &stripsWidgets() const;

    ProjectsModel &projectsModel();
    const ProjectsModel &projectsModel() const;

Q_SIGNALS:
    void dateChanged(const QDate &date);
    void refreshEverything();

protected:
    // QObject interface
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void pushButtonNowPressed();
    void pushButtonStartPressed();
    void pushButtonEndPressed();
    void dateChangedSlot(const QDate &date);

    void minimumTimeChanged();
    void startEnabledChanged();
    void endEnabledChanged();

private:
    void updateComboboxes();

    Ui::MainWindow *ui;
    ZeiterfassungSettings &m_settings;
    ZeiterfassungApi &m_erfassung;
    const GetUserInfoReply::UserInfo &m_userInfo;
    StripFactory &m_stripFactory;
    const QSet<ZeiterfassungPlugin*> &m_plugins;

    std::array<StripsWidget*, 7> m_stripsWidgets;
    StripsWidget *m_currentStripWidget;

    int m_timerId;

    ProjectsModel m_projectsModel;
};
