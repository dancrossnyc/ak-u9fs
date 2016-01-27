# u9fs for Akaros

This is a version of `u9fs` from Plan 9 that has been ported to
Akaros.  This can be cross-compiled from Linux and run on Akaros
to export filesystems _from_ Akaros _to_ remote systems via the
9P protocol.

To build, simply run 'make'. The makefile assumes that the Akaros
cross-compiler is your $PATH.

E.g.,

```
linux% cd /where/ever/ak-u9fs
linux% make
linux% cp u9fs $AKROOT/kern/kfs/bin
linux% cd $AKROOT
linux% make
```

This produces an Akaros kernel image with a copy of `u9fs` in
the KFS.

Typically `u9fs` is started by `listen1` on Akaros; the latter
handles listening for incoming network connections etc.

E.g.,

Boot Akaros on a machine, then:

```
akaros% ash /ifconfig
akaros% mkdir /tmp
akaros% listen1 'tcp!*!2223' /bin/u9fs -a none -n -u mvshaney &
```

One can then import the filesystem exported from the Akaros
machine elsewhere. For example, on Plan 9:

```
term% srv -nq tcp!akaroshost!2223 akaros /n/akaros
```

Or on Linux, using 9pfuse from plan9port:

```
linux% mkdir -p /n/akaros 2>/dev/null
linux% 9pfuse 'tcp!akaroshost!2223' /n/akaros
```
