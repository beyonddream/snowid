FROM ubuntu as build-base
RUN apt-get update && apt-get install build-essential -y

WORKDIR /build
COPY . .

# compile the binaries
RUN make

CMD ["/build/snowid"]