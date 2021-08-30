"""Module for functions and classes related to double pendulum simulation."""
import _double_pendulum
import numpy as np


class DoublePendulum:
    """Class for simulating a double pendulum."""

    def __init__(
        self,
        m1: float = 1.0,
        m2: float = 1.0,
        l1: float = 1.0,
        l2: float = 1.0,
        th1: float = 0.6,
        th2: float = 0.25,
    ):
        """Class for simulating a double pendulum.

        Initialize pendulum parameters then use the Simulation() method to simulate.

        Methods:
            Simulate: Run a double pendulum simulation.

        Args:
           m1: Mass of pendulum 1
           m2: Mass of pendulum 2
           l1: Length of pendulum 1
           l2: Length of pendulum 2
           th1: Initial angle of pendulum 1 relative to resting
           th2: Initial angle of pendulum 2 relative to resting
        """
        self.pendulum = _double_pendulum.DoublePendulum(m1, m2, l1, l2, th1, th2)

    def Simulate(self, n_steps: int, dt: float = 0.001, n_record: int = 100):
        """Run a simulation of a double pendulum.

        Args:
            n_steps: Number of integration steps to perform
            dt: Time delta to use in integration scheme
            n_record: Frequency at which to record pendulum data for analysis
        """
        self.pendulum.Simulate(n_steps, dt, n_record)

    def Data(self):
        """Return double pendulum simulation data.

        Returns:
            Array of N_data_points x 7, with entries corresponding to:
                time, x1, y1, theta1, x2, y2, theta2
        """
        return self.pendulum.Data()

    def SetMasses(self, m1: float, m2: float):
        """Set pendulum masses."""
        self.pendulum.SetMasses(m1, m2)

    def SetLengths(self, l1, l2):
        """Set pendulum lengths."""
        self.pendulum.SetLengths(l1, l2)

    def SetAngles(self, th1, th2):
        """Set pendulum initial angles."""
        self.pendulum.SetAngles(th1, th2)
