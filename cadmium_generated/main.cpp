#include <iostream>
#include <string>
#include <memory>
#include <typeinfo>

#include <NDTime.hpp>

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_atomic.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>

// Optional input reader (not used yet, can stay commented if it causes issues)
// #include <cadmium/basic_model/pdevs/iestream.hpp>

#include "message_types.hpp"
#include "TempSensor.hpp"
#include "Controller.hpp"
#include "Compressor.hpp"

using namespace std;
using namespace cadmium;

// ---- Top-level ports (SFS) ----
struct SFS_defs {
    struct disturbance_in : public in_port<double> {};
    struct temp_out       : public out_port<double> {};
    struct status_out     : public out_port<CoolingStatus> {};
    struct alarm_out      : public out_port<int> {};
};

int main() {
    using TIME = NDTime;

    // ----- Atomic models -----
    auto ts   = dynamic::translate::make_dynamic_atomic_model<TempSensor, TIME>("TS", 8.0, 1.0, 0.4, 0.6);
    auto ctrl = dynamic::translate::make_dynamic_atomic_model<Controller, TIME>("CTRL", 2.0, 6.0, 10.0);
    auto comp = dynamic::translate::make_dynamic_atomic_model<Compressor, TIME>("COMP");

    // ----- Ports for coupled model -----
    dynamic::modeling::Ports iports_SFS = {
        typeid(SFS_defs::disturbance_in)
    };

    dynamic::modeling::Ports oports_SFS = {
        typeid(SFS_defs::temp_out),
        typeid(SFS_defs::status_out),
        typeid(SFS_defs::alarm_out)
    };

    // ----- Submodels -----
    dynamic::modeling::Models submodels_SFS = {ts, ctrl, comp};

    // ----- EIC (External Input Couplings) -----
    dynamic::modeling::EICs eics_SFS = {
        dynamic::translate::make_EIC<SFS_defs::disturbance_in, TempSensor_defs::disturbance_in>("TS")
    };

    // ----- EOC (External Output Couplings) -----
    dynamic::modeling::EOCs eocs_SFS = {
        dynamic::translate::make_EOC<TempSensor_defs::temp_out, SFS_defs::temp_out>("TS"),
        dynamic::translate::make_EOC<Compressor_defs::status_out, SFS_defs::status_out>("COMP"),
        dynamic::translate::make_EOC<Controller_defs::alarm_out, SFS_defs::alarm_out>("CTRL")
    };

    // ----- IC (Internal Couplings) -----
    dynamic::modeling::ICs ics_SFS = {
        dynamic::translate::make_IC<TempSensor_defs::temp_out, Controller_defs::temp_in>("TS", "CTRL"),
        dynamic::translate::make_IC<Controller_defs::cmd_out, Compressor_defs::cmd_in>("CTRL", "COMP"),
        dynamic::translate::make_IC<Compressor_defs::status_out, TempSensor_defs::cooling_status_in>("COMP", "TS")
    };

    // ----- Coupled model: SmartFridgeSystem -----
    auto SFS = make_shared<dynamic::modeling::coupled<TIME>>(
        "SmartFridgeSystem",
        submodels_SFS,
        iports_SFS,
        oports_SFS,
        eics_SFS,
        eocs_SFS,
        ics_SFS
    );

    // ----- Run simulation -----
    dynamic::engine::runner<NDTime> r(SFS, NDTime("00:00:00:000"));
    r.run_until(NDTime("00:00:30:000"));

    cout << "Simulation finished." << endl;
    return 0;
}