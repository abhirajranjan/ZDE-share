# RULES :
#    EOF (/0):
#       at end means that full data is send and start processing it (<=1024)
#       (use this for single sendto command)

PORT = 61235
IPV4_GRP = '225.0.0.250'
IPV6_GRP = 'ff15:7079:7468:6f6e:6465:6d6f:6d63:6173'
TTL = 1  # Increase to reach other networks
LISTENER_BUFFER = 1024
PACKET_ENCODING = 'UTF8'  # should be same for all
EOF_STRING = '\0'
EOF = bytes(EOF_STRING, PACKET_ENCODING)
PACKET_RESEND_AFTER = 1  # in seconds
DEVICE_TO_CONNECT_AT_ONE_TIME = 20
CONNECTION_REMOVE_TIME = 2  # no of secs after connection should be removed from data if inactive
