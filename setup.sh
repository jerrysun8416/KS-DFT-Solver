#!/bin/bash
# Quick setup script for KS-DFT-Solver

echo "========================================="
echo "KS-DFT-Solver Setup Script"
echo "========================================="
echo ""

# Check for g++
if ! command -v g++ &> /dev/null; then
    echo "❌ Error: g++ compiler not found"
    echo "Please install g++ before continuing"
    exit 1
fi
echo "✓ g++ compiler found: $(g++ --version | head -n1)"

# Check for Eigen3
EIGEN_PATHS=(
    "/usr/include/eigen3"
    "/usr/local/include/eigen3"
    "/opt/homebrew/include/eigen3"
)

EIGEN_FOUND=false
for path in "${EIGEN_PATHS[@]}"; do
    if [ -d "$path" ]; then
        echo "✓ Eigen3 found at: $path"
        EIGEN_FOUND=true
        EIGEN_PATH=$path
        break
    fi
done

if ! $EIGEN_FOUND; then
    echo "⚠ Warning: Eigen3 not found in standard locations"
    echo "Please install Eigen3:"
    echo "  Ubuntu/Debian: sudo apt-get install libeigen3-dev"
    echo "  MacOS: brew install eigen"
    echo "  Arch: sudo pacman -S eigen"
    echo ""
    read -p "Continue anyway? (y/n) " -n 1 -r
    echo ""
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        exit 1
    fi
fi

# Compile the solver
echo ""
echo "========================================="
echo "Compiling KS-DFT-Solver..."
echo "========================================="

if [ -n "$EIGEN_PATH" ]; then
    g++ -O2 -std=c++11 -I./include -I"$EIGEN_PATH" src/KS_solver.cpp -o KS_solver
else
    g++ -O2 -std=c++11 -I./include src/KS_solver.cpp -o KS_solver
fi

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ Compilation successful!"
    echo ""
    echo "========================================="
    echo "Setup Complete!"
    echo "========================================="
    echo ""
    echo "To run the solver:"
    echo "  ./KS_solver"
    echo ""
    echo "For help:"
    echo "  make help"
    echo ""
    echo "To visualize results (requires Python):"
    echo "  python3 examples/visualize.py <atom_name>"
    echo ""
else
    echo ""
    echo "❌ Compilation failed"
    echo "Please check error messages above"
    exit 1
fi
