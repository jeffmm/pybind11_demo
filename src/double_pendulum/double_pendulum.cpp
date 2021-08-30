#include "double_pendulum.hpp"

#define G 9.81
#define SQR(x) ((x) * (x))

DoublePendulum::DoublePendulum(double m1, double m2, double l1, double l2, 
                               double th1, double th2) {
    SetMasses(m1, m2);
    SetLengths(l1, l2);
    SetAngles(th1, th2);
}

void DoublePendulum::SetMasses(double m1, double m2) {
    _m1 = m1;
    _m2 = m2;
}

void DoublePendulum::SetLengths(double l1, double l2) {
    _l1 = l1;
    _l2 = l2;
}

void DoublePendulum::SetAngles(double th1, double th2) {
    _th1_init = th1;
    _th2_init = th2;
}

void DoublePendulum::Simulate(int n_steps, double dt, int n_record) {
    _time = 0;
    _th1 = _th1_init;
    _th2 = _th2_init;
    _v1 = 0;
    _v2 = 0;
    _dt = dt;
    InitRecord(n_steps, n_record);
    for (int i=0; i < n_steps; i++) {
        // Record pendulum data at beginning, and every n_record steps
        _time = _dt * i;
        if (i % n_record == 0) {
            RecordData();
        }
        // Integrate pendulum equations of motion
        UpdatePendulum();
    }
    // Record final time data
    _time = n_steps * _dt;
    RecordData();
}

void DoublePendulum::ComputeAcceleration() {
    _a1 = (-G * (2 * _m1 + _m2) * sin(_th1) - _m2 * G * sin(_th1 - 2 * _th2)
        - 2 * sin(_th1 - _th2) * _m2 * (SQR(_v2) * _l2 
            + SQR(_v1) *_l1 * cos(_th1 - _th2))) /
        (_l1 * (2 * _m1 + _m2 * (1 - cos(2 * (_th1 - _th2)))));

    _a2 = (2 * sin(_th1 - _th2) * (SQR(_th1) * _l1 * (_m1 + _m2) 
                + G * (_m1 + _m2) * cos(_th1) 
            + SQR(_th2) * _l2 * _m2 * cos(_th1 - _th2))) / 
        (_l2 * (2 * _m1 + _m2 * (1 - cos(2 * (_th1 - _th2)))));
}

void DoublePendulum::UpdatePendulum() {
    ComputeAngles();
    ComputeVelocity();
}

void DoublePendulum::ComputeAngles() {
    // Compute (velocity-dependent) acceleration
    ComputeAcceleration();

    _th1 += (_v1 + 0.5 * _a1 * _dt) * _dt;
    _th2 += (_v2 + 0.5 * _a2 * _dt) * _dt;
}

// Compute velocity in two steps, accounting for velocity-dependent acceleration from
// updated velocity
void DoublePendulum::ComputeVelocity() {
    // Halfstep
    _v1 += 0.5 * _a1 * _dt;
    _v2 += 0.5 * _a2 * _dt;

    double prev_a1 = _a1;
    double prev_a2 = _a2;

    // Depends on velocity
    ComputeAcceleration();

    // Full step
    _v1 += 0.5 * (_a1 - prev_a1) * _dt;
    _v2 += 0.5 * (_a2 - prev_a2) * _dt;

}

// Compute x, y position of pendulums
void DoublePendulum::ComputePosition() {
    _x1 = _l1 * sin(_th1);
    _y1 = -_l1 * cos(_th1);
    _x2 = _x1 + _l2 * sin(_th2);
    _y2 = _y1 - _l2 * cos(_th2);
}

void DoublePendulum::RecordData() {
    ComputePosition();
    _data[_idx][0] = _time;
    _data[_idx][1] = _x1;
    _data[_idx][2] = _y1;
    _data[_idx][3] = _th1;
    _data[_idx][4] = _x2;
    _data[_idx][5] = _y2;
    _data[_idx][6] = _th2;
    _idx++;
    /* std::cout << _time << " " << _x1 << " " << _y1 << " " << _th1 << " " */
              /* << _x2 << " " << _y2 << " " << _th2 << "\n"; */
}

void DoublePendulum::InitRecord(int n_steps, int n_record) {
    // Initialize output array
    _n_data = n_steps / n_record + 1;
    if (n_steps % n_record > 0) {
        _n_data++;
    }
    _data = new float*[_n_data];
    for (int i=0; i<_n_data; ++i) {
        _data[i] = new float[7];
    }
    _idx = 0;
}
