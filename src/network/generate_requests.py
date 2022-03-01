import json
import typing

from .constants import packet_encoding, EOF_string, listener_buffer


def File_transfer(filename: str = 'alora-share.txt', data_type: typing.Union[bytes, str('ping')] = 'ping'):
    headers = {
        "event-type": "data-transfer",
        "data-type": data_type,
        "metadata": {
            "filename": filename,
        }
    }
    return json.dumps(headers)

def map_request(request: str, chunk: int = listener_buffer, encode: bool = True) -> list:
    returnlist = []

    length = len(request)
    for i in range(length // chunk):
        returnlist.append(request[chunk*i:chunk*(i+1)].encode(packet_encoding) if encode else request[chunk*i:chunk*(i+1)])
    
    if length % chunk:
        returnlist.append((request[(length//chunk)*chunk:].encode(packet_encoding) if encode else request[(length//chunk)*chunk:]).ljust(chunk))

    return returnlist
