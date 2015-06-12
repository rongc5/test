
#ifndef _FILTER_FILE_H_
#define _FILTER_FILE_H_
#include <vector>
#include <string>

class FilterRecord;

class CFilterFile
{
public:

    CFilterFile (const std::string & save_file);
    bool save_filter_record_to_file(const std::vector<FilterRecord> & v_filter_record);
    bool get_filter_record_from_file (std::vector<FilterRecord> & v_filter_record);
private:
    std::string m_save_file;
};


#endif