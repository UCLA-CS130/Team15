# Help from : https://pymotw.com/2/threading/, https://www.tutorialspoint.com/python/python_networking.htm, https://github.com/UCLA-CS130/AAAAA/blob/master/test/integration_multithread_test.py
# taken from Team spaceteam

from os import system
from threading import Thread
import socket
import sys
import subprocess
import time

PORT = 2020
BUFFER_LEN = 1024
HOLD_REQUEST = "GET /hold HTTP/1.1"
STATIC_REQUEST = "GET / HTTP/1.1\r\n\r\n"
EXPECTED_STATIC_RESPONSE = "HTTP/1.0 200 OK\r\nContent-Length: 5\r\nContent-Type: text/html\r\n\r\ntest\n"

# Sets up socket for connection
def setup_socket():
	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.connect(('localhost', PORT))
	return sock	

def receive(sock):
	data = sock.recv(BUFFER_LEN)
	sock.close()
	return data

def hold_connect():
	sock = setup_socket()
	sock.send(HOLD_REQUEST)
	sock.send("\r\n\r\n")
	receive(sock)
	return

def connect(request, expected_response, error_code):
	sock = setup_socket()
	sock.send(request)
	response = receive(sock)

	if (response != expected_response):
		print "Error: ", repr(response)
		error_code = 1
		return
	print "Connection Successful!"
	error_code = 0

def n_connections_test(threads):
	print "Starting hold request thread"

	thread_list = list()

	for x in threads:
		hold = Thread(target = hold_connect, args=())
		hold.start()
		thread_list.append(hold)
		time.sleep(3)


	print "Starting static request thread"
	static_error = 0
	static = Thread(target = connect, args=(STATIC_REQUEST, EXPECTED_STATIC_RESPONSE, static_error))
	static.start()
	static.join()

	# Kill the webserver process since the /hold connection is an infinite loop
	print "Closing webserver connection."
	system("pkill webserver")
	return static_error

def start_server():
	print "Starting server..."
	p = subprocess.Popen("./webserver config_file", shell=True)

if __name__ == "__main__":
	# Give time for the server to start
	start_server()
	time.sleep(2)

	n_connections_results = n_connections_test(sys.argv[1])
	time.sleep(2)

	if (n_connections_results == 0):
		print "THREAD SUCCESS"
		exit(0)
	else:
		print "THREAD FAILURE"
		exit(1)