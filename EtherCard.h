// This code slightly follows the conventions of, but is not derived from:
//      EHTERSHIELD_H library for Arduino etherShield
//      Copyright (c) 2008 Xing Yu.  All right reserved. (this is LGPL v2.1)
// It is however derived from the enc28j60 and ip code (which is GPL v2)
//      Author: Pascal Stang 
//      Modified by: Guido Socher
//      DHCP code: Andrew Lindsay
// Hence: GPL V2
//
// jcw, 2010-05-19

#ifndef EtherCard_h
#define EtherCard_h

#include <WProgram.h>
#include <avr/pgmspace.h>
#include "enc28j60.h"

class BufferFiller : public Print {
  uint8_t *start, *ptr;
public:
  BufferFiller () {}
  BufferFiller (uint8_t* buf) : start (buf), ptr (buf) {}
      
  void emit_p (PGM_P fmt, ...);
  void emit_raw (const char* s, uint8_t n) { memcpy(ptr, s, n); ptr += n; }
  
  uint8_t* buffer () const { return start; }
  uint16_t position () const { return ptr - start; }
  
  virtual void write (uint8_t v) { *ptr++ = v; }
};

class EtherCard : public Ethernet {
public:
  static uint8_t mymac[6];  // my MAC address
  static uint8_t myip[4];   // my ip address
  static uint8_t mymask[4]; // my net mask
  static uint8_t gwip[4];   // gateway
  static uint8_t dnsip[4];  // dns server
  static uint8_t hisip[4];  // dns result
  static uint16_t hisport;  // tcp port to browse to
  
  static uint8_t begin (const uint16_t size, const uint8_t* macaddr) {
    copy_MAC(mymac, macaddr);
    return initialize(size, mymac);
  }
  
  // tcpip.cpp
  static void initIp (uint8_t *myip,uint16_t wwwp);
  static void makeUdpReply (char *data,uint8_t len, uint16_t port);
  static uint16_t packetLoop (uint16_t plen);
  static void httpServerReply (uint16_t dlen);
  static void setGwIp (const uint8_t *gwipaddr);
  static uint8_t clientWaitingGw ();
  static uint8_t clientTcpReq (uint8_t (*r)(uint8_t,uint8_t,uint16_t,uint16_t),
                               uint16_t (*d)(uint8_t),uint16_t port);
  static void browseUrl (prog_char *urlbuf, const char *urlbuf_varpart,
                         prog_char *hoststr,
                         void (*cb)(uint8_t,uint16_t,uint16_t));
  static void httpPost (prog_char *urlbuf, prog_char *hoststr,
                        prog_char *header, const char *postval,
                        void (*cb)(uint8_t,uint16_t,uint16_t));
  static void ntpRequest (uint8_t *ntpip,uint8_t srcport);
  static uint8_t ntpProcessAnswer (uint32_t *time, uint8_t dstport_l);
  static void udpPrepare (uint16_t sport, uint8_t *dip, uint16_t dport);
  static void udpTransmit (uint16_t len);
  static void sendUdp (char *data,uint8_t len,uint16_t sport,
                                              uint8_t *dip, uint16_t dport);
  static void registerPingCallback (void (*cb)(uint8_t*));
  static void clientIcmpRequest (const uint8_t *destip);
  static uint8_t packetLoopIcmpCheckReply (const uint8_t *ip_mh);
  static void sendWol (uint8_t *wolmac);
  // dhcp.cpp
  static bool dhcpSetup ();
  // dns.cpp
  static const bool dnsLookup (prog_char* name);
  // webutil.cpp
  static void copy_IP (uint8_t *dst, const uint8_t *src);
  static void copy_MAC (uint8_t *dst, const uint8_t *src);
  static void printIP (const char* msg, const uint8_t *buf);
  static uint8_t findKeyVal(const char *str,char *strbuf,
                            uint8_t maxlen, const char *key);
  static void urlDecode(char *urlbuf);
  static  void urlEncode(char *str,char *urlbuf);
  static uint8_t parseIp(uint8_t *bytestr,char *str);
  static void makeNetStr(char *rs,uint8_t *bs,uint8_t len,
                                              char sep,uint8_t base);
};

extern EtherCard ether;

#endif
