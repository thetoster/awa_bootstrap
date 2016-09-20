#include <string.h>
#include <stdio.h>
#include "contiki.h"
#include "contiki-net.h"
#include "awa/static.h"

#define HEATER_INSTANCES 1

typedef struct
{
    char Manufacturer[64];
    AwaFloat Temperature;

} HeaterObject;

static HeaterObject heater[HEATER_INSTANCES];

static void DefineHeaterObject(AwaStaticClient * awaClient)
{
    AwaStaticClient_DefineObject(awaClient, 1000, "Heater", 0, HEATER_INSTANCES);
    AwaStaticClient_DefineResource(awaClient, 1000, 101, "Manufacturer", AwaResourceType_String, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, 1000, 101, &heater[0].Manufacturer, sizeof(heater[0].Manufacturer), sizeof(heater[0]));
    AwaStaticClient_DefineResource(awaClient, 1000, 104, "Temperature", AwaResourceType_Float, 0, 1, AwaResourceOperations_ReadOnly);
    AwaStaticClient_SetResourceStorageWithPointer(awaClient, 1000, 104, &heater[0].Temperature, sizeof(heater[0].Temperature), sizeof(heater[0]));
}

static void SetInitialValues(AwaStaticClient * awaClient)
{
    int instance = 0;

    AwaStaticClient_CreateObjectInstance(awaClient, 1000, instance);

    AwaStaticClient_CreateResource(awaClient, 1000, instance, 101);
    strcpy(heater[instance].Manufacturer, "HotAir Systems Inc");

    AwaStaticClient_CreateResource(awaClient, 1000, instance, 104);
    heater[instance].Temperature = 0.0;
}

PROCESS(lwm2m_client, "Awa LWM2M Example Client");
AUTOSTART_PROCESSES(&lwm2m_client);

PROCESS_THREAD(lwm2m_client, ev, data)
{
    PROCESS_BEGIN();

    static AwaFactoryBootstrapInfo bootstrapinfo = { 0 };
    static AwaStaticClient * awaClient;

    awaClient = AwaStaticClient_New();

    AwaStaticClient_SetLogLevel(AwaLogLevel_Debug);
    AwaStaticClient_SetEndPointName(awaClient, "AwaStaticClient1");
    AwaStaticClient_SetCoAPListenAddressPort(awaClient, "::", 15683);
    AwaStaticClient_SetBootstrapServerURI(awaClient, "coap://[fd77:9c74:ddee:c0e7::1]:15683");
    AwaStaticClient_Init(awaClient);

    sprintf(bootstrapinfo.SecurityInfo.ServerURI, "%s", "coap://[fe80::219:f5ff:fe89:bfc]:15683");
    bootstrapinfo.SecurityInfo.SecurityMode = AwaSecurityMode_NoSec;
    sprintf(bootstrapinfo.SecurityInfo.PublicKeyOrIdentity, "LJYWT3tWA0a8fayY2ifvvA");
    sprintf(bootstrapinfo.SecurityInfo.SecretKey, "0F92CCE0E7C8FF20EE29A8ADD40A6CA9ABA06A360122E0DE9ABF53FCF03B7D2C");

    bootstrapinfo.ServerInfo.Lifetime = 60;
    bootstrapinfo.ServerInfo.DefaultMinPeriod = 1;
    bootstrapinfo.ServerInfo.DefaultMaxPeriod = -1;
    bootstrapinfo.ServerInfo.DisableTimeout = 86400;
    bootstrapinfo.ServerInfo.Notification = false;
    sprintf(bootstrapinfo.ServerInfo.Binding, "U");

//    AwaStaticClient_SetFactoryBootstrapInformation(awaClient, &bootstrapinfo);

    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, BOOTSTRAP_IPv6_ADDR1, BOOTSTRAP_IPv6_ADDR2, BOOTSTRAP_IPv6_ADDR3,
        BOOTSTRAP_IPv6_ADDR4, BOOTSTRAP_IPv6_ADDR5, BOOTSTRAP_IPv6_ADDR6, BOOTSTRAP_IPv6_ADDR7,
        BOOTSTRAP_IPv6_ADDR8);
    uip_ds6_defrt_add(&ipaddr, 0);

    uip_nameserver_update(&ipaddr, UIP_NAMESERVER_INFINITE_LIFETIME);

    DefineHeaterObject(awaClient);
    SetInitialValues(awaClient);

    static struct etimer et;

    while (1)
    {
        static int waitTime;

        printf("Running the loop\n");
        waitTime = AwaStaticClient_Process(awaClient);
        printf("Post awa process\n");


        //heater[0].Temperature = value from hardware
//        AwaStaticClient_ResourceChanged(awaClient, 1000, 0, 104);
        printf("Resource changed\n");

        etimer_set(&et, (2 * waitTime * CLOCK_SECOND) / 1000);
        PROCESS_YIELD();
        waitTime = 0;
    }

    AwaStaticClient_Free(&awaClient);

    PROCESS_END();
}
