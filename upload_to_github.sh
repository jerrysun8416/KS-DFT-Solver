#!/bin/bash
# Complete GitHub Upload Script for KS-DFT-Solver
# Save this file and run: bash upload_to_github.sh

echo "╔══════════════════════════════════════════════════════╗"
echo "║   KS-DFT-Solver GitHub Upload Assistant             ║"
echo "╚══════════════════════════════════════════════════════╝"
echo ""

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print status
print_status() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
}

# Step 1: Check prerequisites
echo "Step 1: Checking prerequisites..."
echo "─────────────────────────────────────────────────────"

# Check if git is installed
if ! command -v git &> /dev/null; then
    print_error "Git is not installed"
    echo "Install git first:"
    echo "  Ubuntu/Debian: sudo apt-get install git"
    echo "  MacOS: brew install git"
    exit 1
fi
print_status "Git is installed ($(git --version | cut -d' ' -f3))"

# Check git configuration
if [ -z "$(git config --global user.name)" ] || [ -z "$(git config --global user.email)" ]; then
    print_warning "Git not configured. Let's set it up..."
    echo ""
    read -p "Enter your name: " git_name
    read -p "Enter your email: " git_email
    git config --global user.name "$git_name"
    git config --global user.email "$git_email"
    print_status "Git configured"
else
    print_status "Git configured as: $(git config --global user.name) <$(git config --global user.email)>"
fi

echo ""
echo "Step 2: Repository Information"
echo "─────────────────────────────────────────────────────"

# Ask for repository name
read -p "Enter your GitHub username: " github_username
read -p "Enter repository name [KS-DFT-Solver]: " repo_name
repo_name=${repo_name:-KS-DFT-Solver}

REPO_URL="https://github.com/$github_username/$repo_name.git"

echo ""
print_warning "IMPORTANT: Before continuing, you must:"
echo "1. Go to https://github.com/new"
echo "2. Create a repository named: $repo_name"
echo "3. Do NOT initialize with README, .gitignore, or license"
echo "4. Click 'Create repository'"
echo ""
read -p "Have you created the repository on GitHub? (y/n) " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    print_warning "Please create the repository first, then run this script again"
    exit 0
fi

echo ""
echo "Step 3: Initializing Git repository"
echo "─────────────────────────────────────────────────────"

# Initialize git if not already initialized
if [ ! -d ".git" ]; then
    git init
    print_status "Initialized Git repository"
else
    print_status "Git repository already initialized"
fi

echo ""
echo "Step 4: Adding files"
echo "─────────────────────────────────────────────────────"

# Add all files
git add .
NUM_FILES=$(git diff --cached --numstat | wc -l)
print_status "Added $NUM_FILES files to staging area"

# Show what will be committed
echo ""
echo "Files to be committed:"
git diff --cached --name-status | head -20
if [ $(git diff --cached --name-status | wc -l) -gt 20 ]; then
    echo "... and more"
fi

echo ""
echo "Step 5: Creating initial commit"
echo "─────────────────────────────────────────────────────"

# Commit
git commit -m "Initial commit: Kohn-Sham DFT solver with LDA

- Self-consistent field calculation
- Numerov method on logarithmic grid
- LDA exchange-correlation functional
- Support for atoms H through Ca
- Comprehensive documentation
- Visualization tools"

if [ $? -eq 0 ]; then
    print_status "Created initial commit"
else
    print_error "Failed to create commit"
    exit 1
fi

echo ""
echo "Step 6: Adding remote repository"
echo "─────────────────────────────────────────────────────"

# Check if remote already exists
if git remote | grep -q "origin"; then
    print_warning "Remote 'origin' already exists, removing it"
    git remote remove origin
fi

# Add remote
git remote add origin "$REPO_URL"
print_status "Added remote: $REPO_URL"

echo ""
echo "Step 7: Renaming branch to 'main'"
echo "─────────────────────────────────────────────────────"

# Rename branch to main
git branch -M main
print_status "Branch renamed to 'main'"

echo ""
echo "Step 8: Pushing to GitHub"
echo "─────────────────────────────────────────────────────"

print_warning "You will need to authenticate with GitHub"
echo "Use your Personal Access Token (PAT) as password"
echo ""
echo "To get a PAT:"
echo "1. Go to https://github.com/settings/tokens"
echo "2. Click 'Generate new token (classic)'"
echo "3. Select 'repo' scope"
echo "4. Copy the token and paste it when prompted for password"
echo ""
read -p "Ready to push? (y/n) " -n 1 -r
echo ""

if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    print_warning "Aborted. You can push later with: git push -u origin main"
    exit 0
fi

# Push to GitHub
git push -u origin main

if [ $? -eq 0 ]; then
    echo ""
    echo "╔══════════════════════════════════════════════════════╗"
    echo "║             SUCCESS! 🎉                              ║"
    echo "╚══════════════════════════════════════════════════════╝"
    echo ""
    print_status "Your code is now on GitHub!"
    echo ""
    echo "Repository URL:"
    echo "  https://github.com/$github_username/$repo_name"
    echo ""
    echo "Next steps:"
    echo "  1. Visit your repository and verify files uploaded"
    echo "  2. Add repository topics for discoverability"
    echo "  3. Create a release: git tag -a v1.0.0 -m 'First release'"
    echo "  4. Share your work!"
    echo ""
else
    echo ""
    print_error "Failed to push to GitHub"
    echo ""
    echo "Common issues:"
    echo "  • Wrong username or repository name"
    echo "  • Repository doesn't exist on GitHub"
    echo "  • Authentication failed (need Personal Access Token)"
    echo "  • No internet connection"
    echo ""
    echo "Try manually:"
    echo "  git push -u origin main"
    exit 1
fi
