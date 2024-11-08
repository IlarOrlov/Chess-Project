//#include "ChessGame.h"
//
//int main() {
//	SECURITY_ATTRIBUTES saAttr;
//	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
//	saAttr.bInheritHandle = TRUE;
//	saAttr.lpSecurityDescriptor = nullptr;
//
//	// Create the pipe
//	if (!CreatePipe(&PipeCommunicator::pipeHandles[0], &PipeCommunicator::pipeHandles[1], &saAttr, 0)) {
//		std::cerr << "Error creating pipe." << std::endl;
//		return 1;
//	}
//
//	// Create an instance of PipeCommunicator using the static getInstance method
//	PipeCommunicator& communicator = PipeCommunicator::getInstance();
//	ChessGame qGame;
//
//	qGame.Start(communicator);
//	return 0;
//}