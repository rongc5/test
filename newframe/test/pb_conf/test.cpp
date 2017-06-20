#include "ad.pb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <cstdio>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>

using namespace std;
int main(int argc, char** argv){
    if (argc <2){
        printf("programe reloadfile\n");
        exit(1);
    }

    BidResponse c;
    int fileDescriptor = open(argv[1], O_RDONLY);
    if( fileDescriptor < 0 ){
        return -1;
    }
    google::protobuf::io::FileInputStream fileInput(fileDescriptor);
    fileInput.SetCloseOnDelete( true );
    if (!google::protobuf::TextFormat::Parse(&fileInput, &c)){
        return -2;
    }
    cout<<"request_id:" <<c.request_id() <<endl;
    cout<<"ads number:"<<c.ads_size()<<endl;
    for (int i = 0 ; i < c.ads_size(); i++){
        cout <<"ad_id:"<<c.ads(i).ad_id()<<endl;
        cout <<"desc:" << c.ads(i).desc()<<endl;
    }
    return 0;
}
