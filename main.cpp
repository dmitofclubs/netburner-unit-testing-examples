#include <predef.h>
#include <stdio.h>
#include <ctype.h>
#include <startnet.h>
#include <autoupdate.h>
#include <dhcpclient.h>
#include <smarttrap.h>
#include <taskmon.h>
#include <NetworkDebug.h>
#include "tests.h"

extern "C" {
void UserMain(void * pd);
}

const char * AppName="netburner-unit-testing-examples";

void UserMain(void * pd) {
    InitializeStack();
    GetDHCPAddressIfNecessary();
    OSChangePrio(MAIN_PRIO);
    EnableAutoUpdate();
    EnableTaskMonitor();

    #ifndef _DEBUG
    EnableSmartTraps();
    #endif

    #ifdef _DEBUG
    InitializeNetworkGDB_and_Wait();
    #endif

    printf("Application started\n");
    test_main();
    printf("Testing is done\n");
    while (1) {
        OSTimeDly(TICKS_PER_SECOND);
    }
}
