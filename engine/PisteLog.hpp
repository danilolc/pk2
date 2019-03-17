#ifndef PISTE_LOG
#define PISTE_LOG

#include <string>

enum class TYPE {
	T_UNKOWN = 0, T_INFO, T_WARNING, T_ERROR
};

enum class LOG_LEVEL_DEBUG {
	L_NOTHING, L_EVERYTHING, L_WARNING_ERROR, L_ERROR_ONLY
};

void PisteLog_Init(bool write);
void PisteLog_Allow_Write();
void PisteLog_Write(std::string file, std::string message, TYPE message_type = TYPE::T_INFO);
void PisteLog_Quit();

#endif