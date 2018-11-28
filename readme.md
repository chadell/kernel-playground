# PLAY WITH IT

## Vagrant

```bash
$ vagrant up
```
and enter into the Ubuntu machine by

```bash
$ vagrant ssh
```

## Compile the module

Use Makefile to compile it:

```bash
$ make
make -C /lib/modules/3.13.0-161-generic/build M=/home/vagrant/github.com/kernel-playground modules
make[1]: Entering directory `/usr/src/linux-headers-3.13.0-161-generic'
  CC [M]  /home/vagrant/github.com/kernel-playground/packet-gossiper.o
/home/vagrant/github.com/kernel-playground/packet-gossiper.c: In function ‘init_module’:
/home/vagrant/github.com/kernel-playground/packet-gossiper.c:47:13: warning: assignment from incompatible pointer type [enabled by default]
   nfho.hook = hook_func;                       //function to call when conditions below met
             ^
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /home/vagrant/github.com/kernel-playground/packet-gossiper.mod.o
  LD [M]  /home/vagrant/github.com/kernel-playground/packet-gossiper.ko
make[1]: Leaving directory `/usr/src/linux-headers-3.13.0-161-generic'
```

Check module related info:

```bash
filename:       /home/vagrant/github.com/kernel-playground/packet-gossiper.ko
description:    A simple Netfilter Kernel Module for fun that register packet info in your logs
author:         Christian Adell <chadell@gmail.com>
license:        GPL
srcversion:     0215A4CDA7CC295461C072D
depends:
vermagic:       3.13.0-161-generic SMP mod_unload modversions retpoline
```

## Install the module in your Kernel

Install the module:

```bash
insmod packet-gossiper.ko
```

and check that it's running:
```bash
 lsmod | grep packet_gossiper
packet_gossiper        12627  0
```

## Check what's going on

Check your log messages:

```bash
$ dmesg
[ 1674.220707] TCP Packet from 10.0.2.2 to port 5632 received
[ 1674.222166] ICMP Packet from 127.0.0.1 received
[ 1674.222176] ICMP Packet from 127.0.0.1 received
[ 1674.222623] TCP Packet from 10.0.2.2 to port 5632 received
[ 1675.222388] ICMP Packet from 127.0.0.1 received
[ 1675.222405] ICMP Packet from 127.0.0.1 received
```

## Uninstall it

```bash
rmmod  packet_gossiper
```

# DOCUMENTATION

https://www.tldp.org/LDP/lkmpg/2.6/html/x121.html
https://www.paulkiddie.com/2009/10/creating-a-simple-hello-world-netfilter-module/
https://www.paulkiddie.com/2009/11/creating-a-netfilter-kernel-module-which-filters-udp-packets/
https://www.netfilter.org/documentation/HOWTO/netfilter-hacking-HOWTO.txt
https://www.cyberciti.biz/tips/compiling-linux-kernel-module.html
http://www.roman10.net/2011/07/31/how-to-write-a-linux-firewall-in-less-than-1000-lines-of-codeput-everything-together/ 
