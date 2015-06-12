#include <QSettings>
#include <iostream>
#include "data_struct.h"
#include "filter_file.h"


CFilterFile::CFilterFile (const std::string & save_file)
    : m_save_file (save_file)
{

}
bool CFilterFile::save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record)
{
    QSettings setting(m_save_file.c_str(), QSettings::IniFormat);
    setting.clear();
    setting.beginWriteArray("filer");
    std::vector<FilterRecord>::const_iterator cit
        = v_filter_record.begin();
    int i = 0;
    for ( ; cit != v_filter_record.end (); ++cit)
    {
        std::cout << "save\n";
        setting.setArrayIndex(i++);
        setting.setValue("name", cit->name.c_str());
        setting.setValue("ip", cit->ip.c_str());
        setting.setValue("port", cit->port.c_str());
    }
    setting.endArray();
    return true;
}
bool CFilterFile::get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record)
{
    std::cout << "get_filter_record_from_file\n";
    std::cout << m_save_file.c_str () << "\n";
     QSettings setting(m_save_file.c_str(), QSettings::IniFormat);
    int size = setting.beginReadArray("filer");
    std::cout << size << "\n";

    for ( int i = 0; i < size; ++i)
    {
        FilterRecord fr;
        setting.setArrayIndex(i);
        fr.name = setting.value("name").toString().toStdString();
        fr.ip = setting.value("ip").toString().toStdString();
        fr.port = setting.value("port").toString().toStdString();
        v_filter_record.push_back(fr);
    }
    setting.endArray();
    return true;
}