# Pekka Kana 2 by Janne Kivilahti (2003–2007) and the Piste Gamez Community.
# https://pistegamez.net/game_pk2.html
#
# This image is used to build the Debian package.
# Usage:
#   docker build -t pk2-builder .
#   docker run --rm -it -v $(pwd):/pk2 pk2-builder /pk2/build_deb.sh

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    git \
    make \
    g++ \
    libsdl2-dev \
    libsdl2-image-dev \
    libsdl2-mixer-dev \
    libzip-dev \
    lua5.4 \
    liblua5.4-dev \
    dpkg-dev \
    rsync \
    patchelf

RUN git config --global --add safe.directory /pk2

WORKDIR /pk2