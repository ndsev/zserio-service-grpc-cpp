#include <algorithm>

#include "ZserioServiceGrpc.h"

namespace zserio_service_grpc
{
    GrpcService::GrpcService(::zserio::IService& service) : m_service(service)
    {
    }

    grpc::Status GrpcService::callMethod(grpc::ServerContext* serverContext, const Request* request,
            Response* response)
    {
        if (serverContext->IsCancelled())
            return grpc::Status(grpc::StatusCode::CANCELLED, "Client cancelled, abandoning.");

        const std::vector<uint8_t> requestData(request->requestdata().begin(), request->requestdata().end());
        zserio::BlobBuffer<> responseData;

        m_service.callMethod(request->methodname(), requestData, responseData, serverContext);

        response->set_responsedata({responseData.data().begin(), responseData.data().end()});
        return grpc::Status::OK;
    }

    GrpcClient::GrpcClient(const std::shared_ptr<grpc::Channel>& channel)
    :   m_stub(ZserioService::NewStub(channel))
    {
    }

    void GrpcClient::callMethod(zserio::StringView methodName, zserio::Span<const uint8_t> requestData,
            zserio::IBlobBuffer& responseData, void* context)
    {
        if (context == nullptr)
        {
            grpc::ClientContext defaultContext;
            callMethodWithContext(methodName, requestData, responseData, &defaultContext);
        }
        else
        {
            callMethodWithContext(methodName, requestData, responseData,
                    static_cast<grpc::ClientContext*>(context));
        }
    }

    void GrpcClient::callMethodWithContext(zserio::StringView methodName,
            zserio::Span<const uint8_t> requestData, zserio::IBlobBuffer& responseData,
            grpc::ClientContext* context)
    {
        Request request;
        request.set_methodname(methodName.data(), methodName.size());
        request.set_requestdata({requestData.begin(), requestData.end()});

        Response response;
        grpc::Status status = m_stub->callMethod(context, request, &response);

        if (status.ok())
        {
            const std::string& grpcResponseData = response.responsedata();
            responseData.resize(grpcResponseData.length());
            std::copy(grpcResponseData.begin(), grpcResponseData.end(), responseData.data().begin());
        }
        else
        {
            throw ::zserio::ServiceException("gRPC call failed: " + status.error_message());
        }
    }
} // namespace zserio_service_grpc
