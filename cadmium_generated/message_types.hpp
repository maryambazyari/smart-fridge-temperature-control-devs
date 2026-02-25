#ifndef MESSAGE_TYPES_HPP
#define MESSAGE_TYPES_HPP

#include <iostream>

// Cooling status sent from Compressor -> TempSensor
enum class CoolingStatus {
    OFF = 0,
    ON  = 1
};

// Command sent from Controller -> Compressor
enum class CompressorCommand {
    CMD_OFF = 0,
    CMD_ON  = 1
};

// Pretty printing (useful for logs/debug)
inline std::ostream& operator<<(std::ostream& os, CoolingStatus s) {
    os << ((s == CoolingStatus::ON) ? "COOLING_ON" : "COOLING_OFF");
    return os;
}

inline std::ostream& operator<<(std::ostream& os, CompressorCommand c) {
    os << ((c == CompressorCommand::CMD_ON) ? "CMD_ON" : "CMD_OFF");
    return os;
}

#endif