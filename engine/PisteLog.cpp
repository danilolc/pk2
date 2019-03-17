#include <fstream>
#include <iostream>

#include "PisteLog.hpp"

LOG_LEVEL_DEBUG log_level = LOG_LEVEL_DEBUG::L_NOTHING;

#ifdef DEBUG
	log_level = LOG_LEVEL_DEBUG::L_EVERYTHING;
#endif // DEBUG

std::ofstream fs;

bool write_log = false;

void PisteLog_Init(bool wl) {
	write_log = wl;

	if (write_log) {
		fs.open("log.txt");
		fs << "- PisteLog initialized -\n";
	}
}

void PisteLog_Quit() {
	if (write_log) {
		fs.flush();
		fs.close();
	}
}

void PisteLog_Allow_Write() {
	write_log = true;	
}

void PisteLog_Write(std::string file, std::string message, TYPE message_type) {
	if (write_log) {
		std::string level = "UNKOWN";

		switch (message_type) {
		case TYPE::T_INFO:
			level = "[INFO] ";
			break;

		case TYPE::T_WARNING:
			level = "[WARNING] ";
			break;

		case TYPE::T_ERROR:
			level = "[ERROR] ";
			break;
		}

		// Logging to stdout/stderr, if in debug mode
		switch (log_level) {
			case LOG_LEVEL_DEBUG::L_EVERYTHING:
				std::cout << level << file << " - " << message;
				break;

			case LOG_LEVEL_DEBUG::L_WARNING_ERROR:
				if (message_type == TYPE::T_WARNING || message_type == TYPE::T_ERROR) {
					std::cerr << level << file << " - " << message;
					break;
				}

			case LOG_LEVEL_DEBUG::L_ERROR_ONLY:
				if (message_type == TYPE::T_ERROR) {
					std::cerr << "[ERROR] " << file << " - " << message;
					break;
				}
		}

		fs << level << file << " - " << message << "\n";
	}
}