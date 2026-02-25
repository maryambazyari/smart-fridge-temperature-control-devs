#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <tuple>
#include <limits>
#include <utility>
#include <string>

#include "message_types.hpp"

using namespace cadmium;
using namespace std;

struct Controller_defs {
    struct temp_in   : public in_port<double> {};
    struct cmd_out   : public out_port<CompressorCommand> {};
    struct alarm_out : public out_port<int> {};   // optional: 1 = alarm
};

template<typename TIME>
class Controller {
public:
    using input_ports = tuple<
        typename Controller_defs::temp_in
    >;

    using output_ports = tuple<
        typename Controller_defs::cmd_out,
        typename Controller_defs::alarm_out
    >;

    using input_bags = typename make_message_bags<input_ports>::type;
    using output_bags = typename make_message_bags<output_ports>::type;

    struct state_type {
        bool command_pending;
        CompressorCommand pending_cmd;
        bool alarm_pending;
        bool has_last_cmd;
        CompressorCommand last_cmd;
        double last_temp;

        friend std::ostream& operator<<(std::ostream& os, const state_type& s) {
            os << "{command_pending:" << (s.command_pending ? "true" : "false")
               << ", pending_cmd:" << s.pending_cmd
               << ", alarm_pending:" << (s.alarm_pending ? "true" : "false")
               << ", has_last_cmd:" << (s.has_last_cmd ? "true" : "false")
               << ", last_cmd:" << s.last_cmd
               << ", last_temp:" << s.last_temp
               << "}";
            return os;
        }
    };

    state_type state;

    // Thresholds
    double T_low;
    double T_high;
    double T_alarm;

    Controller() {
        T_low = 2.0;
        T_high = 6.0;
        T_alarm = 10.0;

        state.command_pending = false;
        state.pending_cmd = CompressorCommand::CMD_OFF;
        state.alarm_pending = false;
        state.has_last_cmd = false;
        state.last_cmd = CompressorCommand::CMD_OFF;
        state.last_temp = 0.0;
    }

    Controller(double low, double high, double alarm)
        : T_low(low), T_high(high), T_alarm(alarm) {
        state.command_pending = false;
        state.pending_cmd = CompressorCommand::CMD_OFF;
        state.alarm_pending = false;
        state.has_last_cmd = false;
        state.last_cmd = CompressorCommand::CMD_OFF;
        state.last_temp = 0.0;
    }

    void internal_transition() {
        state.command_pending = false;
        state.alarm_pending = false;
    }

    void external_transition(TIME /*e*/, input_bags mbs) {
        for (const auto &T : get_messages<typename Controller_defs::temp_in>(mbs)) {
            state.last_temp = T;

            // Alarm condition
            if (T >= T_alarm) {
                state.alarm_pending = true;
            }

            // Hysteresis control
            if (T > T_high) {
                if (!state.has_last_cmd || state.last_cmd != CompressorCommand::CMD_ON) {
                    state.pending_cmd = CompressorCommand::CMD_ON;
                    state.command_pending = true;
                    state.last_cmd = CompressorCommand::CMD_ON;
                    state.has_last_cmd = true;
                }
            } else if (T < T_low) {
                if (!state.has_last_cmd || state.last_cmd != CompressorCommand::CMD_OFF) {
                    state.pending_cmd = CompressorCommand::CMD_OFF;
                    state.command_pending = true;
                    state.last_cmd = CompressorCommand::CMD_OFF;
                    state.has_last_cmd = true;
                }
            }
            // If T in [T_low, T_high], hold last command (no new output)
        }
    }

    void confluence_transition(TIME e, input_bags mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    output_bags output() const {
        output_bags bags;

        if (state.command_pending) {
            get_messages<typename Controller_defs::cmd_out>(bags).push_back(state.pending_cmd);
        }
        if (state.alarm_pending) {
            get_messages<typename Controller_defs::alarm_out>(bags).push_back(1);
        }

        return bags;
    }

    TIME time_advance() const {
        if (state.command_pending || state.alarm_pending) return TIME("00:00:00:000");
        return std::numeric_limits<TIME>::infinity();
    }
};

#endif