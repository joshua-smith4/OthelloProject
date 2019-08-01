import socket
import argparse
import time

parser = argparse.ArgumentParser()
parser.add_argument('--addr', type=str)
parser.add_argument('--bport',type=int)
parser.add_argument('--port', type=int)
parser.add_argument("--msg", type=str)
parser.add_argument('-m', '--machine', type=str, default='machine1')
args = parser.parse_args()

i = 0

with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
    s.bind(('',args.bport))
    while True:
        msg = '{}: {}'.format(args.machine, args.msg)
        bin_data = (msg+' ').encode('utf-8') + i.to_bytes(4, byteorder='little')
        s.sendto(bin_data, (args.addr, args.port))
        data = s.recvfrom(1024)
        print(data)
        i+=1
        time.sleep(0.1)

