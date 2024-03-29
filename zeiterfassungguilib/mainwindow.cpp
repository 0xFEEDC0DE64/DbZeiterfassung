#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QUrl>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QStringBuilder>
#include <QMenu>
#include <QLabel>
#include <QDir>
#include <QTemporaryFile>
#include <QDesktopServices>
#include <QRegularExpression>
#include <QTimerEvent>
#include <QSignalBlocker>
#include <QDebug>

#include "zeiterfassungapi.h"
#include "utils/timeutils.h"
#include "zeiterfassungsettings.h"
#include "stripfactory.h"
#include "stripswidget.h"
#include "dialogs/settingsdialog.h"
#include "replies/getcomboboxreply.h"
#include "replies/createbookingreply.h"
#include "replies/createtimeassignmentreply.h"
#include "replies/updatetimeassignmentreply.h"

MainWindow::MainWindow(ZeiterfassungSettings &settings, ZeiterfassungApi &erfassung, const GetUserInfoReply::UserInfo &userInfo,
                       StripFactory &stripFactory, const QSet<ZeiterfassungPlugin*> &plugins, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_settings(settings),
    m_erfassung(erfassung),
    m_userInfo(userInfo),
    m_stripFactory(stripFactory),
    m_plugins(plugins),
    m_currentStripWidget(Q_NULLPTR),
    m_timerId(-1),
    m_projectsModel(m_userInfo.userId, QDate::currentDate(), m_erfassung)
{
    ui->setupUi(this);

    setWindowTitle(tr("Zeiterfassung - %0 (%1)").arg(m_userInfo.text).arg(m_userInfo.email));

    ui->actionQuit->setShortcut(QKeySequence::Quit);

    connect(ui->actionToday, &QAction::triggered, this, [=](){ ui->dateEditDate->setDate(QDate::currentDate()); });

    ui->actionRefresh->setShortcut(QKeySequence::Refresh);
    connect(ui->actionRefresh, &QAction::triggered, this, &MainWindow::refreshEverything);

    connect(ui->actionSettings, &QAction::triggered, this, [this](){ SettingsDialog(m_settings, m_plugins, this).exec(); });

    ui->actionHelp->setShortcut(QKeySequence::HelpContents);

    connect(ui->actionAboutQt, &QAction::triggered, this, [this](){ QMessageBox::aboutQt(this); });

    ui->dateEditDate->setDate(QDate::currentDate());
    connect(ui->dateEditDate, &QDateTimeEdit::dateChanged, this, &MainWindow::dateChangedSlot);

    connect(ui->pushButtonPrevWeek, &QAbstractButton::pressed, this, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(-7)); });
    connect(ui->pushButtonPrev, &QAbstractButton::pressed, this, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(-1)); });
    connect(ui->pushButtonNext, &QAbstractButton::pressed, this, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(1)); });
    connect(ui->pushButtonNextWeek, &QAbstractButton::pressed, this, [=](){ ui->dateEditDate->setDate(ui->dateEditDate->date().addDays(7)); });

    connect(ui->timeEditTime, &QTimeEdit::timeChanged, this, [&](){
        if(m_timerId != -1)
        {
            killTimer(m_timerId);
            m_timerId = -1;
        }
    });
    ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));

    connect(ui->pushButtonNow, &QAbstractButton::pressed, this, &MainWindow::pushButtonNowPressed);

    ui->comboBoxProject->setModel(&m_projectsModel);
    ui->comboBoxText->lineEdit()->setPlaceholderText(tr("Text"));

    updateComboboxes();

    connect(ui->pushButtonStart, &QAbstractButton::pressed, this, &MainWindow::pushButtonStartPressed);
    connect(ui->pushButtonEnd, &QAbstractButton::pressed, this, &MainWindow::pushButtonEndPressed);

    m_timerId = startTimer(60000);

    for(quint8 i = 0; i < 7; i++)
    {
        m_stripsWidgets[i] = new StripsWidget(*this, ui->widgetWeek);
        connect(this, &MainWindow::refreshEverything, m_stripsWidgets[i], &StripsWidget::refresh);
        ui->layoutWeek->addWidget(m_stripsWidgets[i]);
    }

    dateChangedSlot(ui->dateEditDate->date());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMenu *MainWindow::menuFile() const
{
    return ui->menuFile;
}

QMenu *MainWindow::menuView() const
{
    return ui->menuView;
}

QMenu *MainWindow::menuTools() const
{
    return ui->menuTools;
}

QMenu *MainWindow::menuAbout() const
{
    return ui->menuAbout;
}

QToolBar *MainWindow::toolBar() const
{
    return ui->mainToolBar;
}

ZeiterfassungSettings &MainWindow::settings() const
{
    return m_settings;
}

ZeiterfassungApi &MainWindow::erfassung() const
{
    return m_erfassung;
}

const GetUserInfoReply::UserInfo &MainWindow::userInfo() const
{
    return m_userInfo;
}

StripFactory &MainWindow::stripFactory() const
{
    return m_stripFactory;
}

QDate MainWindow::date() const
{
    return ui->dateEditDate->date();
}

void MainWindow::setDate(const QDate &date)
{
    ui->dateEditDate->setDate(date);
}

const std::array<StripsWidget*, 7> &MainWindow::stripsWidgets() const
{
    return m_stripsWidgets;
}

ProjectsModel &MainWindow::projectsModel()
{
    return m_projectsModel;
}

const ProjectsModel &MainWindow::projectsModel() const
{
    return m_projectsModel;
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId)
    {
        QSignalBlocker blocker(ui->timeEditTime);
        ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));
    }
    else
        QMainWindow::timerEvent(event);
}

void MainWindow::pushButtonNowPressed()
{
    ui->dateEditDate->setDate(QDate::currentDate());
    ui->timeEditTime->setTime(timeNormalise(QTime::currentTime()));
}

void MainWindow::pushButtonStartPressed()
{
    if (ui->comboBoxProject->currentIndex() == -1)
    {
        QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % tr("Please select a project/workpackage!"));
        return;
    }

    if(m_timerId != -1)
    {
        killTimer(m_timerId);
        m_timerId = -1;
    }

    if(m_currentStripWidget->bookings().rbegin() == m_currentStripWidget->bookings().rend() ||
       m_currentStripWidget->bookings().rbegin()->type == QStringLiteral("G"))
    {
        auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                 timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                                 QStringLiteral("K"), QStringLiteral(""));

        reply->waitForFinished();

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            goto after;
        }
    }

    {
        auto timeAssignmentTime = m_currentStripWidget->timeAssignmentTime();

        if(m_currentStripWidget->timeAssignments().rbegin() != m_currentStripWidget->timeAssignments().rend())
        {
            auto timeAssignment = *m_currentStripWidget->timeAssignments().rbegin();
            if(timeAssignment.timespan == QTime(0, 0))
            {
                auto timespan = timeBetween(m_currentStripWidget->lastTimeAssignmentStart(), ui->timeEditTime->time());

                auto reply = m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, timeAssignment.date,
                                                                timeAssignment.time, timespan,
                                                                timeAssignment.project, timeAssignment.workpackage, timeAssignment.text);

                reply->waitForFinished();

                if(reply->success())
                    timeAssignmentTime = timeAdd(timeAssignmentTime, timespan);
                else
                {
                    QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
                    m_currentStripWidget->refresh();
                    goto after;
                }
            }
        }

        {
            auto reply = m_erfassung.doCreateTimeAssignment(m_userInfo.userId, ui->dateEditDate->date(),
                                                            timeAssignmentTime, QTime(0, 0),
                                                            ui->comboBoxProject->currentData(Qt::UserRole).toString(),
                                                            ui->comboBoxProject->currentData(Qt::EditRole).toString(),
                                                            ui->comboBoxText->currentText());

            reply->waitForFinished();

            if(!reply->success())
            {
                QMessageBox::warning(this, tr("Could not create time assignment!"), tr("Could not create time assignment!") % "\n\n" % reply->message());
                m_currentStripWidget->refresh();
                goto after;
            }
        }
    }

    m_settings.setLastWorkpackageIndex(ui->comboBoxProject->currentIndex());
    m_settings.prependText(ui->comboBoxText->currentText());

    updateComboboxes();

    //m_currentStripWidget->refresh();
    //refreshReport();
    Q_EMIT refreshEverything();

    after:
    m_timerId = startTimer(60000);
}

void MainWindow::pushButtonEndPressed()
{
    if(m_timerId != -1)
    {
        killTimer(m_timerId);
        m_timerId = -1;
    }

    {
        auto timeAssignment = *m_currentStripWidget->timeAssignments().rbegin();
        Q_ASSERT(timeAssignment.timespan == QTime(0, 0));

        auto timespan = timeBetween(m_currentStripWidget->lastTimeAssignmentStart(), ui->timeEditTime->time());

        auto reply = m_erfassung.doUpdateTimeAssignment(timeAssignment.id, m_userInfo.userId, timeAssignment.date,
                                                        timeAssignment.time, timespan,
                                                        timeAssignment.project, timeAssignment.workpackage, timeAssignment.text);

        reply->waitForFinished();

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not edit time assignment!"), tr("Could not edit time assignment!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            goto after;
        }
    }

    {
        auto reply = m_erfassung.doCreateBooking(m_userInfo.userId, ui->dateEditDate->date(),
                                                 timeNormalise(ui->timeEditTime->time()), QTime(0, 0),
                                                 QStringLiteral("G"), QStringLiteral(""));

        reply->waitForFinished();

        if(!reply->success())
        {
            QMessageBox::warning(this, tr("Could not create booking!"), tr("Could not create booking!") % "\n\n" % reply->message());
            m_currentStripWidget->refresh();
            goto after;
        }
    }

    //m_currentStripWidget->refresh();
    //refreshReport();
    Q_EMIT refreshEverything();

    after:
    m_timerId = startTimer(60000);
}

void MainWindow::dateChangedSlot(const QDate &date)
{
    auto firstDayOfWeek = date.addDays(-(ui->dateEditDate->date().dayOfWeek() - 1));

    for(quint8 i = 0; i < 7; i++)
    {
        auto weekDay = firstDayOfWeek.addDays(i);

        m_stripsWidgets[i]->setDate(weekDay);

        if(weekDay == date && m_currentStripWidget != m_stripsWidgets[i])
        {
            if(m_currentStripWidget)
            {
                m_currentStripWidget->setHighlighted(false);
                disconnect(m_currentStripWidget, &StripsWidget::minimumTimeChanged, this, &MainWindow::minimumTimeChanged);
                disconnect(m_currentStripWidget, &StripsWidget::startEnabledChanged, this, &MainWindow::startEnabledChanged);
                disconnect(m_currentStripWidget, &StripsWidget::endEnabledChanged, this, &MainWindow::endEnabledChanged);
            }

            m_currentStripWidget = m_stripsWidgets[i];

            m_currentStripWidget->setHighlighted(true);

            minimumTimeChanged();
            startEnabledChanged();
            endEnabledChanged();

            connect(m_currentStripWidget, &StripsWidget::minimumTimeChanged, this, &MainWindow::minimumTimeChanged);
            connect(m_currentStripWidget, &StripsWidget::startEnabledChanged, this, &MainWindow::startEnabledChanged);
            connect(m_currentStripWidget, &StripsWidget::endEnabledChanged, this, &MainWindow::endEnabledChanged);
        }
    }

    Q_EMIT dateChanged(ui->dateEditDate->date());
}

void MainWindow::minimumTimeChanged()
{
    ui->timeEditTime->setMinimumTime(m_currentStripWidget->minimumTime());
}

void MainWindow::startEnabledChanged()
{
    auto startEnabled = m_currentStripWidget->startEnabled();
    auto endEnabled = m_currentStripWidget->endEnabled();

    ui->timeEditTime->setEnabled(startEnabled || endEnabled);
    ui->pushButtonNow->setEnabled(startEnabled || endEnabled);

    ui->comboBoxProject->setEnabled(startEnabled);
    ui->comboBoxText->setEnabled(startEnabled);

    ui->pushButtonStart->setEnabled(startEnabled);
    ui->pushButtonStart->setText(endEnabled ? tr("Switch") : tr("Start"));
}

void MainWindow::endEnabledChanged()
{
    auto startEnabled = m_currentStripWidget->startEnabled();
    auto endEnabled = m_currentStripWidget->endEnabled();

    ui->timeEditTime->setEnabled(startEnabled || endEnabled);
    ui->pushButtonNow->setEnabled(startEnabled || endEnabled);

    ui->pushButtonStart->setText(endEnabled ? tr("Switch") : tr("Start"));
    ui->pushButtonEnd->setEnabled(endEnabled);
}

void MainWindow::updateComboboxes()
{
    ui->comboBoxText->clear();

    {
        auto texte = m_settings.texts();
        for(const auto &text : texte)
            ui->comboBoxText->addItem(text);
        if(texte.count())
            ui->comboBoxText->setCurrentText(QString());
    }
}
