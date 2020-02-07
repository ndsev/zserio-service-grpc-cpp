#include <math.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "ZserioServiceGrpc.h"
#include "calculator/Calculator.h"

/** Implementation of Zserio Calculator Service. */
class CalculatorService : public calculator::Calculator::Service
{
public:
    void powerOfTwoImpl(const calculator::I32& request, calculator::U64& response) override
    {
        response.setValue(request.getValue() * request.getValue());
        std::cout << "CalculatorServer: powerOfTwoImpl called, request=" << request.getValue()
                  << ", response=" << response.getValue() << std::endl;
    }

    void squareRootImpl(const calculator::Double& request, calculator::Double& response) override
    {
        response.setValue(sqrt(request.getValue()));
        std::cout << "CalculatorServer: squareRootImpl called, request=" << request.getValue()
                  << ", response=" << response.getValue() << std::endl;
    }
};

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (strncmp(argv[1], "-h", 2) == 0 || strncmp(argv[1], "--help", 6) == 0)
        {
            std::cout << "Usage: " << argv[0] << " [ADDRESS]" << std::endl;
            return 0;
        }
    }

    std::string address = "localhost:5000";
    if (argc > 1)
        address = argv[1];

    std::cout << "Welcome to Zserio Calculator gRPC Server example!"
              << "\n"
              << "Starting server (terminate with ^C) ..." << std::flush;

    CalculatorService calculatorService;
    // GrpcService from ZserioServiceGrpc library wraps the Zserio Calculator Service
    zserio_service_grpc::GrpcService calculatorGrpcService(calculatorService);

    grpc::ServerBuilder serverBuilder;
    serverBuilder.AddListeningPort(address, grpc::InsecureServerCredentials());
    // register the gRPC service provided by ZserioServiceGrpc library
    serverBuilder.RegisterService(&calculatorGrpcService);
    std::unique_ptr<grpc::Server> grpcServer(serverBuilder.BuildAndStart());

    std::cout << " OK!\n"
              << "Server is listening on " << address << " (terminate with ^C) ..." << std::endl;
    grpcServer->Wait();
}
