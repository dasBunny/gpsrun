import http.server
from http.server import BaseHTTPRequestHandler,HTTPServer
import socketserver
import logging
import json
import time
import mysql.connector
import datetime
from db_login import mydb,PORT

LOG_FILENAME = 'events.log'
logging.basicConfig(filename=LOG_FILENAME,level=logging.DEBUG)


def logtofile(msg):
    dt = datetime.datetime.now()
    t=dt.strftime('%Y-%m-%d %H:%M:%S')
    logging.info("["+t+"] "+msg)

class myHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        ts = datetime.datetime.now()
        time=ts.strftime('%Y-%m-%d %H:%M:%S')
        print("GET request,\nPath: %s\nHeaders:\n%s\n", str(self.path), str(self.headers))
        args = str(self.path).split("/")
        logtofile("Received GET request: " + str(self.path))
        print(args)
        if len(args)==0:
            print("Fehler")
        data = dict()
        for i in args:
            if '=' in i:
                val = i.split("=")
                data[val[0]]=val[1]
        print(data)
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()
        output = "<html><head><title>Sucess</title></head>"
        self.wfile.write(output.encode(encoding='utf_8'))
        output = "<body><p>Server reached.</p>"
        self.wfile.write(output.encode(encoding='utf_8'))
        output = "<p>You accessed path: %s</p>" % self.path
        self.wfile.write(output.encode(encoding='utf_8'))
        output = "</body></html>"
        self.wfile.write(output.encode(encoding='utf_8'))
        if "lat" in data and "lng" in data and "token" in data and "id" in data:
            logtofile("Matching parameters, checking token.")
            sql="""SELECT id,token,trackname from devices where id = %s"""
            mycursor.execute(sql,(data["id"],))
            records = mycursor.fetchone()
            token = records[1]
            trackname = records[2]
            print(token)
            if token==data["token"]:
                logtofile("Token OK - Inserting data with trackname {}".format(trackname))
                sql = "INSERT INTO coords (timestamp,lat,lng,name) VALUES (%s, %s, %s,%s)"
                val = (time,data["lat"],data["lng"],trackname)
                mycursor.execute(sql, val)
                mydb.commit()
            else:
                logtofile("Bad Token")
        else:
            logtofile("Missing parameters")
    def do_POST(self):
        logtofile("Received POST, not handling.")
        self.send_response(405)

mycursor = mydb.cursor()

with socketserver.TCPServer(("", PORT), myHandler) as httpd:
    print("serving at port", PORT)
    logtofile("Started on port " + str(PORT))
    httpd.serve_forever()