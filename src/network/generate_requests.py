import json
import typing

from .constants import packet_encoding


def File_transfer(filename: str = 'alora-share.txt', data_type: typing.Union[bytes, 'ping'] = 'ping'):
    headers = {
        "event-type": "data-transfer",
        "data-type": data_type,
        "metadata": {
            "filename": filename,
        }
    }
    return json.dumps(headers).encode(packet_encoding)