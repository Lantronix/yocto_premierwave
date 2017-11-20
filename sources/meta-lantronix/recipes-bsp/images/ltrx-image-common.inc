SUMMARY = "A console-only image for the Lantronix pw2050 machine"

IMAGE_FEATURES += "splash"

LICENSE = "MIT"

inherit image

IMAGE_LINGUAS = ""

# main packages include binaries and other files for running
# -dev packages include .so and .h files
# -staticdev packages include .a and .h files

IMAGE_INSTALL += "avahi-daemon"
IMAGE_INSTALL += "bash"
IMAGE_INSTALL += "bridge-utils"
IMAGE_INSTALL += "busybox"
IMAGE_INSTALL += "bzip2"
IMAGE_INSTALL += "cjson-dev"
IMAGE_INSTALL += "dbus"
IMAGE_INSTALL += "expat"
IMAGE_INSTALL += "ezxml-staticdev"
IMAGE_INSTALL += "fcgi"
IMAGE_INSTALL += "glib-2.0"
IMAGE_INSTALL += "gmp"
IMAGE_INSTALL += "gtest"
IMAGE_INSTALL += "i2c-tools"
IMAGE_INSTALL += "iproute2"
IMAGE_INSTALL += "iputils"
IMAGE_INSTALL += "kernel-modules"
IMAGE_INSTALL += "libbsd-mini-dev"
IMAGE_INSTALL += "libdaemon"
IMAGE_INSTALL += "libffi"
IMAGE_INSTALL += "libjson"
IMAGE_INSTALL += "libpcap"
IMAGE_INSTALL += "libpcre"
IMAGE_INSTALL += "libusb1"
IMAGE_INSTALL += "libwebsockets"
IMAGE_INSTALL += "mii-diag"
IMAGE_INSTALL += "mrouted"
IMAGE_INSTALL += "ncurses"
IMAGE_INSTALL += "ndisc6"
IMAGE_INSTALL += "openssl"
IMAGE_INSTALL += "openswan"
IMAGE_INSTALL += "paho-mqtt.embedded-c"
IMAGE_INSTALL += "python"
IMAGE_INSTALL += "python-pyserial"
IMAGE_INSTALL += "python-pyusb"
IMAGE_INSTALL += "python-pyzmq"
IMAGE_INSTALL += "python-setuptools"
IMAGE_INSTALL += "readline"
IMAGE_INSTALL += "rng-tools"
IMAGE_INSTALL += "spawn-fcgi"
IMAGE_INSTALL += "tar"
IMAGE_INSTALL += "tcpdump"
IMAGE_INSTALL += "tzdata"
IMAGE_INSTALL += "u-boot-fw-utils"
IMAGE_INSTALL += "u-boot-mkimage"
IMAGE_INSTALL += "usbutils"
IMAGE_INSTALL += "util-linux"
IMAGE_INSTALL += "wide-dhcpv6"
IMAGE_INSTALL += "zlib"

apply_overlay() {
    cp -rf ${THISDIR}/files/overlay/* ${IMAGE_ROOTFS}/
    cp -rf ${THISDIR}/files/${MACHINE}-overlay/* ${IMAGE_ROOTFS}/

    # un-expire account passwords
    sed -i 's/:0:0:99999/:10933:0:99999/' ${IMAGE_ROOTFS}/etc/shadow

    # override to use tmpfs instead of RAM disk
    install -D -m 0755 ${THISDIR}/files/ltrx_remount_etc ${IMAGE_ROOTFS}/sbin/ltrx_remount_etc

# add yocto tmpfs mounts to Lantronix fstab
    cat >> ${IMAGE_ROOTFS}/etc/fstab << EOF
tmpfs /run tmpfs mode=0755,nodev,nosuid,strictatime 0 0
tmpfs /var/volatile tmpfs defaults 0 0
EOF

# create dirs in tmpfs mounts
    cat >> ${IMAGE_ROOTFS}/etc/inittab << EOF

# Yocto compatibility fixes
null::sysinit:/bin/mkdir -p /var/volatile/log
null::sysinit:/bin/mkdir -p /var/volatile/tmp
null::sysinit:/bin/mkdir -p /run/lock
null::sysinit:/usr/bin/update-alternatives
null::sysinit:/bin/bash -c 'for script in /etc/rcS.d/*; do \$script start; done'
null::sysinit:/bin/bash -c 'for script in /etc/rc5.d/*; do \$script start; done'

EOF

    if [ ! -e ${IMAGE_ROOTFS}/http/config/en_US ]; then
        cd ${IMAGE_ROOTFS}/http/config
        ln -s . en_US
    fi

    if [ -e ${IMAGE_ROOTFS}/${sbindir}/i2cset -a ! -e ${IMAGE_ROOTFS}/usr/bin/i2cset ]; then
        ln -s ${sbindir}/i2cset ${IMAGE_ROOTFS}/usr/bin/i2cset
    fi

    if [ -e ${IMAGE_ROOTFS}/${base_sbindir}/fw_printenv -a ! -e ${IMAGE_ROOTFS}/usr/sbin/fw_printenv ]; then
        ln -s ${base_sbindir}/fw_printenv ${IMAGE_ROOTFS}/usr/sbin/fw_printenv
    fi

    if [ -e ${IMAGE_ROOTFS}/${base_sbindir}/fw_setenv -a ! -e ${IMAGE_ROOTFS}/usr/sbin/fw_setenv ]; then
        ln -s ${base_sbindir}/fw_setenv ${IMAGE_ROOTFS}/usr/sbin/fw_setenv
    fi

    if [ -e ${IMAGE_ROOTFS}/${base_sbindir}/mke2fs -a ! -e ${IMAGE_ROOTFS}/usr/sbin/mke2fs ]; then
        ln -s ${base_sbindir}/mke2fs ${IMAGE_ROOTFS}/usr/sbin/mke2fs
    fi

    if [ -e ${IMAGE_ROOTFS}/${bindir}/dnsmasq -a ! -e ${IMAGE_ROOTFS}/usr/sbin/dnsmasq ]; then
        ln -s ${bindir}/dnsmasq ${IMAGE_ROOTFS}/usr/sbin/dnsmasq
    fi

    if [ -e ${IMAGE_ROOTFS}/${bindir}/dhcp6c -a ! -e ${IMAGE_ROOTFS}/bin/dhcp6c ]; then
        ln -s ${bindir}/dhcp6c ${IMAGE_ROOTFS}/bin/dhcp6c
    fi

    chmod 4755 ${IMAGE_ROOTFS}/bin/ltrx_cli
    chmod 4755 ${IMAGE_ROOTFS}/usr/sbin/wpa_cli
    chmod 4755 ${IMAGE_ROOTFS}/bin/ser_con
    chmod 4755 ${IMAGE_ROOTFS}/usr/bin/traceroute.traceroute
    chmod 4755 ${IMAGE_ROOTFS}/usr/sbin/tcpdump
    chmod 4755 ${IMAGE_ROOTFS}/bin/ping.iputils
    chmod 4755 ${IMAGE_ROOTFS}/bin/ping6.iputils
    chmod 4755 ${IMAGE_ROOTFS}/sbin/ebtables
    chmod 4755 ${IMAGE_ROOTFS}/usr/sbin/iptables
    rm -rf ${IMAGE_ROOTFS}/lib/libBrokenLocale*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libBrokenLocale.so
    rm -rf ${IMAGE_ROOTFS}/lib/libanl*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libanl.so
    rm -rf ${IMAGE_ROOTFS}/lib/libcidn*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libcidn*
    rm -rf ${IMAGE_ROOTFS}/lib/libfdisk*
    rm -rf ${IMAGE_ROOTFS}/usr/sbin/sfdisk
    rm -rf ${IMAGE_ROOTFS}/sbin/fdisk
    rm -rf ${IMAGE_ROOTFS}/lib/libnss_db*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libnss_db*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libnss_hesiod*
    rm -rf ${IMAGE_ROOTFS}/lib/libnss_hesiod*
    rm -rf ${IMAGE_ROOTFS}/lib/libnss_mdns*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libnss_nis*
    rm -rf ${IMAGE_ROOTFS}/lib/libnss_nis*
    rm -rf ${IMAGE_ROOTFS}/lib/libsmartcols*
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/setuptools-2.1.2-py2.7.egg
    rm -rf ${IMAGE_ROOTFS}/sbin/badblocks
    rm -rf ${IMAGE_ROOTFS}/sbin/blockdev*
    rm -rf ${IMAGE_ROOTFS}/sbin/blkid
    rm -rf ${IMAGE_ROOTFS}/sbin/debugfs
    rm -rf ${IMAGE_ROOTFS}/sbin/fdisk.util-linux
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/encodings/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/logging/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/json/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/json/tests/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/sqlite3/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/xml/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/xml/sax/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/xml/etree/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/xml/dom/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/bin/uboot-mkimage
    rm -rf ${IMAGE_ROOTFS}/usr/bin/col
    rm -rf ${IMAGE_ROOTFS}/usr/bin/colcrt
    rm -rf ${IMAGE_ROOTFS}/usr/bin/colrm
    rm -rf ${IMAGE_ROOTFS}/usr/bin/column
    rm -rf ${IMAGE_ROOTFS}/usr/bin/cgi-fcgi
    rm -rf ${IMAGE_ROOTFS}/usr/bin/lslogins
    rm -rf ${IMAGE_ROOTFS}/usr/bin/nettle-*
    rm -rf ${IMAGE_ROOTFS}/usr/bin/nsenter
    rm -rf ${IMAGE_ROOTFS}/usr/bin/taskset
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/email/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/email/test
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/email/mime/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/serial/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/serial/tools/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/serial/urlhandler/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/usb/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/usb/backend/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/cffi_core/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/core/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/devices/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/eventloop/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/eventloop/minitornado/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/green/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/green/eventloop/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/log/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/ssh/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/sugar/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/tests/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/zmq/utils/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/ctypes/test
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/ctypes/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/ctypes/macholib/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/distutils/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/distutils/tests
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/distutils/command/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/setuptools/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/setuptools/command/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/site-packages/setuptools/tests/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/lib/python2.7/wsgiref/*.py
    rm -rf ${IMAGE_ROOTFS}/usr/bin/write
    rm -rf ${IMAGE_ROOTFS}/usr/bin/whereis
    rm -rf ${IMAGE_ROOTFS}/usr/bin/wdctl
    rm -rf ${IMAGE_ROOTFS}/usr/bin/wall
    rm -rf ${IMAGE_ROOTFS}/usr/bin/wall.util-linux
    rm -rf ${IMAGE_ROOTFS}/wall.util-linux
    rm -rf ${IMAGE_ROOTFS}/usr/bin/utmpdump
    rm -rf ${IMAGE_ROOTFS}/usr/bin/utmpdump.util-linux
    rm -rf ${IMAGE_ROOTFS}/usr/bin/ulockmgr_server
    rm -rf ${IMAGE_ROOTFS}/usr/bin/ul
    rm -rf ${IMAGE_ROOTFS}/usr/bin/setarch
    rm -rf ${IMAGE_ROOTFS}/usr/bin/sexp-conv
    rm -rf ${IMAGE_ROOTFS}/usr/bin/setterm
    rm -rf ${IMAGE_ROOTFS}/usr/bin/script
    rm -rf ${IMAGE_ROOTFS}/usr/bin/scriptreplay
    rm -rf ${IMAGE_ROOTFS}/usr/bin/newgrp
    rm -rf ${IMAGE_ROOTFS}/usr/bin/newgrp.shadow
    rm -rf ${IMAGE_ROOTFS}/usr/bin/m4
    rm -rf ${IMAGE_ROOTFS}/usr/bin/fallocate
    rm -rf ${IMAGE_ROOTFS}/usr/bin/fixproc
    rm -rf ${IMAGE_ROOTFS}/usr/bin/cal
    rm -rf ${IMAGE_ROOTFS}/usr/bin/namei
    rm -rf ${IMAGE_ROOTFS}/usr/bin/psplash
    rm -rf ${IMAGE_ROOTFS}/usr/bin/psplash-default
    rm -rf ${IMAGE_ROOTFS}/usr/bin/psplash-write
    rm -rf ${IMAGE_ROOTFS}/usr/bin/pkcs1-conv
    rm -rf ${IMAGE_ROOTFS}/usr/bin/prlimit
    rm -rf ${IMAGE_ROOTFS}/usr/bin/spawn-fcgi
    rm -rf ${IMAGE_ROOTFS}/usr/bin/tkmib
    rm -rf ${IMAGE_ROOTFS}/usr/bin/tset
    rm -rf ${IMAGE_ROOTFS}/usr/bin/unshare
    rm -rf ${IMAGE_ROOTFS}/sbin/cfdisk
    rm -rf ${IMAGE_ROOTFS}/usr/bin/ionice
    rm -rf ${IMAGE_ROOTFS}/usr/bin/ipcmk
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libX11.so.6.3.0
    rm -rf ${IMAGE_ROOTFS}/usr/lib/libperl.so.5.22.0
    rm -rf ${IMAGE_ROOTFS}/usr/include/*

    mkdir -p ${IMAGE_ROOTFS}/root

    chown --recursive --no-dereference root:root ${IMAGE_ROOTFS}

    find ${IMAGE_ROOTFS} \( -name ".gitkeep" -o -name ".gitignore" \) -exec rm {} \;
}
ROOTFS_POSTPROCESS_COMMAND += "apply_overlay; "