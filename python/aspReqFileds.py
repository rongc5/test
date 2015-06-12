#!/app/ecom/pypy-1.9/bin/pypy
#coding=utf-8

import sys

def main(filename):
    allRecords = {};
    f = open(filename);
    line = f.readline();
    while line:
        if not line.startswith("NOTICE"):
            continue;
        record = {}
        items = line.split(" ")
        for item in items:
            item = item.strip()
            sub_items = item.split("=")
            if len(sub_items) < 2:
                continue
            record[sub_items[0]] = sub_items[1]
        #record = eval(line);
        #print(record);
        for k in record:
            #print(k);
            if k in allRecords:
                continue;
            else:
                allRecords[k] = k;
        line = f.readline();
    f.close();

    myStr = "";
    recordFile = open("records", "w+");
    for k in allRecords:
        myStr += k;
        myStr += ',';
        #print(k);
        pass

    print(myStr);
    recordFile.write(myStr);
    recordFile.close();
    pass

if '__main__' == __name__:
    if len(sys.argv) != 2:
        print("参数个数不对!\n");
        sys.exit(0);

    main(sys.argv[1]);
    pass
