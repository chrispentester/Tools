#!/usr/bin/env python
"""
Created by: Chris Sikes
Date: 12/23/2020
Version: 1.0

Python HTTP POST Server
Listens on a port for your chosen file
The file is sent and saved with the same name in the PostTest directory.


Usage::
    ./posthandle.py [<port>]

Send a POST request::
    curl -X POST -F 'data=@/root/Desktop/testfile' http://127.0.0.1:8080

"""
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
import SocketServer
import os


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
	content_len = int(self.headers.getheader('content-length', 0))
    	post_data = self.rfile.read(content_len) # <--- Gets the data itself
	filename = '\n'.join(post_data.split('\n')[1:2])
	filename2 = filename.split('"')[3]
	print filename2
	print '----'+'\n'
	post_data2 = '\n'.join(post_data.split('\n')[4:-2])
	f = open("PostTest/" + filename2, "a")
	print post_data2
	f.write(post_data2)
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

