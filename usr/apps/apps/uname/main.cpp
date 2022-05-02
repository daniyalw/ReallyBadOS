#include <stdio.h>
#include <system.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    auto info = System::info();

    printf("OS: %s\n", info.name);
    printf("Version: %s\n", info.version);
    printf("Vendor: %s\n", info.vendor);
    printf("CPU Name: %s\n", info.cpu_name);
    printf("Dev mode: %s\n", (char *)(info.dev ? "true" : "false"));
    printf("Has hypervisor: %s\n", (char *)(info.hypervisor ? "true" : "false"));
    printf("Model: %d\n", info.model);

    return 0;
}
