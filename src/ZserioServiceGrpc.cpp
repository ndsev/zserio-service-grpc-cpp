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

        const std::string& requestData = request->requestdata();
        std::vector<uint8_t> responseData;

        m_service.callMethod(request->methodname(), {requestData.begin(), requestData.end()}, responseData,
                serverContext);

        response->set_responsedata({responseData.begin(), responseData.end()});
        return grpc::Status::OK;
    }

    GrpcClient::GrpcClient(const std::shared_ptr<grpc::Channel>& channel)
    :   m_stub(ZserioService::NewStub(channel))
    {
    }

    void GrpcClient::callMethod(const std::string& methodName, const std::vector<uint8_t>& requestData,
                std::vector<uint8_t>& responseData, void* context)
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

    void GrpcClient::callMethodWithContext(const std::string& methodName,
            const std::vector<uint8_t>& requestData, std::vector<uint8_t>& responseData,
            grpc::ClientContext* context)
    {
        Request request;
        request.set_methodname(methodName);
        request.set_requestdata({requestData.begin(), requestData.end()});

        Response response;
        grpc::Status status = m_stub->callMethod(context, request, &response);

        if (status.ok())
        {
            const std::string& grpcResponseData = response.responsedata();
            responseData.assign(grpcResponseData.begin(), grpcResponseData.end());
        }
        else
        {
            throw ::zserio::ServiceException("gRPC call failed: " + status.error_message());
        }
    }
} // namespace zserio_service_grpc
