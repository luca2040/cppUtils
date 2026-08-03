#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>

#include "server.hpp"

#define charsize(arr) sizeof(arr) / sizeof(char)
#define MACHINE_NAME_BUF_SIZE 1024

namespace utils::server
{

Machine SServer::getMachineAddr()
{
    static char name[MACHINE_NAME_BUF_SIZE];
    static char ipaddr[INET_ADDRSTRLEN];

    if (gethostname(name, charsize(name)) != 0)
        name[0] = '\0';

    ifaddrs* ifaddr;

    bool found = false;

    if (getifaddrs(&ifaddr) != 0)
    {
        ipaddr[0] = '\0';
        return {
            .name = name, .ipaddr = ipaddr, .nameSz = charsize(name), .ipaddrSz = charsize(ipaddr)};
    }

    for (auto ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (!ifa->ifa_addr)
            continue;

        if (ifa->ifa_addr->sa_family != AF_INET)
            continue;

        if (ifa->ifa_flags & IFF_LOOPBACK)
            continue;

        if (!(ifa->ifa_flags & IFF_UP))
            continue;

        sockaddr_in* sa  = reinterpret_cast<sockaddr_in*>(ifa->ifa_addr);
        const char*  dst = inet_ntop(AF_INET, &sa->sin_addr, ipaddr, charsize(ipaddr));

        if (dst)
        {
            found = true;
            break;
        }
    }

    freeifaddrs(ifaddr);
    if (!found)
        ipaddr[0] = '\0';

    return {.name = name, .ipaddr = ipaddr, .nameSz = charsize(name), .ipaddrSz = charsize(ipaddr)};
}

} // namespace utils::server
