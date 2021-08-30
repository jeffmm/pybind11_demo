#include <iostream>
#include <math.h>

class DoublePendulum {
    public:
        DoublePendulum(double m1=1.0, double m2=1.0,
                       double l1=1.0, double l2=1.0,
                       double th1=0.6, double th2=0.25);
        void Simulate(int n_steps, double dt=0.001, int n_record=100);
        void SetAngles(double th1, double th2);
        void SetMasses(double m1, double m2);
        void SetLengths(double l1, double l2);
        float **GetData() const { return _data; }
        size_t GetNData() const { return _n_data; }
    private:
        void UpdatePendulum();
        void ComputeAngles();
        void ComputeAcceleration();
        void ComputeVelocity();
        void ComputePosition();
        void RecordData();
        void InitRecord(int n_steps, int n_record);

        double _dt;
        double _time;
        double _m1, _m2;
        double _l1, _l2;
        double _a1, _a2;
        double _v1, _v2;
        double _x1, _x2;
        double _y1, _y2;
        double _th1, _th2;
        double _th1_init, _th2_init;
        size_t _n_data;
        int _idx;
        float **_data;
};

