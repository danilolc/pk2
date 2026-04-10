#!/bin/bash

# Pekka Kana 2 by Janne Kivilahti (2003–2007) and the Piste Gamez community.
# https://pistegamez.net/game_pk2.html
#
# This script builds a Debian package (.deb) from the PK2 source tree
# for redistribution and installation using APT.


set -e

ARCH=$(dpkg --print-architecture)
APP=pekka-kana-2
VER=1.5.1
PKG="pkg/${APP}_${VER}_Linux_${ARCH}"

PKGDIR=$PKG

BIN_DIR=$PKGDIR/usr/games
LIB_DIR=$PKGDIR/usr/lib/games/$APP
SHARE_DIR=$PKGDIR/usr/share/games/$APP
DESKTOP_DIR=$PKGDIR/usr/share/applications
ICON_DIR=$PKGDIR/usr/share/icons/hicolor/64x64/apps

echo "== Build =="
make clean
make -j$(nproc)

echo "== Clean =="
rm -rf "$PKGDIR"

echo "== Create dirs =="
mkdir -p "$BIN_DIR"
mkdir -p "$LIB_DIR"
mkdir -p "$SHARE_DIR"
mkdir -p "$DESKTOP_DIR"
mkdir -p "$ICON_DIR"
mkdir -p "$PKGDIR/DEBIAN"

echo "== Launcher =="
install -m 755 misc/linux/dist-launcher.lua \
    "$BIN_DIR/$APP"

echo "== Binary =="
install -m 755 bin/pekka-kana-2 \
    "$LIB_DIR/pk2_greta_${VER}"

echo "== Bundle libs =="

LIBARCH=$(dpkg-architecture -qDEB_HOST_MULTIARCH)

cp -d /usr/lib/$LIBARCH/libzip.so.* "$LIB_DIR/"
cp -d /usr/lib/$LIBARCH/libz.so.* "$LIB_DIR/"

echo "== Assets =="
rsync -a \
  --exclude='data' \
  --chmod=F644,D755 \
  res/ \
  "$SHARE_DIR/"

echo "== Desktop =="
cat > "$DESKTOP_DIR/$APP.desktop" <<EOF
[Desktop Entry]
Type=Application
Version=1.0
Name=Pekka Kana 2
Comment=Platformer in which you play as a rooster
Exec=/usr/games/${APP}
Icon=pekka-kana-2
Terminal=false
Categories=Game;
StartupNotify=true
Keywords=game;platformer;pekka;
EOF

echo "== Icon =="
install -m 644 misc/icon_64x64.png \
    "$ICON_DIR/$APP.png"

echo "== Control =="
cat > "$PKGDIR/DEBIAN/control" <<EOF
Package: pekka-kana-2
Version: ${VER}
Section: games
Priority: optional
Architecture: ${ARCH}
Maintainer: SaturninTheAlien <alienufobomber@gmail.com>
Depends: libstdc++6, libsdl2-2.0-0, libsdl2-image-2.0-0, libsdl2-mixer-2.0-0, liblua5.4-0, lua5.4
Conflicts: pekka-kana-2-data
Replaces: pekka-kana-2-data
Homepage: https://pistegamez.net/game_pk2.html
Description: Pekka Kana 2 is a classic platformer in which you play as a rooster.
EOF

echo "== Build deb =="
dpkg-deb --build --root-owner-group "$PKGDIR"

echo "== Cleaning == "
make clean

echo "== Done =="
