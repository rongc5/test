

#include "config_filter_model.h"

CConfigFilterModel::CConfigFilterModel (QObject *)
{
    m_head_list << "name";
    m_head_list << "ip";
    m_head_list << "port";

    QObject::connect(
        this, 
        SIGNAL (dataChanged(const QModelIndex & , const QModelIndex & , const QVector<int> & )), 
        this, 
        SLOT(slot_data_changed(const QModelIndex & , const QModelIndex & , const QVector<int> &))
        );
}
int CConfigFilterModel::rowCount(const QModelIndex &) const
{
    return m_recorder_list.count();
}
int CConfigFilterModel::columnCount(const QModelIndex &) const
{
    return 3;
}
QVariant CConfigFilterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())  
        return QVariant();  
    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return QVariant ();
    }
    if (role == Qt::EditRole)
    {
        return QVariant ();
    }
   
    FilterRecord pr =  m_recorder_list[index.row()];
    QVariant var;
    switch (index.column())
    {
    case 0:
        {
            var = pr.name.c_str();
        }
        break;
    case 1:
        {
            var = pr.ip.c_str();

        }
        break;
    case 2:
        {
            var = pr.port.c_str();

        }
        break;
    default:
        break;
    }

    return var;
}
bool CConfigFilterModel::setData(
    const QModelIndex & index, 
    const QVariant & value, 
    int role)
{
    if (role == Qt::EditRole)
    {
        if (index.isValid())
        {
            if (index.column() == 0 && !value.toString().isEmpty())
            {
                m_recorder_list[index.row ()].name = value.toString().toStdString();
            }
            if (index.column() == 1 && !value.toString().isEmpty())
            {
                m_recorder_list[index.row ()].ip = value.toString().toStdString();
            }
            if (index.column() == 2 && !value.toString().isEmpty())
            {
                m_recorder_list[index.row ()].port = value.toString().toStdString();
            }
            return true;
        }
    }

    return false;
}
QVariant CConfigFilterModel::headerData(int section, Qt::Orientation orientation,
    int role ) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    {
        return QVariant();
    }
    return m_head_list[section];
}
 Qt::ItemFlags CConfigFilterModel::flags(const QModelIndex & index) const
{
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
void CConfigFilterModel::add_record (const FilterRecord & record)
{
    int pos = m_recorder_list.count();
    if (pos < 50)
    {
        beginInsertRows(QModelIndex(), pos, pos);
        m_recorder_list.push_back(record);
        endInsertRows();
    }
}
void CConfigFilterModel::delete_record (int row)
{
    if (row >=0 && row < m_recorder_list.count())
    {
        
        int count = rowCount(QModelIndex ());
        beginRemoveRows (QModelIndex (), row, row);
        m_recorder_list.removeAt(row);
        endRemoveRows();
    }
}

void CConfigFilterModel::clear ()
{
    int count = rowCount(QModelIndex ());
    beginRemoveRows (QModelIndex (), 0, count - 1 >= 0 ? count - 1 : 0);
    m_recorder_list.clear ();
    endRemoveRows();
}
bool CConfigFilterModel::has_empty_data ()
{
    bool ret = false;
    QList <FilterRecord>::const_iterator cit
        = m_recorder_list.begin();
    for ( ; cit != m_recorder_list.end (); ++cit)
    {
        if (cit->name.empty() || cit->ip.empty() && cit->port.empty())
        {
            ret = true;
            break;
        }
    }
    return ret;
}
void CConfigFilterModel::slot_data_changed(const QModelIndex & topLeft, 
    const QModelIndex & bottomRight, 
    const QVector<int> & roles)
{
    int i = 0;
}
int CConfigFilterModel::get_filter_data (QList <FilterRecord> & recorder_list)
{
    recorder_list = m_recorder_list;
    return 0;
}
int CConfigFilterModel::set_filter_data (const QList <FilterRecord> & recorder_list)
{
    clear ();
    beginInsertRows(QModelIndex(), 0, recorder_list.count() - 1);
    m_recorder_list = recorder_list;
    endInsertRows();
    return 0;
}