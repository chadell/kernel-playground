#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/inet.h>
#include <linux/ip.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#define DRIVER_AUTHOR "Christian Adell <chadell@gmail.com>"
#define DRIVER_DESC   "A simple Netfilter Kernel Module for fun that register packet info in your logs"

static struct nf_hook_ops nfho;         //struct holding set of hook function options
struct sk_buff *sock_buff;
struct iphdr *ip_header;
struct tcphdr *tcp_header;  
struct udphdr *udp_header;  

//function to be called by hook
unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn)(struct sk_buff *))
{
  char src_addr[INET_ADDRSTRLEN];

  sock_buff = skb;
  ip_header = (struct iphdr *)skb_network_header(sock_buff);
  snprintf(src_addr, INET_ADDRSTRLEN, "%pI4", &ip_header->saddr);

  if ( ip_header->protocol==6 ) {
    tcp_header = (struct tcphdr *)skb_transport_header(sock_buff);
    printk(KERN_DEBUG "TCP Packet from %s to port %u received\n", src_addr, tcp_header->dest);
  } else if ( ip_header->protocol==17 ) {
    udp_header = (struct udphdr *)skb_transport_header(sock_buff);
    printk(KERN_DEBUG "UDP Packet from %s to port %u received\n", src_addr, udp_header->dest);
  } else if ( ip_header->protocol==1 ) {
    printk(KERN_DEBUG "ICMP Packet from %s received\n", src_addr);
  } else {
    printk(KERN_DEBUG "Non ICMP/TCP/UDP Packet from %s received\n", src_addr);
  }

  return NF_ACCEPT;                            //accepts the packet
}

//Called when module loaded using 'insmod'
int init_module()
{
  nfho.hook = hook_func;                       //function to call when conditions below met
  nfho.hooknum = NF_INET_PRE_ROUTING;          //called right after packet recieved, first hook in Netfilter
  // nfho.hooknum = NF_IP_PRE_ROUTING;            //old Netfilter hook from previous Linux release
  nfho.pf = PF_INET;                           //IPV4 packets
  nfho.priority = NF_IP_PRI_FIRST;             //set to highest priority over all other hook functions
  nf_register_hook(&nfho);                     //register hook

  return 0;                                    //return 0 for success
}

//Called when module unloaded using 'rmmod'
void cleanup_module()
{
  nf_unregister_hook(&nfho);                     //cleanup – unregister hook
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
