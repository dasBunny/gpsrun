import http.server
from http.server import BaseHTTPRequestHandler
import socketserver
import logging
import json
import time
import datetime
import mysql.connector
from db_login import mydb

mycursor = mydb.cursor()
mycursor.execute("SELECT trackname,token FROM devices WHERE id='server' LIMIT 1")
myresult = mycursor.fetchall()
token = myresult[0][1]
trackname=myresult[0][0]
print(token)
print(trackname)