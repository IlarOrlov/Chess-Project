#ifndef PIPE_COMMUNICATOR_H
#define PIPE_COMMUNICATOR_H

#include <iostream>
#include <windows.h>
#include <string>

class PipeCommunicator {
public:
    static PipeCommunicator& getInstance();
    static HANDLE pipeHandles[2];  // Static member for the pipe handles

    void writeToPipe(const std::string& message);
    std::string readFromPipe();
    void clearPipe();

private:
    PipeCommunicator(HANDLE readEnd, HANDLE writeEnd);

    HANDLE readEnd;
    HANDLE writeEnd;
};

#endif // PIPE_COMMUNICATOR_H