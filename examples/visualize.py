#!/usr/bin/env python3
"""
Visualization script for KS-DFT-Solver output
Plots radial wavefunctions and charge densities
"""

import numpy as np
import matplotlib.pyplot as plt
import glob
import sys
import os

def plot_wavefunction(filename):
    """Plot a single wavefunction"""
    if not os.path.exists(filename):
        print(f"Error: File {filename} not found")
        return
    
    data = np.loadtxt(filename)
    r = data[:, 0]
    u = data[:, 1]
    
    # Extract quantum numbers from filename
    # Format: atom_n{n}l{l}_nodes{nodes}.dat
    parts = os.path.basename(filename).split('_')
    atom = parts[0]
    n = parts[1][1]
    l = parts[1][3]
    
    l_labels = ['s', 'p', 'd', 'f', 'g']
    orbital_label = f"{n}{l_labels[int(l)]}"
    
    plt.figure(figsize=(10, 6))
    plt.plot(r, u, linewidth=2, label=f'{atom} {orbital_label}')
    plt.xlabel('r (Bohr)', fontsize=12)
    plt.ylabel('u$_{nl}$(r)', fontsize=12)
    plt.title(f'{atom} Atom - {orbital_label} Orbital Wavefunction', fontsize=14)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=11)
    plt.xlim(0, min(15, r[-1]))  # Focus on relevant region
    plt.tight_layout()
    plt.savefig(f'{atom}_{orbital_label}_wavefunction.png', dpi=300)
    print(f"Saved: {atom}_{orbital_label}_wavefunction.png")
    plt.show()

def plot_all_wavefunctions(atom_name):
    """Plot all wavefunctions for a given atom"""
    pattern = f"{atom_name}_n*l*.dat"
    files = sorted(glob.glob(pattern))
    
    if not files:
        print(f"No wavefunction files found for atom {atom_name}")
        return
    
    print(f"Found {len(files)} wavefunction files for {atom_name}")
    
    fig, axes = plt.subplots(len(files), 1, figsize=(10, 4*len(files)))
    if len(files) == 1:
        axes = [axes]
    
    l_labels = ['s', 'p', 'd', 'f', 'g']
    
    for idx, filename in enumerate(files):
        data = np.loadtxt(filename)
        r = data[:, 0]
        u = data[:, 1]
        
        # Extract quantum numbers
        parts = os.path.basename(filename).split('_')
        n = parts[1][1]
        l = parts[1][3]
        orbital_label = f"{n}{l_labels[int(l)]}"
        
        axes[idx].plot(r, u, linewidth=2, color=f'C{idx}')
        axes[idx].set_xlabel('r (Bohr)', fontsize=11)
        axes[idx].set_ylabel('u$_{nl}$(r)', fontsize=11)
        axes[idx].set_title(f'{orbital_label} orbital', fontsize=12)
        axes[idx].grid(True, alpha=0.3)
        axes[idx].set_xlim(0, min(15, r[-1]))
    
    plt.suptitle(f'{atom_name} Atom - All Orbital Wavefunctions', fontsize=14, y=0.995)
    plt.tight_layout()
    plt.savefig(f'{atom_name}_all_wavefunctions.png', dpi=300)
    print(f"Saved: {atom_name}_all_wavefunctions.png")
    plt.show()

def plot_radial_density(atom_name):
    """Plot radial charge density from wavefunctions"""
    pattern = f"{atom_name}_n*l*.dat"
    files = sorted(glob.glob(pattern))
    
    if not files:
        print(f"No wavefunction files found for atom {atom_name}")
        return
    
    # Read first file to get r grid
    data = np.loadtxt(files[0])
    r = data[:, 0]
    density = np.zeros_like(r)
    
    l_labels = ['s', 'p', 'd', 'f', 'g']
    
    plt.figure(figsize=(10, 6))
    
    # Plot individual orbital contributions
    for filename in files:
        data = np.loadtxt(filename)
        u = data[:, 1]
        
        # Extract info
        parts = os.path.basename(filename).split('_')
        n = parts[1][1]
        l = parts[1][3]
        
        # Occupancy (simplified - assumes closed shell)
        l_val = int(l)
        occupancy = 2 * (2 * l_val + 1) if l_val > 0 else 2
        
        orbital_density = occupancy * u**2 / (4 * np.pi * r**2)
        density += orbital_density
        
        orbital_label = f"{n}{l_labels[l_val]}"
        plt.plot(r, orbital_density, label=f'{orbital_label}', alpha=0.7, linewidth=1.5)
    
    # Plot total density
    plt.plot(r, density, 'k-', linewidth=2.5, label='Total', linestyle='--')
    
    plt.xlabel('r (Bohr)', fontsize=12)
    plt.ylabel('n(r) (Bohr$^{-3}$)', fontsize=12)
    plt.title(f'{atom_name} Atom - Radial Charge Density', fontsize=14)
    plt.grid(True, alpha=0.3)
    plt.legend(fontsize=10)
    plt.xlim(0, min(10, r[-1]))
    plt.yscale('log')
    plt.tight_layout()
    plt.savefig(f'{atom_name}_charge_density.png', dpi=300)
    print(f"Saved: {atom_name}_charge_density.png")
    plt.show()

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 visualize.py <atom_name> [mode]")
        print("Modes: 'all' (default), 'single <filename>', 'density'")
        print("\nExamples:")
        print("  python3 visualize.py C              # Plot all C wavefunctions")
        print("  python3 visualize.py C single C_n1l0_nodes0.dat  # Plot specific file")
        print("  python3 visualize.py C density       # Plot charge density")
        sys.exit(1)
    
    atom_name = sys.argv[1]
    mode = sys.argv[2] if len(sys.argv) > 2 else 'all'
    
    if mode == 'single' and len(sys.argv) > 3:
        plot_wavefunction(sys.argv[3])
    elif mode == 'density':
        plot_radial_density(atom_name)
    else:
        plot_all_wavefunctions(atom_name)

if __name__ == "__main__":
    main()
