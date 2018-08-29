//
// Created by pcz on 8/27/18.
//

#ifndef SAOCLIB_CPP_ACLBLASPROGRAM_H
#define SAOCLIB_CPP_ACLBLASPROGRAM_H

#include <mutex>
#include "CLProgram.h"
#include "ACLBlasAccelerator.h"

namespace saoclib {

    // TODO this class must be thread safe
    class ACLResourceManager {
    public:
        ACLResourceManager(const char *binaryPath);

        const CLContext *getContext() const;

        const CLProgram *getProgram() const;

        ~ACLResourceManager();

        bool allocateAccelerator(ACLBlasAccelerator **accelerator, std::string &msg);

    private:
        bool initResource(std::string &reason);

        std::mutex devicesLock;
        std::mutex initedLock;
        bool inited = false;
        bool initState = false;
        std::vector<cl_device_id> freeDevices;
        std::vector<cl_device_id> busyDevices;
        std::vector<ACLBlasAccelerator *> accelerators;
        CLContext *context;
        CLProgram *program;
    };
}


#endif //SAOCLIB_CPP_ACLBLASPROGRAM_H