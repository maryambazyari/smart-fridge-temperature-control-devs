#ifndef TEMP_SENSOR_HPP
#define TEMP_SENSOR_HPP

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <tuple>
#include <limits>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <utility>

#include "message_types.hpp"

using namespace cadmium;
using namespace std;

// ------------ Ports ------------
struct TempSensor_defs {
    struct cooling_status_in : public in_port<CoolingStatus> {};
    struct disturbance_in    : public in_port<double> {};   // additive temp disturbance
    struct temp_out          : public out_port<double> {};
};

// ------------ Atomic Model ------------
template<typename TIME>
class TempSensor {
public:
    // Port definitions (IMPORTANT: define before using in function signatures)
    using input_ports = tuple<
        typename TempSensor_defs::cooling_status_in,
        typename TempSensor_defs::disturbance_in
    >;

    using output_ports = tuple<
        typename TempSensor_defs::temp_out
    >;

    using input_bags = typename make_message_bags<input_ports>::type;
    using output_bags = typename make_message_bags<output_ports>::type;

    // State
    struct state_type {
        double T;                    // current temperature
        bool cooling_on;
        double pending_disturbance;  // one-shot additive disturbance

        friend std::ostream& operator<<(std::ostream& os, const state_type& s) {
            os << "{T:" << s.T
               << ", cooling_on:" << (s.cooling_on ? "true" : "false")
               << ", pending_disturbance:" << s.pending_disturbance
               << "}";
            return os;
        }
    };

    state_type state;

    // Parameters
    double sample_time_sec;  // seconds
    double r_warm;           // warming rate per internal step
    double r_cool;           // cooling rate per internal step
    double Tmin_clip;
    double Tmax_clip;

    // Helper: convert seconds to NDTime-like string "hh:mm:ss:ms"
    static TIME seconds_to_TIME(double sec) {
        if (sec < 0) sec = 0;
        long long total_ms = static_cast<long long>(std::llround(sec * 1000.0));

        long long hh = total_ms / 3600000; total_ms %= 3600000;
        long long mm = total_ms / 60000;   total_ms %= 60000;
        long long ss = total_ms / 1000;
        long long ms = total_ms % 1000;

        std::ostringstream oss;
        oss << std::setfill('0')
            << std::setw(2) << hh << ":"
            << std::setw(2) << mm << ":"
            << std::setw(2) << ss << ":"
            << std::setw(3) << ms;
        return TIME(oss.str());
    }

    // Constructor
    TempSensor() {
        state.T = 8.0;
        state.cooling_on = false;
        state.pending_disturbance = 0.0;

        sample_time_sec = 1.0;
        r_warm = 0.4;
        r_cool = 0.6;
        Tmin_clip = -5.0;
        Tmax_clip = 30.0;
    }

    // Dynamic Cadmium instantiation currently passes 4 doubles (from your error log)
    TempSensor(double initT, double dt_sec, double warm, double cool) {
        state.T = initT;
        state.cooling_on = false;
        state.pending_disturbance = 0.0;

        sample_time_sec = dt_sec;
        r_warm = warm;
        r_cool = cool;
        Tmin_clip = -5.0;
        Tmax_clip = 30.0;
    }

    // Internal transition: update temperature periodically
    void internal_transition() {
        if (!state.cooling_on) state.T += r_warm;
        else                   state.T -= r_cool;

        // Apply one-shot disturbance if any
        state.T += state.pending_disturbance;
        state.pending_disturbance = 0.0;

        // Clip (optional)
        if (state.T < Tmin_clip) state.T = Tmin_clip;
        if (state.T > Tmax_clip) state.T = Tmax_clip;
    }

    // External transition: receive cooling status and/or disturbance
    void external_transition(TIME /*e*/, input_bags mbs) {
        for (const auto &msg : get_messages<typename TempSensor_defs::cooling_status_in>(mbs)) {
            state.cooling_on = (msg == CoolingStatus::ON);
        }

        for (const auto &d : get_messages<typename TempSensor_defs::disturbance_in>(mbs)) {
            state.pending_disturbance += d;
        }
    }

    // Confluent transition
    void confluence_transition(TIME e, input_bags mbs) {
        internal_transition();
        external_transition(TIME(), std::move(mbs));
    }

    // Output function: emit current temp
    output_bags output() const {
        output_bags bags;
        get_messages<typename TempSensor_defs::temp_out>(bags).push_back(state.T);
        return bags;
    }

    // Time advance
    TIME time_advance() const {
        return seconds_to_TIME(sample_time_sec);
    }
};

#endif