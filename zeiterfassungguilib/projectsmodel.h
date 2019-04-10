#pragma once

#include <QAbstractListModel>
#include <QDate>

#include <memory>
#include <vector>

#include "replies/getcomboboxreply.h"

class ZeiterfassungApi;

class ProjectsModel : public QAbstractListModel
{
    Q_OBJECT

    struct Project : public GetComboboxReply::Item {
        Project(const QString &label, const QString &value, std::unique_ptr<GetComboboxReply> getWorkpackagesReply) :
            GetComboboxReply::Item(label, value),
            getWorkpackagesReply(std::move(getWorkpackagesReply))
        {}

        std::unique_ptr<GetComboboxReply> getWorkpackagesReply;

        std::vector<GetComboboxReply::Item> workpackages;
    };

public:
    explicit ProjectsModel(int userId, const QDate &date, ZeiterfassungApi &api, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QString getProjectLabel(const QString &projectId) const;

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
