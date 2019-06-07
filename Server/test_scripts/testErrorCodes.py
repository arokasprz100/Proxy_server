#!/usr/bin/env python
# -*- encoding: utf-8 -*-

from socket import *
import threading
import random
import time
import sys
import ssl

PROXY_ADDRESS = ('127.0.0.1', 8080)
PROXY_SSL = False

test_path = "/what?ever=42"

def client_func(expectedCode):
	headers = ""
	dest = "127.0.0.1:%d" % 7000
	protocol = "http"
	if expectedCode == 413:
		for i in range(0, 801):
			headers += "aaaaa" + ": bbbbbb\r\n"
	elif expectedCode == 501:
		protocol = "unsupported"
		headers = "Host: " + dest + "\r\nConnection: close\r\nWhatever: 42\r\n"
	elif expectedCode == 502:
		dest = "---:%d" % 7000
		headers = "Host: " + dest + "\r\nConnection: close\r\nWhatever: 42\r\n"
	else:
		return

	client = socket(AF_INET, SOCK_STREAM)
	client.connect(PROXY_ADDRESS)
	if PROXY_SSL:
		context = ssl.create_default_context()
		context.check_hostname = False
		context.verify_mode = ssl.CERT_NONE
		conn = context.wrap_socket(client)
	else:
		conn = client

	print ("Klient połączony z serwerem proxy")
	conn.sendall("GET " + protocol + "://" + dest + test_path + " HTTP/1.1\r\n" + headers + "\r\n")

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


print("413 code test:")
client_func(413)
print("501 code test:")
client_func(501)
print("502 code test:")
client_func(502)