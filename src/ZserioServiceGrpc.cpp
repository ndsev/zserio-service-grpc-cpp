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

        zserio::Span<const uint8_t> requestData(
                reinterpret_cast<const uint8_t*>(request->requestdata().data()), request->requestdata().size());

        ::zserio::IResponseDataPtr responseData = m_service.callMethod(
                request->methodname(), requestData, serverContext);
        response->set_responsedata({responseData->getData().begin(), responseData->getData().end()});

        return grpc::Status::OK;
    }

    GrpcClient::GrpcClient(const std::shared_ptr<grpc::Channel>& channel)
    :   m_stub(ZserioService::NewStub(channel))
    {
    }

    std::vector<uint8_t> GrpcClient::callMethod(zserio::StringView methodName,
            const zserio::RequestData& requestData, void* context)
    {
        if (context == nullptr)
        {
            grpc::ClientContext defaultContext;
            return callMethodWithContext(methodName, requestData, &defaultContext);
        }
        else
        {
            return callMethodWithContext(methodName, requestData, static_cast<grpc::ClientContext*>(context));
        }
    }

    std::vector<uint8_t> GrpcClient::callMethodWithContext(zserio::StringView methodName,
            const zserio::RequestData& requestData, grpc::ClientContext* context)
    {
        Request request;
        request.set_methodname(methodName.data(), methodName.size());
        request.set_requestdata({requestData.getData().begin(), requestData.getData().end()});

        Response response;
        grpc::Status status = m_stub->callMethod(context, request, &response);

        if (status.ok())
        {
            const std::string& grpcResponseData = response.responsedata();
            std::vector<uint8_t> responseData(grpcResponseData.length());
            std::copy(grpcResponseData.begin(), grpcResponseData.end(), responseData.begin());
            return responseData;
        }
        else
        {
            throw ::zserio::ServiceException("gRPC call failed: " + status.error_message());
        }
    }
} // namespace zserio_service_grpc
