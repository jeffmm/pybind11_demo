#include "../double_pendulum/double_pendulum.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>

void print_help(const char* name) {
    std::cerr << name << " expects 9 parameters: \n";
    std::cerr << "\t" << name << " " << "m1 m2 l1 l2 theta1 theta2 n_steps dt n_record\n";
    std::cerr << "where:\n";
    std::cerr << "\tm1 and m2 are the pendulum masses\n";
    std::cerr << "\tl1 and l2 are the pendulum lengths\n";
    std::cerr << "\ttheta1 and theta2 are the initial pendulum angles\n";
    std::cerr << "\tn_steps is the number of integration steps\n";
    std::cerr << "\tdt is the time delta in the integrator\n";
    std::cerr << "\tn_record is the rate at which simulation data is recorded\n";
}

int main(int argc, const char** argv) {
    if (argc != 10) {
        print_help(argv[0]);
        return 1;
    }
    double m1 = atof(argv[1]);
    double m2 = atof(argv[2]);
    double l1 = atof(argv[3]);
    double l2 = atof(argv[4]);
    double th1 = atof(argv[5]);
    double th2 = atof(argv[6]);
    int n_steps = atoi(argv[7]);
    double dt = atof(argv[8]);
    int n_record = atoi(argv[9]);

    DoublePendulum pendulum(m1, m2, l1, l2, th1, th2);
    pendulum.Simulate(n_steps, dt, n_record);
    const matrix data = pendulum.GetData();
    size_t N = data.size();
    size_t M = data[0].size();
    std::ofstream fout("double_pendulum.dat");
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < M; ++ j) {
            fout << data[i][j] << " ";
        }
        fout << "\n";
    }
    fout.close();

    return 0;
}
