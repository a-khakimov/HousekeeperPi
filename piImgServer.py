#!/usr/bin/env python

from picamera import PiCamera
from http.server import BaseHTTPRequestHandler, HTTPServer

camera = PiCamera()
camera.resolution = (400, 400)

# HTTPRequestHandler class
class testHTTPServer_RequestHandler(BaseHTTPRequestHandler):

    def make_image():
        camera.capture('/tmp/img.png')

    def do_GET(self):
        if self.path=="/image":
            print("Get image request")
            make_image()
            f = open("/tmp/img.png", 'rb')
            self.send_response(200)
            self.send_header('Content-type', 'image/png')
            self.end_headers()
            self.wfile.write(f.read())
            f.close()
            return

def run():
    server_address = ('127.0.0.1', 2081)
    httpd = HTTPServer(server_address, testHTTPServer_RequestHandler)
    print('Start serve...')
    httpd.serve_forever()

run()
