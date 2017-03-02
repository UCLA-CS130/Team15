#!/bin/bash

# Build the binary
make clean
make

# Start the server
echo "
# This is a comment.

port 2020;  # This is also a comment.
server_name 127.0.0.1;

path / StaticHandler {
  root /foo/bar;
}

path /echo EchoHandler{}

# Default response handler if no handlers match.
default NotFoundHandler{}" > test_config

echo "
# This is a comment.
port 8080;  # This is also a comment.
server_name 127.0.0.1;
path / StaticHandler {
  root /foo/bar;
}
path /echo EchoHandler{}
path /80 ProxyHandler {
  server_host ucla.edu;
  server_port 80;
}
path /2020 ProxyHandler {
  server_host localhost;
  server_port 2020;
}
# Default response handler if no handlers match.
default NotFoundHandler{}" > test_proxy_config


./webserver test_config &>/dev/null &
#./webserver test_config &
./webserver test_proxy_config &>/dev/null &

sleep 5
# Send request to server
curl -i -s localhost:2020 > test_response

# Verify the response from the server works as expected
DIFF=$(diff expected_response test_response)
EXIT_STATUS=$?

#Do the proxy test
curl -i -s localhost:8080/2020/echo > test_proxy_response
DIFF_PROXY=$(diff expected_proxy_response test_proxy_response)
EXIT_STATUS_PROXY=$?

curl -i -s localhost:8080/80/ > test_proxy_redirect_response
head -n 1 test_proxy_redirect_response > test_proxy_redirect_response1
DIFF_REDIRECT=$(diff test_proxy_redirect_response1 expected_proxy_redirect_response)
EXIT_STATUS_PROXY_REDIRECT=$?

# Error handling
if [ "$EXIT_STATUS" -eq 0 ]
then
    echo "SUCCESS: Integration test passed"
else
    echo "FAILED: Integration test failed"
    echo "diff:"
    echo $DIFF
fi

#Proxy error handling
if [ "$EXIT_STATUS_PROXY" -eq 0 ]
then
    echo "SUCCESS: Proxy test passed"
else
    echo "FAILED: Proxy test failed"
    echo "diff: "
    echo $DIFF_PROXY
fi

#Proxy Redirected Handling
if [ "$EXIT_STATUS_PROXY_REDIRECT" -eq 0 ]
then
    echo "SUCCESS: Proxy 302 test passed"
else
    echo "FAILED: Proxy 302 test failed"
    echo "diff: "
    echo $DIFF_REDIRECT
fi


# Shutdown the webserver and cleanup
echo "Cleaning up and shutting down"
pkill webserver
make clean
rm -f test_response
rm -f test_proxy_response
rm -f test_proxy_redirect_response
rm -f test_proxy_redirect_response1

exit "$EXIT_STATUS"
