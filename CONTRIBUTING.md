# Contributing to KS-DFT-Solver

Thank you for your interest in contributing! This document provides guidelines for contributing to the project.

## Ways to Contribute

### 1. Bug Reports
If you find a bug, please open an issue with:
- Description of the bug
- Steps to reproduce
- Expected vs actual behavior
- System information (OS, compiler version, Eigen version)
- Relevant output or error messages

### 2. Feature Requests
Suggestions for improvements are welcome! Please include:
- Clear description of the feature
- Use case and motivation
- Proposed implementation (if applicable)

### 3. Code Contributions

#### Areas for Improvement
- **Spin-polarized DFT**: Extend to open-shell atoms
- **Advanced functionals**: Implement GGA or hybrid functionals
- **Parallelization**: Add OpenMP/MPI support for orbital solving
- **Excited states**: TDDFT or ΔSCF methods
- **Performance**: Optimize grid operations and integrals
- **Testing**: Add unit tests and validation suite
- **Documentation**: Improve comments and documentation

## Development Setup

### 1. Fork and Clone
```bash
git clone https://github.com/yourusername/KS-DFT-Solver.git
cd KS-DFT-Solver
```

### 2. Create a Branch
```bash
git checkout -b feature/your-feature-name
```

### 3. Make Changes
- Follow existing code style
- Add comments for complex sections
- Test your changes thoroughly

### 4. Commit
```bash
git add .
git commit -m "Brief description of changes"
```

### 5. Push and Create Pull Request
```bash
git push origin feature/your-feature-name
```

## Code Style Guidelines

### General Principles
- Use clear, descriptive variable names
- Add comments for non-obvious logic
- Keep functions focused on single tasks
- Use const correctness where applicable

### Naming Conventions
- **Functions**: `Snake_Case` or `CamelCase` (be consistent)
- **Variables**: `snake_case` or `camelCase`
- **Constants**: `UPPER_CASE` or `PascalCase`
- **Classes/Structs**: `PascalCase`

### Example
```cpp
// Good: Clear function name and structure
double Calculate_Exchange_Energy(const std::vector<double>& density,
                                 const std::vector<double>& r_grid) {
    double E_x = 0.0;
    // Implementation
    return E_x;
}

// Good: Descriptive variable names
const double convergence_threshold = 1E-6;
int iteration_count = 0;
```

## Testing

### Before Submitting
1. **Compile without warnings**: `g++ -Wall -Wextra ...`
2. **Test with multiple atoms**: Try H, He, Li, C, Ne, Ar
3. **Verify convergence**: Check that SCF converges properly
4. **Validate results**: Compare against known values when possible

### Example Test Atoms
- **H**: Should give E ≈ -0.5 Hartree
- **He**: Should give E ≈ -2.9 Hartree
- **Li**: Should give E ≈ -7.5 Hartree
- **C**: Should give E ≈ -37.8 Hartree

## Pull Request Process

### 1. Update Documentation
- Update README.md if adding features
- Add code comments for new functions
- Include usage examples if applicable

### 2. Provide Clear Description
In your PR, include:
- What changes were made
- Why the changes are needed
- How to test the changes
- Any breaking changes

### 3. Respond to Feedback
- Be open to suggestions
- Make requested changes promptly
- Ask questions if unclear

## Questions?

Feel free to:
- Open an issue for questions
- Start a discussion for general topics
- Contact the maintainer directly

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

Thank you for contributing to KS-DFT-Solver!
