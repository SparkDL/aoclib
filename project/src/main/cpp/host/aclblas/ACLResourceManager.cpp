//
// Created by pcz on 8/27/18.
//

#include "ACLResourceManager.h"

namespace saoclib {

    ACLResourceManager::ACLResourceManager(const char *binaryPath) {
        context = new CLContext;
        program = new CLProgram(context, binaryPath);
    }

    const CLContext *ACLResourceManager::getContext() const {
        return context;
    }

    const CLProgram *ACLResourceManager::getProgram() const {
        return program;
    }

    ACLResourceManager::~ACLResourceManager() {
        delete program;
        delete context;
        for (auto p:accelerators) {
            delete p;
        }
    }

    bool ACLResourceManager::allocateAccelerator(ACLBlasAccelerator **accelerator, std::string &msg) {
        initedLock.lock();
        if (!inited) {
            initState = initResource(msg);
            inited = true;
        }
        initedLock.unlock();

        if (!initState) {
            log("Init OpenCL failed.%s", msg.c_str());
            return false;
        }

        log("*** Allocating Accelerator ***\n");
        log("Free Devices:%d,Busy devices:%d\n", freeDevices.size(), busyDevices.size());

        devicesLock.lock();
        if (freeDevices.empty()) {
            devicesLock.unlock();
            msg = "No free devices.";
            return false;
        } else {
            auto device = freeDevices.back();
            busyDevices.push_back(device);
            freeDevices.pop_back();
            devicesLock.unlock();
            auto ret = new ACLBlasAccelerator(this, device);
            accelerators.push_back(ret);
            *accelerator = ret;
            return true;
        }

    }

    bool ACLResourceManager::initResource(std::string &reason) {
        try {
            context->initOpenCL();
            auto allDevices = context->getDevices();
            program->deploy(allDevices);
            freeDevices = program->getDeployedDevices();
            return true;
        }
        catch (std::runtime_error &error) {
            reason = error.what();
            return false;
        }
    }
}