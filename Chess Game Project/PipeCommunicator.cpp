#include "PipeCommunicator.h"

// Initialize the static member
HANDLE PipeCommunicator::pipeHandles[2] = { nullptr, nullptr };

PipeCommunicator& PipeCommunicator::getInstance() {
    // Ensure that we create only one instance
    static PipeCommunicator instance(pipeHandles[0], pipeHandles[1]);
    return instance;
}

void PipeCommunicator::writeToPipe(const std::string& message) {
    // clear the pipe before writing
    clearPipe();
    // Write to the pipe
    WriteFile(writeEnd, message.c_str(), static_cast<DWORD>(message.size()), nullptr, nullptr);
}

std::string PipeCommunicator::readFromPipe() {
    char buffer[100]; // Adjust the buffer size as needed
    DWORD bytesRead;
    // Read from the pipe
    if (ReadFile(readEnd, buffer, sizeof(buffer), &bytesRead, nullptr)) {
        // Successful read
        return std::string(buffer, bytesRead);
    }
    else {
        // Error handling
        DWORD error = GetLastError();
        std::cerr << "ReadFile failed with error code " << error << std::endl;
        return ""; // Return an empty string or handle the error as appropriate
    }
}

void PipeCommunicator::clearPipe() {
    // Read any existing data from the pipe and discard it
    char buffer[100];
    DWORD bytesRead;
    while (PeekNamedPipe(readEnd, nullptr, 0, nullptr, &bytesRead, nullptr) && bytesRead > 0) {
        if (!ReadFile(readEnd, buffer, sizeof(buffer), &bytesRead, nullptr)) {
            // Error handling
            DWORD error = GetLastError();
            std::cerr << "ReadFile failed with error code " << error << std::endl;
            // Handle the error as appropriate, e.g., break the loop or return
            break;
        }
    }
}

PipeCommunicator::PipeCommunicator(HANDLE readEnd, HANDLE writeEnd) : readEnd(readEnd), writeEnd(writeEnd) {}