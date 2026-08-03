#include "server.hpp"

#include <string.h>

namespace utils::server
{

Method SEndpoint::strmetToVal(const char* met)
{
    static const char* GET     = "GET";
    static const char* HEAD    = "HEAD";
    static const char* POST    = "POST";
    static const char* PUT     = "PUT";
    static const char* DELETE  = "DELETE";
    static const char* CONNECT = "CONNECT";
    static const char* OPTIONS = "OPTIONS";
    static const char* TRACE   = "TRACE";
    static const char* PATCH   = "PATCH";

    if (strcmp(met, GET) == 0)
        return Method::GET;
    else if (strcmp(met, HEAD) == 0)
        return Method::HEAD;
    else if (strcmp(met, POST) == 0)
        return Method::POST;
    else if (strcmp(met, PUT) == 0)
        return Method::PUT;
    else if (strcmp(met, DELETE) == 0)
        return Method::DELETE;
    else if (strcmp(met, CONNECT) == 0)
        return Method::CONNECT;
    else if (strcmp(met, OPTIONS) == 0)
        return Method::OPTIONS;
    else if (strcmp(met, TRACE) == 0)
        return Method::TRACE;
    else if (strcmp(met, PATCH) == 0)
        return Method::PATCH;
    else
        return Method::UNK;
}

bool SEndpoint::operator==(const SEndpoint& rhs) const
{
    if (method != rhs.method)
        return false;

    if (strcmp(url, rhs.url) != 0)
        return false;

    return true;
}

void SServer::removePtr(void** ptr)
{
    std::lock_guard<std::mutex> guard(iterationMapMut);

    uintptr_t ptrptr = reinterpret_cast<uintptr_t>(*ptr);
    if (iterationMap.contains(ptrptr))
    {
        if (userFree)
            userFree(iterationMap[ptrptr].userPtr);
        iterationMap.erase(ptrptr);
    }

    *ptr = nullptr;
}

bool SServer::isFirstCall(size_t callNum) { return callNum <= 1; }

bool SServer::consumeData(size_t* dataSz)
{
    if (*dataSz)
    {
        *dataSz = 0;
        return true;
    }
    return false;
}

MHD_Result SServer::serverEcho(void* that_, MHD_Connection* connection, const char* url,
                               const char* method, const char* version, const char* uploadData,
                               size_t* uploadDataSize, void** ptr)
{
    SServer* that = reinterpret_cast<SServer*>(that_);
    Method   met  = SEndpoint::strmetToVal(method);

    UserPtr currentPtr;
    if (!*ptr)
    {
        std::lock_guard<std::mutex> guard(that->iterationMapMut);

        uintptr_t newId = that->iterationIdCount.newVal();
        currentPtr      = {.callNum = 0, .userPtr = nullptr};

        if (that->userAlloc)
            currentPtr.userPtr = that->userAlloc();

        that->iterationMap[newId] = currentPtr;

        *ptr = reinterpret_cast<void*>(newId);
        return MHD_YES;
    }
    else
    {
        std::lock_guard<std::mutex> guard(that->iterationMapMut);

        auto& thatPtr = that->iterationMap[reinterpret_cast<uintptr_t>(*ptr)];
        thatPtr.callNum++;
        currentPtr = thatPtr;
    }

    auto endpointAndClean = [&](auto&& endpFunc) -> decltype(auto)
    {
        bool removePtr = !*uploadDataSize;
        auto res       = endpFunc();
        if (removePtr)
            that->removePtr(ptr);
        return res;
    };

    if (that->genericCallback)
        that->genericCallback(that, connection, url, method, version, uploadData, uploadDataSize,
                              currentPtr.callNum, currentPtr.userPtr);

    SEndpoint compareTarget = {met, url};
    if (that->endpoints.contains(compareTarget))
        return endpointAndClean(
            [&]
            {
                return that->endpoints[compareTarget](that, connection, version, uploadData,
                                                      uploadDataSize, currentPtr.callNum,
                                                      currentPtr.userPtr);
            });

    if (that->endp404)
        return endpointAndClean(
            [&]
            {
                return that->endp404(that, connection, url, met, version, uploadData,
                                     uploadDataSize, currentPtr.callNum, currentPtr.userPtr);
            });

    // default 404 page in case all the others dont exist
    static const char* page = "Error 404 - not found";

    MHD_Response* response;
    MHD_Result    ret;

    if (consumeData(uploadDataSize))
        return MHD_YES;
    else
        that->removePtr(ptr);

    response = MHD_create_response_from_buffer(strlen(page), (void*)page, MHD_RESPMEM_PERSISTENT);
    ret      = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return ret;
}

void SServer::setPanicCallback(MHD_PanicCallback cb) { MHD_set_panic_func(cb, this); }

void SServer::setUserAlloc(useralloc_t ualloc) { userAlloc = ualloc; }
void SServer::setUserFree(userfree_t ufree) { userFree = ufree; }

void SServer::addEndpoint(SEndpoint endp, endpoint_t func) { endpoints[endp] = func; }
void SServer::set404Endpoint(err404_t endp) { endp404 = endp; }
void SServer::setGenericCallback(callback_t cllbk) { genericCallback = cllbk; }

bool SServer::sstart(const uint16_t& port)
{
    daemon = MHD_start_daemon(MHD_USE_THREAD_PER_CONNECTION, port, NULL, NULL, serverEcho, this,
                              MHD_OPTION_END);
    if (!daemon)
        return false;

    return true;
}

bool SServer::sstop()
{
    if (!daemon)
        return false;

    MHD_stop_daemon(daemon);
    return true;
}

} // namespace utils::server
