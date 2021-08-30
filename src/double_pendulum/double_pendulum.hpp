#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

typedef std::vector<std::vector<float>> matrix;

// Class for initializing pendulum and simulation parameters and running
// pendulum simulation using a velocity Verlet integration algorithm
class DoublePendulum {
    public:
        DoublePendulum(double m1=1.0, double m2=1.0,
                       double l1=1.0, double l2=1.0,
                       double th1=30.0, double th2=45.0);
        void Simulate(int n_steps, double dt=0.0001, int n_record=1000);
        void SetAngles(double th1, double th2);
        void SetMasses(double m1, double m2);
        void SetLengths(double l1, double l2);
        const std::string GetDescription() const;
        const matrix &GetData() const { return _data; }
    private:
        void UpdatePendulum();
        void ComputeAngles();
        void ComputeAcceleration();
        void ComputeVelocity();
        void ComputePosition();
        void ComputeEnergy();
        void RecordData();
        void InitRecord(int n_steps, int n_record);

        double _dt;         // time delta to use for integration step
        float _time;        // current simulation time
        double _m1, _m2;    // pendulum masses
        double _l1, _l2;    // pendulum lengths
        double _a1, _a2;    // pendulum angular acceleration
        double _v1, _v2;    // pendulum angular velocities
        double _x1, _x2;    // pendulum x coordinates
        double _y1, _y2;    // pendulum y coordinates
        double _th1, _th2;  // pendulum angles relative to resting, in radians
        double _th1_init, _th2_init;  // initial pendulum angles, in radians
        double _kinetic, _potential;  // kinetic and potential energies
        size_t _n_data;     // the number of rows of data recorded
        int _idx;           // index-tracking variable for writing data
        matrix _data;      // matrix of data for output and analysis
};
