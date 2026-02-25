#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <tuple>
#include <limits>
#include <utility>
#include <string>

#include "message_types.hpp"

using namespace cadmium;
using namespace std;

struct Compressor_defs {
    struct cmd_in     : public in_port<CompressorCommand> {};
    struct status_out : public out_port<CoolingStatus> {};
};

template<typename TIME>
class Compressor {
public:
    using input_ports = tuple<
        typename Compressor_defs::cmd_in
    >;

    using output_ports = tuple<
        typename Compressor_defs::status_out
    >;

    using input_bags = typename make_message_bags<input_ports>::type;
    using output_bags = typename make_message_bags<output_ports>::type;

    struct state_type {
        CoolingStatus mode;
        bool output_pending;

        friend std::ostream& operator<<(std::ostream& os, const state_type& s) {
            os << "{mode:" << s.mode
               << ", output_pending:" << (s.output_pending ? "true" : "false")
               << "}";
            return os;
        }
    };

    state_type state;

    Compressor() {
        state.mode = CoolingStatus::OFF;
        state.output_pending = true; // emit initial status if desired
    }

    void internal_transition() {
        state.output_pending = false;
    }

    void external_transition(TIME /*e*/, input_bags mbs) {
        for (const auto &cmd : get_messages<typename Compressor_defs::cmd_in>(mbs)) {
            CoolingStatus new_mode =
                (cmd == CompressorCommand::CMD_ON) ? CoolingStatus::ON : CoolingStatus::OFF;

            if (new_mode != state.mode) {
                state.mode = new_mode;
                state.output_pending = true;
            }
        }
    }

    void confluence_transition(TIME e, input_bags mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    output_bags output() const {
        output_bags bags;
        if (state.output_pending) {
            get_messages<typename Compressor_defs::status_out>(bags).push_back(state.mode);
        }
        return bags;
    }

    TIME time_advance() const {
        if (state.output_pending) return TIME("00:00:00:000");
        return std::numeric_limits<TIME>::infinity();
    }
};

#endif