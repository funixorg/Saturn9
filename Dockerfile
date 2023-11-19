FROM ubuntu:latest

# Install essential packages
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    nasm \
    qemu \
    xorriso \
    python3 

WORKDIR /usr/src/saturn9

COPY . /usr/src/saturn9

RUN cd saturn9/limine; make install

CMD make build
