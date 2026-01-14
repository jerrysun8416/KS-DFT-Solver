# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-01-14

### Added
- Initial release of KS-DFT-Solver
- Self-consistent Kohn-Sham DFT for atoms H through Ca
- Numerov method on logarithmic grid for Schrödinger equation
- LDA exchange-correlation with Perdew-Zunger parametrization
- Newton's method for Hartree potential (Poisson equation)
- Bisection method with node counting for eigenvalue search
- Density mixing (50%) for SCF stability
- Wavefunction output to .dat files
- Support for s, p, d orbitals
- Comprehensive README with installation and usage instructions
- Makefile for easy compilation
- Python visualization script for wavefunction plotting
- MIT License

### Known Issues
- Nitrogen atom may have convergence difficulties
- Limited to closed-shell atoms only (no spin polarization)
- Heavy atoms beyond Ca may require parameter tuning

## [0.10.0] - Development

### Added
- Logarithmic grid implementation (finally working!)
- Correct Numerov formula on non-uniform grid with proper Jacobian
- Applied to both Hartree potential and orbital solving

### Fixed
- Hartree potential accuracy on logarithmic grid
- Wavefunction near r=0 accuracy improved

### Notes
- Tested successfully for H through Ca
- Future plans: Exchange-correlation beyond LDA, MPI parallelization

## [0.9.0] - Development

### Added
- Half density update to avoid oscillation
- Newton's method required for Hartree solution

### Fixed
- Oscillation issues in SCF convergence

### Known Issues
- B atom accuracy needs improvement
- N atom not solvable, periodic energy difference

## [0.8.0] - Development

### Changed
- Improved precision with alternative grid schemes
- Currently works well for H through Li

### Notes
- Hartree: Newton's method
- Schrödinger: Avoiding oscillation

## [0.7.0] - Development

### Attempted
- Logarithmic grid with updated Numerov formula - FAILED
- Issue: y' and y''' terms cannot be ignored on non-uniform grid
- Lead to inaccuracy near r = 0

## [0.6.0] - Development

### Added
- Cleaned up unnecessary algorithms
- Kept only Passive Refinement Numerov for both Hartree and Schrödinger
- Error code checking in main loop

### Improved
- Code organization and readability

## [0.5.0] - Development

### Fixed
- Major bug in U_Hartree formula (incorrect calculation)
- Purged convergence loop for 1S, 2S orbitals

### Added
- Separate solvers for H, He, Li, Be

## Earlier Versions

Development versions focusing on:
- Basic Numerov implementation
- Hartree potential solver
- Exchange-correlation functionals
- Grid optimization studies
