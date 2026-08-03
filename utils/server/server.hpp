#pragma once

#include <microhttpd.h>
#include <mutex>
#include <unordered_map>

#include "../unique.hpp"

namespace utils::server
{

// forward declarations
class SServer;

struct Machine
{
    char*  name;
    char*  ipaddr;
    size_t nameSz;
    size_t ipaddrSz;
};

enum Method
{
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    UNK,
};

// clang-format off
typedef MHD_Result (*endpoint_t) (SServer* that, MHD_Connection* connection, const char* version,
                                  const char* uploadData, size_t* uploadDataSize, size_t callNum,
                                  void* ptr);

typedef MHD_Result (*err404_t)   (SServer* that, MHD_Connection* connection, const char* url,
                                  Method method, const char* version, const char* uploadData,
                                  size_t* uploadDataSize, size_t callNum, void* ptr);

typedef void       (*callback_t) (SServer* that, MHD_Connection* connection, const char* url,
                                  const char* method, const char* version, const char* uploadData,
                                  size_t* uploadDataSize, size_t callNum, void* ptr);

typedef void*      (*useralloc_t)();
typedef void       (*userfree_t) (void* ptr);
// clang-format on

struct SEndpoint
{
  public:
    Method      method;
    const char* url;

    bool operator==(const SEndpoint& rhs) const;
    bool operator!=(const SEndpoint& rhs) const { return !(*this == rhs); }

    static Method strmetToVal(const char* met);
};

struct SEndpointHash
{
    size_t operator()(const SEndpoint& k) const noexcept
    {
        size_t h1 = std::hash<int>()(static_cast<int>(k.method));
        size_t h2 = std::hash<std::string_view>()(k.url);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

struct UserPtr
{
    size_t callNum = 0;
    void*  userPtr = nullptr;
};

class SServer
{
  private:
    MHD_Daemon*                                              daemon    = nullptr;
    std::unordered_map<SEndpoint, endpoint_t, SEndpointHash> endpoints = {};

    std::mutex                             iterationMapMut;
    utils::Unique<uintptr_t>               iterationIdCount = {};
    std::unordered_map<uintptr_t, UserPtr> iterationMap     = {}; // index, iteration number

    void removePtr(void** ptr);

    useralloc_t userAlloc = nullptr;
    userfree_t  userFree  = nullptr;

    err404_t          endp404         = nullptr;
    callback_t        genericCallback = nullptr;
    static MHD_Result serverEcho(void* that_, MHD_Connection* connection, const char* url,
                                 const char* method, const char* version, const char* uploadData,
                                 size_t* uploadDataSize, void** ptr);

  public:
    SServer() {};

    void setPanicCallback(MHD_PanicCallback cb);

    void setUserAlloc(useralloc_t ualloc);
    void setUserFree(userfree_t ufree);

    void addEndpoint(SEndpoint endp, endpoint_t func);
    void set404Endpoint(err404_t endp);
    void setGenericCallback(callback_t cllbk);

    static bool isFirstCall(size_t callNum);
    static bool consumeData(size_t* dataSz);

    bool sstart(const uint16_t& port);
    bool sstop();

    static Machine getMachineAddr();
};

} // namespace utils::server
