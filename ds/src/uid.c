/*--------------------------------------------------------------------------
submitter : Rozaline Kozly
reviewer : ? 
version : 1
date : 05 jan 2026
stage : writing code
----------------------------------------------------------------------------*/
#include <time.h>       /* time, difftime */
#include <unistd.h>     /* getpid         */
#include <netinet/in.h> /* sockaddr_in    */
#include <arpa/inet.h>  /* inet_ntoa      */
#include <ifaddrs.h>    /* getifaddrs     */
#include <string.h>     /* strcpy, strcmp */
#include <linux/if.h>     /* IFF_LOOPBACK   */

#include "uid.h"        /* API            */
/*---------------------------------------------------------------------------*/
typedef volatile size_t atomic_ty;
/*---------------------------------------------------------------------------*/
const uid_ty invalid_uid_g = {0};
static atomic_ty uid_counter = 0;
/*---------------------------------------------------------------------------*/
/* Internal helper to find and copy the IP into the struct's array */
static void SetIp(char* ip_buffer)
{
    struct ifaddrs *ifap = NULL;
    struct ifaddrs *ifa = NULL;
    struct sockaddr_in *sa = NULL;

    if (-1 == getifaddrs(&ifap))
    {
        strcpy(ip_buffer, "");
        return;
    }

    for (ifa = ifap; NULL != ifa ; ifa = ifa->ifa_next)
    {
        /* Check for IPv4 addresses */
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            sa = (struct sockaddr_in *)ifa->ifa_addr;
            
            /* Copy the IP string directly into the struct's internal buffer */
            strcpy(ip_buffer, inet_ntoa(sa->sin_addr));

            /* If it's a real network IP (not loopback), we stop looking */
            if (!(ifa->ifa_flags & IFF_LOOPBACK))
            {
                break;
            }
        }
    }

    freeifaddrs(ifap);
}

/*---------------------------------------------------------------------------*/
uid_ty UidCreate(void)
{
    uid_ty new_uid = {0};

    new_uid.counter = __atomic_add_fetch(&uid_counter, 1, __ATOMIC_RELAXED);
    /*check if failed */
    new_uid.time = time(NULL);
    
    if (new_uid.time == (time_t)-1)
    {
        return invalid_uid_g;
    }

    new_uid.pid = getpid();

    SetIp(new_uid.ip);
    
    /* Ensure we actually found a valid IP string */
    if (strcmp(new_uid.ip, "") == 0)
    {
        return invalid_uid_g;
    }

    return new_uid;
}

/*---------------------------------------------------------------------------*/
int IsMatchId(uid_ty id_1, uid_ty id_2)
{
    return (id_1.counter == id_2.counter &&
            id_1.pid == id_2.pid &&
            difftime(id_1.time, id_2.time) == 0 &&
            strcmp(id_1.ip, id_2.ip) == 0);
}
