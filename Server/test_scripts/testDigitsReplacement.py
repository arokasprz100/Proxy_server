#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from socket import *
import threading
import random
import time
import sys
import hashlib
import ssl

PROXY_ADDRESS = ('127.0.0.1', 8080)
PROXY_SSL = False
PROXY_MEGABYTES = 32

port = None
port_set_event = threading.Event()
test_path = "/what?ever=42"
test_header = "Whatever: 42"

body = "00 0000 0000 0000 0000 0000 00723333"


def server_thread_func():
    global port, port_set_event
    server = socket(AF_INET, SOCK_STREAM)
    server.bind(('127.0.0.1', 0))
    port = server.getsockname()[1]
    port_set_event.set()
    server.listen(1)

    while True:
        client, addr = server.accept()
        print ("Serwer proxy połączony z serwerem docelowym")
        tmp = client.recv(4096)
        print(tmp)
        client.send("HTTP/1.1 200 OK\r\n\r\n")
        client.close()

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
    dest = "127.0.0.1:%d" % port
    print ("Klient połączony z serwerem proxy")
    headers = "Host: " + dest + "\r\nContent-Length: 36\r\n" + test_header + "\r\n"

    conn.sendall("POST http://" + dest + test_path + " HTTP/1.1\r\n" + headers + "\r\n" + body)

    print("Starting to receive data")
    buf = ""
    while True:
        tmp = conn.recv(1048576)
        if not tmp:
            break
        buf += tmp

    print(buf)
    client.close()

server_thread = threading.Thread(target=server_thread_func)
server_thread.daemon = True
server_thread.start()

port_set_event.wait()

client_func()

