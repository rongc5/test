#!/bin/bash
source /etc/profile
python /data2/mingz/.stock/script/load_ids.py
python /data2/mingz/.stock/script/load_finance.py
python /data2/mingz/.stock/script/load_brief.py
python /data2/mingz/.stock/script/load_plate.py
#python load_plate.py
