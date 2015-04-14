
#include <QDialog>
#include <vector>
#include <vector>
#include <iostream>
#include "data_struct.h"
#include "iview.h"
#include "config_filter_model.h"
#include "filter_delegate.h"
#include "config_filter_dlg.h"




CConfigFilterDlg::CConfigFilterDlg (IView * view, QWidget * parent)
    : QDialog (parent), m_view (view)
{
    m_ui_config_filter_dlg.setupUi(this);
    m_filter_model = new CConfigFilterModel (this);
    m_filter_delegate = new CFilterDelegate (this);
    m_ui_config_filter_dlg.tableView->setModel(m_filter_model);
      m_ui_config_filter_dlg.tableView->setItemDelegate (m_filter_delegate);
    m_ui_config_filter_dlg.tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
   
    if (view != NULL)
    {
        std::cout << "get_filter_record_from_file begin\n";
        std::vector <FilterRecord> v_filter_record;
        view->get_filter_record_from_file(v_filter_record);
        std::cout << "get_filter_record_from_file end\n";
        std::vector <FilterRecord>::const_iterator cit
            = v_filter_record.begin();
        for ( ; cit != v_filter_record.end (); ++cit)
        {
            std::cout << "add record\n";
            m_filter_model->add_record(*cit);
        }
    }
  
    std::cout << "CConfigFilterDlg end\n";
}
bool CConfigFilterDlg::has_empty_data ()
{
    return m_filter_model->has_empty_data();
}
int CConfigFilterDlg::get_filter_data (QList <FilterRecord> & recorder_list)
{
    return m_filter_model->get_filter_data (recorder_list);
}
int CConfigFilterDlg::set_filter_data (const QList <FilterRecord> & recorder_list)
{
   
    return m_filter_model->set_filter_data (recorder_list);
}
void CConfigFilterDlg::set_view (IView * view)
{
    m_view = view;
    if (m_view != NULL)
    {
        std::vector <FilterRecord> v_filter_record;
        m_view->get_filter_record_from_file(v_filter_record);
        std::cout << "get_filter_record_from_file end\n";
        std::vector <FilterRecord>::const_iterator cit
            = v_filter_record.begin();
        for ( ; cit != v_filter_record.end (); ++cit)
        {
            std::cout << "name = " << cit->name.c_str () << "\n";
            std::cout << "ip = " << cit->ip.c_str () << "\n";
            std::cout << "port = " << cit->port.c_str () << "\n";
            std::cout << "add record\n";
            m_filter_model->add_record(*cit);
        }
    }
}
void CConfigFilterDlg::on_add_btn_clicked ()
{
     
    m_filter_model->add_record(FilterRecord());
}
void CConfigFilterDlg::on_delete_btn_clicked ()
{
    QModelIndex index = m_ui_config_filter_dlg.tableView->currentIndex();
    if (index.isValid())
    {
        m_filter_model->delete_record(index.row());
    }

}
void CConfigFilterDlg::save_filter_data ()
{
    std::cout << "save_filter_data\n";
    QList <FilterRecord>  recorder_list;
    std::vector <FilterRecord> v_filter_record;
    m_filter_model->get_filter_data(recorder_list);
    QList <FilterRecord>::const_iterator cit
        = recorder_list.begin();
    for ( ; cit != recorder_list.end (); ++cit)
    {
        std::cout << "name = " << cit->name.c_str () << "\n";
        std::cout << "ip = " << cit->ip.c_str () << "\n";
        std::cout << "port = " << cit->port.c_str () << "\n";
        v_filter_record.push_back(*cit);
    }
    std::cout << "record count = " << v_filter_record.size() << "\n";
    m_view->save_filter_record_to_file(v_filter_record);
}