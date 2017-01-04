#include "base_data_process.h"
#include "log_helper.h"

size_t base_data_process::process_recv_buf(char *buf, size_t len)
{
    _s_buf.clear();
	_s_buf.append(buf, len);

    LOG_DEBUG("process_recv_buf .. ");

	return len;
}



