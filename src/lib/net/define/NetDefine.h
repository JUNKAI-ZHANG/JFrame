#ifndef NET_DEFINE_NETDEFINE_H
#define NET_DEFINE_NETDEFINE_H

enum ServiceType {
    None_Type = 0,

    Client_Type = 1,

    GateServer_Type = 2,
    LogicServer_Type = 3,
    DataServer_Type = 4,
    RouteServer_Type = 5,

    Max_Service_Type
};

#endif  // NET_DEFINE_NETDEFINE_H