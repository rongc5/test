#include "common_util.h"
#include "base_def.h"

int main(int argc, char *argv[])
{
    std::vector<std::string> t_vec;

    SplitString("/queryid?id=sz002285&history_days=5", "/", &t_vec, SPLIT_MODE_ONE);

    for (uint32_t i = 0; i< t_vec.size(); i++){
        printf("%s\n", t_vec[i].c_str());
    }

    return 0;
}
