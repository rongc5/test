# -*- coding: utf-8 -*-
import os, json
import requests
from pbgen.app_lianmeng_api_pb2 import BidRequest, BidResponse


def copy_dict_to_pb(pb, d):
    for key, value in d.iteritems():
        if value is None:
            continue
        # If the value is another dictionary set the field to the values
        # in the nested dictionary
        if isinstance(value, dict):
            attribute = getattr(pb, key)
            copy_dict_to_pb(attribute, value)
        # If the value is iterable, copy the list into the repeated field
        elif hasattr(value, "__iter__"):
            attribute = getattr(pb, key)
            if len(value) == 0 or not isinstance(value[0], dict):
                attribute.extend(value)
            else:
                for item in value:
                    copy_dict_to_pb(attribute.add(), item)
        # Otherwise the value is a basic type, so set the field directly
        else:
            setattr(pb, key, value)

def get_request_dict(filename):
    cur_path = os.path.realpath(__file__)
    _file = file(os.path.dirname(cur_path) + '/' + filename)
    return json.load(_file)

def test():
    ad_dict = get_request_dict('request.json')
    bid_request = BidRequest()
    copy_dict_to_pb(bid_request, ad_dict)
    url = 'http://i.snssdk.com/api/ad/union/get_ads/'
    header = {'Content-type': 'application/octet-stream'}
    result = requests.post(url, data=bid_request.SerializeToString(), headers=header)
    bid_response = BidResponse()
    bid_response.ParseFromString(result.content)
    print bid_response

if __name__ == '__main__':
    test()
