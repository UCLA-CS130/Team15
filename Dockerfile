FROM ubuntu:14.04

RUN apt-get update
RUN apt-get install -y libboost-all-dev
RUN apt-get install -y make
RUN apt-get install -y g++

WORKDIR /opt/webserver
COPY . /opt/webserver

# EXPOSE 8080

RUN make clean && make

# CMD ["./webserver","config_file"]

CMD tar -cf - webserver