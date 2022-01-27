# RULES :
#   For Sending: EOL (/0)  at end means that there is yet to come more data (>1024)
#                       (use this if cant send data in single stream)
#
#                EOF (/1) at end means that full data is send and start processing it (<=1024)
#                       (use this for single sendto command)

port = 61235
ipv4_grp = '225.0.0.250'
ipv6_grp = 'ff15:7079:7468:6f6e:6465:6d6f:6d63:6173'
ttl = 1  # Increase to reach other networks
listener_buffer = 1024
EOF = b'\1'
EOL = b'\0'
packet_encoding = 'UTF8'  # should be same for all
packet_resend_after = 1  # in seconds
device_to_connect_at_one_time = 20
connection_remove_time = 2  # no of secs after connection should be removed from data if inactive
