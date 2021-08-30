"""Module for functions and classes related to double pendulum simulation."""
import _double_pendulum
import pandas as pd


class DoublePendulum:
    """Class for simulating a double pendulum."""

    _pendulum = None
    data = None

    def __init__(
        self,
        m1: float = 1.0,
        m2: float = 1.0,
        l1: float = 1.0,
        l2: float = 1.0,
        th1: float = 30.0,
        th2: float = 45.0,
    ):
        """Class for simulating a double pendulum.

        Initialize pendulum parameters then use the Simulation() method to simulate.

        Methods:
            Simulate: Run a double pendulum simulation.
            SetMasses: Set pendulum masses.
            SetLengths: Set pendulum lengths.
            SetAngles: Set initial pendulum angles in degrees.

        Attributes:
            data: Array of shape (N, 9), with entries corresponding to:
                time, x1, y1, theta1, x2, y2, theta2, kinetic, potential.
                Created after simulation is complete.

        Args:
           m1: Mass of pendulum 1
           m2: Mass of pendulum 2
           l1: Length of pendulum 1
           l2: Length of pendulum 2
           th1: Initial angle of pendulum 1 relative to resting, in degrees
           th2: Initial angle of pendulum 2 relative to resting, in degrees
        """
        self._pendulum = _double_pendulum.DoublePendulum(m1, m2, l1, l2, th1, th2)

    def Simulate(self, n_steps: int, dt: float = 0.0001, n_record: int = 1000):
        """Run a simulation of a double pendulum.

        Args:
            n_steps: Number of integration steps to perform
            dt: Time delta to use in integration scheme
            n_record: Frequency at which to record pendulum data for analysis
        """
        self._pendulum.Simulate(n_steps, dt, n_record)
        self.data = pd.DataFrame(
            self._pendulum.Data(),
            columns=["time", "x1", "y1", "th1", "x2", "y2", "th2", "ke", "pe"],
        )
        self.data = self.data.set_index("time")

    def SetMasses(self, m1: float, m2: float):
        """Set pendulum masses."""
        self._pendulum.SetMasses(m1, m2)

    def SetLengths(self, l1, l2):
        """Set pendulum lengths."""
        self._pendulum.SetLengths(l1, l2)

    def SetAngles(self, th1, th2):
        """Set pendulum initial angles relative to resting, in degrees."""
        self._pendulum.SetAngles(th1, th2)

    def __repr__(self):
        """Return DoublePendulum description representation."""
        return f"<double_pendulum.{self._pendulum.__repr__()}>"

    def __str__(self):
        """Return DoublePendulum description string."""
        return self._pendulum.__repr__()
