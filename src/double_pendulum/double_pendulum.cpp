#include "double_pendulum.hpp"

#define G 9.81  // Gravitational constant
#define SQR(x) ((x) * (x))

// Initialize pendulum parameters
DoublePendulum::DoublePendulum(double m1, double m2, double l1, double l2, 
                               double th1, double th2) {
    SetMasses(m1, m2);
    SetLengths(l1, l2);
    SetAngles(th1, th2);
}

// Set pendulum masses
void DoublePendulum::SetMasses(double m1, double m2) {
    _m1 = m1;
    _m2 = m2;
}

// Set pendulum lengths
void DoublePendulum::SetLengths(double l1, double l2) {
    _l1 = l1;
    _l2 = l2;
}

// Set initial pendulum angles (in degrees) relative to resting
void DoublePendulum::SetAngles(double th1, double th2) {
    _th1_init = M_PI * th1 / 180;
    _th2_init = M_PI * th2 / 180;
}

// Start a simulation of the pendulum given the pendulum parameters. The
// simulation runs for n_steps integration steps with a time delta of dt. The
// global error is O(dt^2). n_record specifies the frequency at which to record
// the pendulum data in an array. The first and final steps are always
// recorded, assuming n_steps > 0.
void DoublePendulum::Simulate(int n_steps, double dt, int n_record) {
    _time = 0;
    _dt = dt;
    // Initialize pendulum angles and velocity
    _th1 = _th1_init;
    _th2 = _th2_init;
    _v1 = 0;
    _v2 = 0;
    // Initialize output data array
    InitRecord(n_steps, n_record);
    // Begin integration of pendulum equations of motion
    for (int i=0; i < n_steps; i++) {
        // Record pendulum data at beginning, and every n_record steps
        if (i % n_record == 0) {
            _time = _dt * i;
            RecordData();
        }
        // Integrate pendulum equations of motion and update pendulum position
        UpdatePendulum();
    }
    // Record final time data
    _time = n_steps * _dt;
    RecordData();
}

// Angular acceleration for a double pendulum.
void DoublePendulum::ComputeAcceleration() {
    double d_th = _th1 - _th2;
    _a1 = (-_m2 * cos(d_th) * _l1 * SQR(_v1) * sin(d_th) 
            + _m2 * cos(d_th) * G * sin(_th2) - _m2 * _l2 * SQR(_v2) * sin(d_th)
            - (_m1 + _m2) * G * sin(_th1)) / (_l1 * (_m1 + _m2) - _m2 * SQR(cos(d_th)));

    _a2 = (_m1 + _m2) * (_l1 * SQR(_v1) * sin(d_th)
           + SQR(_v2) * sin(d_th) * cos(d_th) * _m2 * _l2 / (_m1 + _m2)
        + cos(d_th) * G * sin(_th1) - G * sin(_th2))
        / (_l2 * (_m1 + _m2 * SQR(sin(d_th))));
}

// Integrate pendulum equations of motion using modified velocity Verlet
// algorithm.
void DoublePendulum::UpdatePendulum() {
    ComputeAngles();
    ComputeVelocity();
}

// Update pendulum angles given current angular acceleration and velocity.
void DoublePendulum::ComputeAngles() {
    // Compute acceleration given the current angles and velocity
    ComputeAcceleration();

    // Update pendulum angles
    _th1 += (_v1 + 0.5 * _a1 * _dt) * _dt;
    _th2 += (_v2 + 0.5 * _a2 * _dt) * _dt;
}

// Compute velocity in two steps, accounting for the velocity-dependent
// acceleration. This is a modification of the usual velocity Verlet
// integration algorithm that accounts for an acceleration that depends on the
// current velocity.
void DoublePendulum::ComputeVelocity() {
    // Compute the midstep velocity using the acceleration computed using the
    // velocity from the previous step
    _v1 += 0.5 * _a1 * _dt;
    _v2 += 0.5 * _a2 * _dt;
    // Update acceleration using the half-step velocity
    ComputeAcceleration();
    // Compute the fullstep velocity from the acceleration computed using the
    // midstep velocity
    _v1 += 0.5 * _a1 * _dt;
    _v2 += 0.5 * _a2 * _dt;
}

// Compute x, y position of pendulums
void DoublePendulum::ComputePosition() {
    _x1 = _l1 * sin(_th1);
    _y1 = -_l1 * cos(_th1);
    _x2 = _x1 + _l2 * sin(_th2);
    _y2 = _y1 - _l2 * cos(_th2);
}

// Compute kinetic and potential energy of double pendulum system
void DoublePendulum::ComputeEnergy() {
    double ke1 = 0.5 * _m1 * SQR(_l1) * SQR(_v1);
    double ke2 = 0.5 * _m2 * (SQR(_l1) * SQR(_v1) + SQR(_l2) * SQR(_v2)
            + 2 * _l1 * _l2 * _v1 * _v2 * cos(_th1 - _th2));
    _kinetic = ke1 + ke2;
    double pe1 = _m1 * G * _l1 * (1 - cos(_th1));
    double pe2 = _m2 * G * (_l1 * (1 - cos(_th1)) + _l2 * (1 - cos(_th2)));
    _potential = pe1 + pe2;
}

// Write the pendulum position and angles to an array.
void DoublePendulum::RecordData() {
    // Compute the pendulum positions and energies only when necessary, since
    // the equations of motion do not depend on them.
    ComputePosition();
    ComputeEnergy();
    // Write the data
    _data[_idx][0] = _time;
    _data[_idx][1] = _x1;
    _data[_idx][2] = _y1;
    _data[_idx][3] = _th1;
    _data[_idx][4] = _x2;
    _data[_idx][5] = _y2;
    _data[_idx][6] = _th2;
    _data[_idx][7] = _kinetic;
    _data[_idx][8] = _potential;
    // Update the index tracking variable
    _idx++;
}

// Initialize output data array given the number of steps that will be written.
void DoublePendulum::InitRecord(int n_steps, int n_record) {
    // Compute number of steps that will be written
    size_t n_data = n_steps / n_record + 1;
    // TODO test that this is necessary?
    if (n_steps % n_record > 0) {
        n_data++;
    }
    // Initialize array
    _data.resize(n_data);
    // 9 data columns: time, x1, y1, th1, x2, y2, th2, ke, pe
    for (size_t i = 0; i < n_data; ++i) {
        _data[i].resize(9);
    }
    // Initialize variable for tracking index to write next.
    _idx = 0;
}

// Get a description of the DoublePendulum instance
const std::string DoublePendulum::GetDescription() const {
    std::ostringstream oss;
    oss << "DoublePendulum with masses " << _m1 << ", " << _m2 << ", lengths "
        << _l1 << ", " << _l2 << ", and initial angles " << _th1_init << ", " 
        << _th2_init << " radians";
    return oss.str();
}
