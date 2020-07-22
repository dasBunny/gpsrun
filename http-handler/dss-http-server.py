import http.server
from http.server import BaseHTTPRequestHandler,HTTPServer
import socketserver
import logging
import json
import time
import datetime
import mysql.connector
from db_login import mydb,PORT

HOST = ''
PORT = 1949

#preparing logging
LOG_FILENAME = 'events.log'
logging.basicConfig(filename=LOG_FILENAME,level=logging.DEBUG)
def logtofile(msg):
	ts = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	logging.info("["+ts+"] "+msg)
	print("["+ts+"] "+msg)

class myHandler(BaseHTTPRequestHandler):

	#When receiving a HEAD request do:
	def do_HEAD(self):
		#Data Handling....
		head_request_ok = True
		#sending http response 200 OK or 400 NOK if request is corrupt
		if head_request_ok:
			print("Response 200")
			self.send_response_only(200, 'OK')
		else:
			print("Response 400")
			self.send_response_only(400, 'NO')
		self.send_header('Content-Length', '1') #Mandatory must be set and not 0 for sim7080 http head response
		self.end_headers()

		#get time when data was received
		time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		print("--- TIME ", ts, " ----------------------")
		print("HEAD request,\nPath:  \t", str(self.path), "\nHeaders:\t", str(self.headers), "\n")
		#Splitting the request to get the different parameters
		args = str(self.path).split("/")
		#logging request
		logtofile("Received HEAD request: " + str(self.path))
		print("Args: ", args)
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
		print("Data: ", data)

		#Matching token against database, if the token does not exist in the database end the handling process
		if "tk" in data:
			print(data["tk"])
			logtofile("Checking token.")
			mycursor.execute("SELECT trackname FROM devices WHERE longToken='{}' LIMIT 1".format((data["tk"])))
			records = mycursor.fetchone()
			print(records)
			if records:
				trackname = records[0]
				logtofile("Matching token, trackname is "+trackname)
			else:
				logtofile("Database: No entry with this token.")
				return
		else:
			logtofile("No token specified, aborting.")
			return
		
		#checking if all required parameters are given, otherwise end processing
		if "lat" in data and "lng" in data:
			logtofile("All required parameters are given")
		else:
			logtofile("Missing parameters, aborting.")
			return

		#check if the data for a timestamp has been send
		if "ts" in data:
			gpsTime = datetime.datetime.utcfromtimestamp(int(data["ts"], 16)/1000).strftime('%Y-%m-%d %H:%M:%S')
			logtofile("Timestamp converted to "+gpsTime)
			logtofile("Received timestamp from device, converting...")
			sql = "INSERT INTO coords (timestamp,lat,lng,name,recording_timestamp) VALUES (%s, %s, %s,%s,%s)"
			val = (time,data["lat"],data["lng"],trackname,gpsTime)                    
			mycursor.execute(sql, val)
			mydb.commit()
		else:
			logtofile("No timestamp received from device, continuing without one.")
			sql = "INSERT INTO coords (timestamp,lat,lng,name) VALUES (%s, %s, %s,%s)"
			val = (time,data["lat"],data["lng"],trackname)                    
			mycursor.execute(sql, val)
			mydb.commit()
		logtofile("Inserting completed.")
		return

	def do_GET(self):
		#get time when data was received
		ts = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
		print("--- TIME ", ts, " ----------------------")
		print("GET request,\nPath:  \t", str(self.path), "\nHeaders:\t", str(self.headers), "\n")
		#Splitting the request to get the different parameters
		args = str(self.path).split("/")
		#logging request
		logtofile("Received HEAD request: " + str(self.path))
		print("Args: ", args)
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
		print("Data: ", data)
		
		#Data Handling....
		head_request_ok = True
		content = json.dumps(data) + '\r\n'
		body = content.encode('UTF-8', 'replace')
		if head_request_ok:
			print("Response 200")
			self.send_response(200, 'OK')
		else:
			print("Response 400")
			self.send_response_only(400, 'NO')
		self.send_header("Content-Type", 'application/json')
		print('Body: ', body)
		print('Content-Length: ' + str(len(body)))
		self.send_header('Content-Length', str(len(body)))
		self.end_headers()
		self.wfile.write(body) #Body must be set after header
		return

	def do_POST(self):
		#POST requests are not handled by this script, responding with 405 Method not allowed
		logtofile("Received POST, not handling.")
		self.send_error(405, 'POST not allowed')
		return

#with socketserver.TCPServer(("", PORT), myHandler) as httpd:
#    #starting HTTP Server
#	ts = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
#	print(ts, " DSS Server Starts - %s:%s" % (HOST, PORT))
#	logtofile("DSS HTTP Server listening at Port  " + str(HOST) + str(PORT))
#	try:
#		httpd.serve_forever()
#	except KeyboardInterrupt:
#		pass
#	finally:
#		httpd.server_close()
		
mycursor = mydb.cursor()

if __name__ == '__main__':
	server_class = HTTPServer
	httpd = server_class((HOST, PORT), myHandler)
	ts = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
	print(ts, " DSS Server Starts - %s:%s" % (HOST, PORT))
	logtofile("DSS HTTP Server listening at Port  " + str(HOST) + str(PORT))
	try:
		httpd.serve_forever()
	except KeyboardInterrupt:
		pass
	finally:
		httpd.server_close()
