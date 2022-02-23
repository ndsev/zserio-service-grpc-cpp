#ifndef ZSERIO_SERVICE_GRPC_ZSERIO_SERVICE_GRPC_H
#define ZSERIO_SERVICE_GRPC_ZSERIO_SERVICE_GRPC_H

#include <memory>
#include <grpcpp/grpcpp.h>

#include "zserio/IService.h"
#include "zserio/ServiceException.h"

#include "zserio_service.pb.h"
#include "zserio_service.grpc.pb.h"

namespace zserio_service_grpc
{
    /**
     * Implementation of the gRPC service generated for ZserioService.
     *
     * GrpcService is intended to wrap implementation of a particular Zserio service and make it usable in gRPC.
     */
    class GrpcService : public ZserioService::Service
    {
    public:
        /**
         * Constructor.
         *
         * \param service Implementation of a particular Zserio service to wrap.
         */
        explicit GrpcService(zserio::IService& service);

        /** Implementation of gRPC ZserioService::Service. */
        grpc::Status callMethod(grpc::ServerContext* serverContext, const zserio_service_grpc::Request* request,
                zserio_service_grpc::Response* response) override;

    private:
        zserio::IService& m_service;
    };

    /** Implementation of gRPC ZserioService client as Zserio generic service interface. */
    class GrpcClient : public zserio::IServiceClient
    {
    public:
        /**
         * Constructor.
         *
         * \param channel gRPC channel to use.
         */
        explicit GrpcClient(const std::shared_ptr<grpc::Channel>& channel);

        /**
         * Implementation of IServiceClient::callMethod.
         */
        std::vector<uint8_t> callMethod(zserio::StringView methodName, const zserio::IServiceData& requestData,
                void* context) override;

    private:
        std::vector<uint8_t> callMethodWithContext(zserio::StringView methodName,
                const zserio::IServiceData& requestData, grpc::ClientContext* context);

        std::unique_ptr<zserio_service_grpc::ZserioService::Stub> m_stub;
    };
} // namespace zserio_service_grpc

#endif // ZSERIO_SERVICE_GRPC_ZSERIO_SERVICE_GRPC_H
