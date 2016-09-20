CONTIKI_PROJECT=static-client-tutorial
CONTIKI=contiki
LWM2M_DIR=AwaLWM2M
TARGET=mikro-e

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\" -DVERSION='\"$(VERSION)\"'
CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -I$(LWM2M_DIR)/api/include -DLWM2M_CLIENT -g -DDTLS_PEERS_NOHASH=1

CFLAGS += -DUIP_CONF_BUFFER_SIZE=4096

APPS += erbium

APPDIRS += $(LWM2M_DIR)/lib
APPDIRS += $(LWM2M_DIR)/core/src

APPS += client
APPS += common


APPS += er-coap
APPS += rest-engine
APPS += jsmn
APPS += b64
#APPS += hmac
APPS += libobjects
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

PAN_ID=0xdcba
CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_RPL = 0
USE_CA8210 = 1
RESOLV_CONF_SUPPORTS_MDNS = 0
UIP_CONF_TCP = 1
TINYDTLS = 1

CFLAGS += -I $(CONTIKI)/platform/$(TARGET)
CFLSGS += -fno-short-double
LDFLAGS += -Wl,--defsym,_min_heap_size=64000

SMALL=0


all: static-client-tutorial
	xc32-bin2hex static-client-tutorial.$(TARGET)
include $(CONTIKI)/Makefile.include
