import http.server
from http.server import BaseHTTPRequestHandler,HTTPServer
import socketserver
import logging
import json
import time
import mysql.connector
import datetime
from db_login import mydb,PORT

#preparing logging
LOG_FILENAME = 'events.log'
logging.basicConfig(filename=LOG_FILENAME,level=logging.DEBUG)
def logtofile(msg):
    dt = datetime.datetime.now()
    t=dt.strftime('%Y-%m-%d %H:%M:%S')
    logging.info("["+t+"] "+msg)

class myHandler(BaseHTTPRequestHandler):
    #When receiving a GET request do:
    def do_GET(self):
        #get time when data was received
        ts = datetime.datetime.now()
        time=ts.strftime('%Y-%m-%d %H:%M:%S')
        #print("GET request,\nPath: %s\nHeaders:\n%s\n", str(self.path), str(self.headers))
        #Splitting the request to get the different parameters
        args = str(self.path).split("/")
        #logging request
        logtofile("Received GET request: " + str(self.path))
        #print(args)      
        #make sure the request has parameters
        if len(args)==0:
            print("Error")
            logtofile("Error: No parameters")

        #storing all the parameters in a dict
        data = dict()
        for i in args:
            if '=' in i:
                val = i.split("=")
                data[val[0]]=val[1]
        #print(data)

        #sending http response with some debugging information
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

        #check if the data for a timestamp has been send
        if "hour" in data and "min" in data and "sec" in data and "day" in data and "month" in data and "year" in data:
            #transforming date into a string that we can insert into the database
            datestring = "%s-%s-%s %s:%s:%s" % (data["year"],data["month"],data["day"],data["hour"],data["min"],data["sec"])
            logtofile("Date was send: "+datestring)
            #The flag indicates that we want to use an SQL insert command that includes the timestamp
            recording_timestamp_flag = True
        else:
            #if no timestamp is given we want to use a different insert command
            recording_timestamp_flag = False

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
                if recording_timestamp_flag:
                    sql = "INSERT INTO coords (timestamp,lat,lng,name,recording_timestamp) VALUES (%s, %s, %s,%s,%s)"
                    val = (time,data["lat"],data["lng"],trackname,datestring)
                else:
                    sql = "INSERT INTO coords (timestamp,lat,lng,name) VALUES (%s, %s, %s,%s)"
                    val = (time,data["lat"],data["lng"],trackname)                    
                mycursor.execute(sql, val)
                mydb.commit()
            else:
                logtofile("Bad Token")
        else:
            logtofile("Missing parameters")
    def do_POST(self):
        #POST requests are not handled by this script, responding with 405 Method not allowed
        logtofile("Received POST, not handling.")
        self.send_response(405)

mycursor = mydb.cursor()

with socketserver.TCPServer(("", PORT), myHandler) as httpd:
    #starting HTTP Server
    #print("serving at port", PORT)
    logtofile("Started on port " + str(PORT))
    httpd.serve_forever()