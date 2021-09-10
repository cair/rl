from cython.cimports.sin_of_square import Function
import cython


@cython.cclass
class Wumpus:

    # Not available in Python-space:
    offset: float

    # Available in Python-space:
    freq = cython.declare(cython.double, visibility='public')

    # Available in Python-space, but only for reading:
    scale = cython.declare(cython.double, visibility='readonly')

    # Available in Python-space:


    @cython.ccall
    def step(self, a: int) -> float:
        return 0.0

    @property
    def period(self):
        return 1.0 / self.freq

    @period.setter
    def period(self, value):
        self.freq = 1.0 / value




