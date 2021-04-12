#!/usr/bin/env python
"""
Created by: Chris Sikes
Date: 6/28/2019
Version: 1.0

Python HTTP POST Server
Listens on a port of your choosing for an encrypted file.
The file is unencrypted, then unzipped, and saved in the current directory.


Usage::
    ./dummy-web-server.py [<port>]

Send a POST request::
    curl -d "foo=bar&bin=baz" http://localhost

"""
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import SocketServer
import os
from arc4 import ARC4


class S(BaseHTTPRequestHandler):
    def _set_headers(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        self._set_headers()
        self.wfile.write("<html><body><h1>hi!</h1></body></html>")

    def do_HEAD(self):
        self._set_headers()
    
    def do_POST(self):
	global i
	arc4 = ARC4('test')
	content_len = int(self.headers.getheader('content-length', 0))
    	post_data = self.rfile.read(content_len) # <--- Gets the data itself
	post_data2 = '\n'.join(post_data.split('\n')[4:-1])
	post_data3 = arc4.decrypt(post_data2)
	f = open("PostTest/File%s" % i, "a")
	f.write(post_data3)
	f.close()
	i = i + 1
   	self._set_headers()
        
def run(server_class=HTTPServer, handler_class=S, port=80):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print 'Starting httpd...'
    httpd.serve_forever()

i = 0


if __name__ == "__main__":
    from sys import argv
    
    if len(argv) == 2:
        run(port=int(argv[1]))
    else:
        run()

