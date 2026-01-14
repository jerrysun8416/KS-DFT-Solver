# Kohn-Sham DFT Solver

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++](https://img.shields.io/badge/C++-11-blue.svg)](https://isocpp.org/)

A self-consistent Kohn-Sham Density Functional Theory (DFT) solver for atomic systems, implementing the Local Density Approximation (LDA) with Perdew-Zunger exchange-correlation functionals.

## Features

- **Self-consistent Field (SCF) Calculation**: Iterative solution of Kohn-Sham equations until convergence
- **Numerov Method**: High-accuracy numerical integration on logarithmic grid for radial Schrödinger equations
- **LDA Exchange-Correlation**: Perdew-Zunger parametrization of Ceperley-Alder QMC data
- **Hartree Potential**: Self-consistent solution via Newton's method on Poisson equation
- **Atomic Systems**: Supports elements from H to Ca (Z=1 to Z=20)
- **Logarithmic Grid**: Efficient sampling near nucleus with exponential grid spacing
- **Accurate Node Counting**: Bisection method with proper quantum number validation

## Physical Methods

### Kohn-Sham Equations
The solver implements the radial Kohn-Sham equations in atomic units:

```
[-1/2 d²/dr² + l(l+1)/(2r²) + V_eff(r)] u_nl(r) = E_nl u_nl(r)
```

where:
- `u_nl(r) = r R_nl(r)` is the radial wavefunction
- `V_eff = V_Hartree + V_xc - Z/r` is the effective Kohn-Sham potential
- `V_xc` uses LDA with Perdew-Zunger parametrization
- Boundary conditions: `u_nl(0) = 0`, `u_nl(∞) = 0`

### Numerical Methods
- **Numerov Algorithm**: 4th-order accurate finite difference method on logarithmic grid
- **Bisection Method**: Energy eigenvalue search with node counting for proper quantum states
- **Simpson's Rule**: Numerical integration for charge densities and total energies
- **Newton's Method**: Iterative solution of Poisson equation for Hartree potential
- **Density Mixing**: 50% linear mixing for stable SCF convergence

### Total Energy Calculation
```
E_tot = Σ_nl N_nl ε_nl - E_Hartree + E_xc - ∫ n(r) V_xc(r) dr
```

## Requirements

### Dependencies
- **C++11** or later compiler (g++, clang++)
- **[Eigen3](https://eigen.tuxfamily.org/)** - Linear algebra library (v3.3+)
- Standard C++ libraries

### System Requirements
- Linux/Unix/MacOS
- 4GB RAM minimum (8GB recommended for heavier atoms)
- ~100MB disk space

## Installation

### 1. Clone the Repository
```bash
git clone https://github.com/yourusername/KS-DFT-Solver.git
cd KS-DFT-Solver
```

### 2. Install Eigen3

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install libeigen3-dev
```

**MacOS (Homebrew):**
```bash
brew install eigen
```

**Arch Linux:**
```bash
sudo pacman -S eigen
```

**Fedora/RHEL:**
```bash
sudo dnf install eigen3-devel
```

### 3. Compile the Solver

**Standard compilation:**
```bash
g++ -O2 -I./include src/KS_solver.cpp -o KS_solver
```

**With explicit Eigen path (if needed):**
```bash
g++ -O2 -I./include -I/usr/include/eigen3 src/KS_solver.cpp -o KS_solver
```

**For maximum optimization:**
```bash
g++ -O3 -march=native -I./include src/KS_solver.cpp -o KS_solver
```

## Usage

### Basic Execution
```bash
./KS_solver
```

The program will prompt you to enter an atom name:
```
Enter atom name (H ~ Ca): Li
Selected atom: Li		Ntot = 3
```

### Example Session
```
Enter atom name (H ~ Ca): C
Selected atom: C		Ntot = 6
------Starting Main Loop Iteration = 0
[✔] Done: Schrodinger converged via Bisection Numerov!
iter = 18	E = -1.13254e+01	n = 1	l = 0	...
------Done: Main Loop Iteration = 1	Total Energy = -3.75426e+01
...
Converged! Writing wavefunction unl ...
All job done! Final atomic config:
	n = 1	l = 0	Occupancy = 2	Enl = -1.13254e+01
	n = 2	l = 0	Occupancy = 2	Enl = -5.73421e-01
	n = 2	l = 1	Occupancy = 2	Enl = -3.45123e-01
```

### Output Files
The solver generates wavefunction files:
- Format: `{atom}_n{n}l{l}_nodes{nodes}.dat`
- Example: `C_n1l0_nodes0.dat`, `C_n2l0_nodes1.dat`
- Two-column format: `r` (Bohr) and `u_nl(r)`

### Visualizing Results

**Python example:**
```python
import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('C_n1l0_nodes0.dat')
r, u = data[:, 0], data[:, 1]

plt.plot(r, u)
plt.xlabel('r (Bohr)')
plt.ylabel('u_nl(r)')
plt.title('Carbon 1s Wavefunction')
plt.grid(True)
plt.show()
```

## Configuration Parameters

Key adjustable parameters in `src/KS_solver.cpp`:

```cpp
// Grid parameters
const double rmin = 1E-12;           // Minimum radius
const double rmax = 30.0;            // Maximum radius
const int Nx = 20000;                // Number of grid points

// Convergence parameters
const int Iter_max = 100;            // Maximum SCF iterations
const double E_converge = 1E-5;      // Energy convergence threshold
```

## Supported Atoms

All atoms from H (Z=1) to Ca (Z=20):

| Period 1 | Period 2 | Period 3 | Period 4 |
|----------|----------|----------|----------|
| H, He | Li, Be, B, C, N, O, F, Ne | Na, Mg, Al, Si, P, S, Cl, Ar | K, Ca |

## Algorithm Overview

1. **Initialize**: Start with trial electron density
2. **Hartree Potential**: Solve Poisson equation
3. **XC Potential**: Calculate from LDA functional
4. **Solve KS Equations**: Find eigenvalues/eigenfunctions
5. **Update Density**: Construct new density from wavefunctions
6. **Mix Densities**: Stabilize convergence
7. **Check Convergence**: Repeat until converged

## Troubleshooting

### Compilation errors with Eigen
```bash
g++ -O2 -I./include -I/usr/include/eigen3 src/KS_solver.cpp -o KS_solver
```

### "Schrodinger did not converge"
Adjust `E_start` in main() or increase `rmax` and `Nx`

### Oscillating total energy
Increase density mixing (change 0.5 to 0.3 in line 566)

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Author

Yuewen Sun (sunyw@shanghaitech.edu.cn)

---

**Note**: Educational/research code. For production, use ABINIT, Quantum ESPRESSO, or GPAW.
