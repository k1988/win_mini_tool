#!/usr/bin/env python
"""
calc uuid by player's nickname
the algorithm is copy from minecraft server 
"""

import sys,hashlib,binascii;

def mc_uuid_gen(nick_name):
    md5 = hashlib.md5("OfflinePlayer:" + nick_name)

    #example: 08D699BB6400555E581B678C9441FA75
    result = bytearray(md5.digest())
    result[6] = result[6] & 0x0f
    result[6] = result[6] | 0x30
    result[8] = result[8] & 0x3f
    result[8] = result[8] | 0x80
    s = binascii.hexlify(result)
    # example: 08d699bb-6400-355e-981b-678c9441fa75
    print("%s-%s-%s-%s-%s" % (s[0:8], s[8:12], s[12:16], s[16:20], s[20:32]))


if __name__ == '__main__':
    if (len(sys.argv) == 1):
        print("No player's nickname specified!")
        sys.exit(64) # EX_USAGE
    nick_name = sys.argv[1]
    mc_uuid_gen(nick_name)