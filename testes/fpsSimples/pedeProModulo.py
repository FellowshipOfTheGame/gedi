#!/usr/bin/env python

import zmq

def main ():
    ctx = zmq.Context ()
    skt = ctx.socket (zmq.DEALER)
    skt.connect ('tcp://127.0.0.1:5555')

    skt.send_string ('create', zmq.SNDMORE)
    skt.send_string ('ZMQ', zmq.SNDMORE)
    skt.send_string ('800', zmq.SNDMORE)
    skt.send_string ('600')

    while skt.getsockopt (zmq.EVENTS) & zmq.POLLIN == 0:
        skt.send_string ('loop')
    print (skt.recv_string ())
    skt.send_string ('quit')

if __name__ == '__main__':
    main ()
