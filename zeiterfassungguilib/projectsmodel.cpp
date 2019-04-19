#include "projectsmodel.h"

#include <QFont>

#include "zeiterfassungapi.h"

ProjectsModel::ProjectsModel(int userId, const QDate &date, ZeiterfassungApi &api, QObject *parent) :
    QAbstractListModel(parent),
    m_userId(userId),
    m_date(date),
    m_api(api)
{
    m_getProjectsReply = m_api.doGetProjects(m_userId, m_date);
    connect(m_getProjectsReply.get(), &ZeiterfassungReply::finished, this, &ProjectsModel::getProjectsFinished);
}

int ProjectsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (m_getProjectsReply != nullptr)
        return 1;

    int rows { 0 };
    for (const auto &project : m_projects)
    {
        rows++; // header
        if (project.getWorkpackagesReply != nullptr)
            rows++; // loading or failed
        else
            rows += project.workpackages.size();
    }
    return rows;
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.column() != 0 || index.row() < 0)
        return {};

    if (m_getProjectsReply != nullptr)
    {
        if (index.row() != 0)
            return {};

        switch (role)
        {
        case Qt::DisplayRole:
            if (m_getProjectsReply->isFinished())
            {
                Q_ASSERT(!m_getProjectsReply->success());
                return m_getProjectsReply->message();
            }
            else
                return tr("Loading projects...");
        default:
            return {};
        }
    }

    int row { 0 };
    for (const auto &project : m_projects)
    {
        if (row++ == index.row())
        {
            switch (role)
            {
            case Qt::DisplayRole:
                return project.value + " (" + project.label + ')';
            case Qt::EditRole:
                return project.value;
            case Qt::FontRole:
            {
                QFont font;
                font.setBold(true);
                return font;
            }
            default:
                return {};
            }
        }

        if (project.getWorkpackagesReply != nullptr)
        {
            if (row++ == index.row())
            {
                switch (role)
                {
                case Qt::DisplayRole:
                    if (project.getWorkpackagesReply->isFinished())
                    {
                        Q_ASSERT(!project.getWorkpackagesReply->success());
                        return project.getWorkpackagesReply->message();
                    }
                    else
                        return tr("Loading workpackages...");
                default:
                    return {};
                }
            }
        }
        else
        {
            for (const auto &workpackage : project.workpackages)
            {
                if (row++ == index.row())
                {
                    switch (role)
                    {
                    case Qt::DisplayRole:
                        return workpackage.label + " (" + workpackage.value + ')';
                    case Qt::EditRole:
                        return workpackage.label;
                    case Qt::UserRole:
                        return project.value;
                    default:
                        return {};
                    }
                }
            }
        }
    }

    return {};
}

Qt::ItemFlags ProjectsModel::flags(const QModelIndex &index) const
{
    auto flags = QAbstractListModel::flags(index);
    if (!isSelectable(index))
        flags &=~Qt::ItemIsSelectable;
    return flags;
}

QString ProjectsModel::getProjectLabel(const QString &projectId) const
{
    const auto iter = std::find_if(m_projects.begin(), m_projects.end(), [&projectId](const Project &project){ return project.value == projectId; });
    if (iter == m_projects.end())
        return {};

    return iter->label;
}

void ProjectsModel::getProjectsFinished()
{
    if (!m_getProjectsReply->success())
    {
        emit dataChanged(createIndex(0, 0), createIndex(0, 0), { Qt::DisplayRole, Qt::EditRole });
        return;
    }

    emit beginResetModel();
    for (const auto &item : m_getProjectsReply->items())
    {
        const auto index = m_projects.size();
        m_projects.emplace_back(item.label, item.value, m_api.doGetWorkpackages(m_userId, m_date, item.value));
        auto &project = m_projects.back();
        connect(project.getWorkpackagesReply.get(), &ZeiterfassungReply::finished, this, [this,index](){ getWorkspacesFinished(index); });
    }
    m_getProjectsReply = nullptr;
    emit endResetModel();
}

void ProjectsModel::getWorkspacesFinished(int index)
{
    getWorkspacesFinished(m_projects[index]);
}

void ProjectsModel::getWorkspacesFinished(ProjectsModel::Project &project)
{
    if (!project.getWorkpackagesReply->success())
    {
        //TODO: improve performance
        emit beginResetModel();
        emit endResetModel();
        return;
    }

    for (const auto &item : project.getWorkpackagesReply->items())
        project.workpackages.emplace_back(item.value, item.label);

    project.getWorkpackagesReply = nullptr;

    //TODO: improve performance
    emit beginResetModel();
    emit endResetModel();
}

bool ProjectsModel::isSelectable(const QModelIndex &index) const
{
    if (!index.isValid() || index.column() != 0 || index.row() < 0)
        return {};

    if (m_getProjectsReply != nullptr)
    {
        if (index.row() != 0)
            return {};

        return false;
    }

    int row { 0 };
    for (const auto &project : m_projects)
    {
        if (row++ == index.row())
            return false;

        if (project.getWorkpackagesReply != nullptr)
        {
            if (row++ == index.row())
                return false;
        }
        else
        {
            for (const auto &workpackage : project.workpackages)
            {
                Q_UNUSED(workpackage)
                if (row++ == index.row())
                    return true;
            }
        }
    }

    return {};
}
