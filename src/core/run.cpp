#include "run.hpp"
#include "MoshLib.hpp"


using namespace mosh::control;


void mosh::run::base(const mosh::control::Mover& mover, const mosh::control::Quiter& quiter, bool hold_at_end) {
    while (quiter.tick()) mover.tick();
    mosh::env::robot.motorsReset();
    if (hold_at_end) goHold();
    motorL.setPWM(0);
    motorR.setPWM(0);
}

void mosh::run::time(const mosh::control::Mover& mover, uint32_t runtime) { base(mover, OnTimer(runtime)); }
