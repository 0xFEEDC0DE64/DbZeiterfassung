#pragma once

#include <QAbstractListModel>
#include <QDate>

#include <memory>
#include <vector>

class ZeiterfassungApi;
class GetComboboxReply;

class ProjectsModel : public QAbstractListModel
{
    Q_OBJECT

    struct Project {
        Project(const QString &label, const QString &value, std::unique_ptr<GetComboboxReply> getWorkpackagesReply) :
            label(label), value(value), getWorkpackagesReply(std::move(getWorkpackagesReply))
        {}

        QString label;
        QString value;

        std::unique_ptr<GetComboboxReply> getWorkpackagesReply;

        std::vector<std::pair<QString, QString> > workpackages;
    };

public:
    explicit ProjectsModel(int userId, const QDate &date, ZeiterfassungApi &api, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void getProjectsFinished();
    void getWorkspacesFinished(int index);
    void getWorkspacesFinished(Project &project);

private:
    bool isSelectable(const QModelIndex &index) const;

    const int m_userId;
    const QDate m_date;
    ZeiterfassungApi &m_api;

    std::unique_ptr<GetComboboxReply> m_getProjectsReply;
    std::vector<Project> m_projects;
};
