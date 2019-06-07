#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from socket import *
import threading
import random
import string
import time
import sys
import ssl

PROXY_ADDRESS = ('127.0.0.1', 8080)
PROXY_SSL = False

port_set_event = threading.Event()
test_path = "/what?ever=42"

def client_func():
    client = socket(AF_INET, SOCK_STREAM)
    client.connect(PROXY_ADDRESS)
    if PROXY_SSL:
        context = ssl.create_default_context()
        context.check_hostname = False
        context.verify_mode = ssl.CERT_NONE
        conn = context.wrap_socket(client)
    else:
        conn = client

    dest = "---:%d" % 7000
    print ("Klient połączony z serwerem proxy")
    headers = ""
    for i in range(0, 801):
    	headers += "aaaaa" + ": bbbbbb\r\n"
    conn.sendall("GET http://" + dest + test_path + " HTTP/1.1\r\n" + headers + "\r\n")


    print("Starting to receive data")
    buf = ""
    last_megabyte = 0
    while True:
        tmp = conn.recv(1000)
        if not tmp:
            break
        buf += tmp

    print(buf)
    client.close()

client_func()