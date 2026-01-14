# Examples

This directory contains example scripts and tutorials for using the KS-DFT-Solver.

## Visualization Script

### visualize.py

Python script to visualize wavefunction outputs from the solver.

**Requirements:**
```bash
pip install numpy matplotlib
```

**Usage:**

1. **Plot all wavefunctions for an atom:**
```bash
python3 visualize.py C
```

2. **Plot a specific wavefunction:**
```bash
python3 visualize.py C single C_n1l0_nodes0.dat
```

3. **Plot charge density:**
```bash
python3 visualize.py C density
```

**Output:**
- PNG images saved in current directory
- High-resolution (300 DPI) plots ready for publication

## Example Calculations

### Hydrogen Atom
```bash
./KS_solver
# Enter: H
# Expected E_tot ≈ -0.5 Hartree
```

### Carbon Atom
```bash
./KS_solver
# Enter: C
# Expected E_tot ≈ -37.8 Hartree
# Produces: C_n1l0_nodes0.dat, C_n2l0_nodes1.dat, C_n2l1_nodes0.dat
```

### Neon Atom (Closed Shell)
```bash
./KS_solver
# Enter: Ne
# Expected E_tot ≈ -128.5 Hartree
```

## Gnuplot Examples

If you prefer gnuplot for quick visualization:

**Single wavefunction:**
```bash
gnuplot -e "set xlabel 'r (Bohr)'; set ylabel 'u(r)'; plot 'C_n1l0_nodes0.dat' with lines title '1s'"
```

**Multiple wavefunctions:**
```bash
gnuplot -persist <<EOF
set xlabel 'r (Bohr)'
set ylabel 'u_{nl}(r)'
set title 'Carbon Atom Wavefunctions'
plot 'C_n1l0_nodes0.dat' with lines title '1s', \
     'C_n2l0_nodes1.dat' with lines title '2s', \
     'C_n2l1_nodes0.dat' with lines title '2p'
EOF
```

## Data Format

All output `.dat` files have two columns:
1. **Column 1**: Radius r in Bohr (a₀ = 0.529 Å)
2. **Column 2**: Radial wavefunction u_nl(r) where u = r·R(r)

The full wavefunction is:
```
ψ_nlm(r,θ,φ) = R_nl(r) Y_lm(θ,φ) = [u_nl(r)/r] Y_lm(θ,φ)
```

## Converting Units

To convert from atomic units (Hartree) to eV:
```python
E_eV = E_Hartree * 27.211386245988  # eV
```

To convert radius from Bohr to Angstrom:
```python
r_angstrom = r_bohr * 0.529177210903  # Å
```
