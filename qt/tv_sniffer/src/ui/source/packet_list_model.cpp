
#include "packet_list_model.h"


CPacketListModel::CPacketListModel (QObject *parent)
    : QAbstractTableModel (parent)
{
    m_head_list << "No.";
    m_head_list << "Time";
    m_head_list << "Source";
    m_head_list << "Source Port";
    m_head_list << "Destination";
    m_head_list << "Destination Port";
    m_head_list << "Protocols";
    m_head_list << "Size";
}
int CPacketListModel::rowCount(const QModelIndex &) const
{
    return m_recorder_list.count();
}
int CPacketListModel::columnCount(const QModelIndex &) const
{
    return 8;
}
QVariant CPacketListModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid())  
        return QVariant();  
    if (role != Qt::DisplayRole)
    {
        return QVariant ();
    }
    Packet_Record pr =  m_recorder_list[index.row()];
    QVariant var = "";
    switch (index.column())
    {
    case 0:
        {
            if (!pr.no.empty())
            {
                var = pr.no.c_str();
            }
            
        }
        break;
    case 1:
        {
            if (!pr.time.empty())
            {
                var = pr.time.c_str();
            }
            

        }
        break;
    case 2:
        {
            if (!pr.src_ip.empty())
            {
                var = pr.src_ip.c_str();
            }
            
        }
        break;
    case 3:
        {
            if (!pr.src_port.empty())
            {
                var = pr.src_port.c_str();
            }
            

        }
        break;
    case 4:
        {
            if (!pr.dst_ip.empty())
            {
                var = pr.dst_ip.c_str();
            }
            
        }
        break;
    case 5:
        {
            if (!pr.dst_port.empty())
            {
                var = pr.dst_port.c_str();
            }
        }
        break;
    case 6:
        {
            if (!pr.protocol.empty())
            {
                var = pr.protocol.c_str();
            }
            
        }
        break;
    case 7:
        {
            if (!pr.packet_size.empty())
            {
                var = pr.packet_size.c_str();
            }
        }
        break;
    default:
        break;
    }
    return var;
}
QVariant CPacketListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    {
        return QVariant();
    }
    return m_head_list[section];
 
}
void CPacketListModel::add_packet (const Packet_Record & packet_record)
{
    //insertRow(rowCount(QModelIndex ()));
    if (m_recorder_list.count() == 10000 * 5)
    {
        beginRemoveRows(QModelIndex (), 0, 0);
        m_recorder_list.pop_front();
        endRemoveRows();
        
    }

    int pos = m_recorder_list.count();
    beginInsertRows(QModelIndex(), pos, pos);
    m_recorder_list.push_back(packet_record);
    endInsertRows();
  
   

}
void CPacketListModel::clear ()
{
    if (m_recorder_list.empty())
    {
        return;
    }
    int count = rowCount(QModelIndex ());
    beginRemoveRows (QModelIndex (), 0, count - 1);
    m_recorder_list.clear ();
    endRemoveRows();

 
}
