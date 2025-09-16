#!/bin/bash
set -e

# Configuration
TMP_DIR="/tmp/build-tmp"
PROJECT_DIR="/media/ssd/Project/open_source/gnome-monitor"
BUILD_JSON="${PROJECT_DIR}/org.ji.test.json"
BUILD_DIR="${PROJECT_DIR}/build-dir"
SRC_DIR="${PROJECT_DIR}/gnome-system-monitor"
MESON_BUILD_DIR="${TMP_DIR}/build"
INSTALL_PREFIX="${TMP_DIR}/install"
SCHEMA_DIR="$INSTALL_PREFIX/share/glib-2.0/schemas"
APP_BINARY="$INSTALL_PREFIX/bin/gnome-system-monitor"

function setup_tmpfs() {
    mkdir -p "$TMP_DIR"
    if ! mountpoint -q "$TMP_DIR"; then
        echo "Mounting tmpfs at $TMP_DIR..."
        sudo mount -t tmpfs -o size=4G tmpfs "$TMP_DIR"
    else
        echo "tmpfs already mounted at $TMP_DIR"
    fi
    sudo chown "$USER":"$USER" "$TMP_DIR"
}

function run_build() {
    flatpak-builder --run \
        --filesystem="$TMP_DIR" \
        --filesystem=host \
        --share=network \
        "$BUILD_DIR" "$BUILD_JSON" bash -c "
            set -e
            echo 'Running Meson + Ninja build inside Flatpak sandbox...'

            cd \"$SRC_DIR\"

            export PKG_CONFIG_PATH=\"/app/lib64/pkgconfig:/app/lib/pkgconfig:/usr/lib64/pkgconfig:/usr/lib/pkgconfig\"

            meson setup \"$MESON_BUILD_DIR\" --buildtype debug -Ddevelopment=true --prefix=\"$INSTALL_PREFIX\"

            ninja -C \"$MESON_BUILD_DIR\"
            ninja -C \"$MESON_BUILD_DIR\" install

            export GSETTINGS_SCHEMA_DIR=$INSTALL_PREFIX/share/glib-2.0/schemas

            echo 'Build complete.'
            bash
        "
}

function run_app() {
    export GSETTINGS_SCHEMA_DIR="$SCHEMA_DIR"

    if [[ ! -x "$APP_BINARY" ]]; then
        echo "Error: App binary not found: $APP_BINARY"
        echo "Did you run ./run.sh or ./run.sh -build first?"
        exit 1
    fi

    echo "Running application with GSETTINGS_SCHEMA_DIR=$GSETTINGS_SCHEMA_DIR"
    "$APP_BINARY"
}

# Main logic
case "$1" in
    -build)
        run_build
        ;;
    -run)
        run_app
        ;;
    *)
        setup_tmpfs
        run_build
        run_app
        ;;
esac

# ./run.sh         # full build setup
# ./run.sh -build  # only build inside flatpak
# ./run.sh -run    # run the built binary with correct schema path