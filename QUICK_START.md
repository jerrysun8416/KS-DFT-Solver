# Quick Reference Guide: Upload to GitHub

## TL;DR - Fast Track (5 minutes)

```bash
# 1. Navigate to your project
cd KS-DFT-Solver

# 2. Initialize Git
git init
git add .
git commit -m "Initial commit"

# 3. Create repo on GitHub (do this first!)
# Go to https://github.com/new
# Name: KS-DFT-Solver
# Don't initialize with anything
# Click "Create repository"

# 4. Connect and push
git remote add origin https://github.com/YOUR_USERNAME/KS-DFT-Solver.git
git branch -M main
git push -u origin main
```

Done! Your code is now on GitHub.

---

## Complete Step-by-Step Guide

### Step 1: Install Git (if needed)

**Check if installed:**
```bash
git --version
```

**Install:**
- Ubuntu: `sudo apt-get install git`
- MacOS: `brew install git`
- Windows: Download from https://git-scm.com

### Step 2: Configure Git (First time only)

```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

### Step 3: Create GitHub Repository

1. Go to https://github.com and sign in
2. Click '+' → 'New repository'
3. Name: `KS-DFT-Solver`
4. Description: "Self-consistent Kohn-Sham DFT solver with LDA"
5. Public or Private (your choice)
6. **DO NOT** check any initialize options
7. Click 'Create repository'

### Step 4: Upload Your Code

```bash
# Navigate to your project folder
cd /path/to/KS-DFT-Solver

# Initialize Git
git init

# Add all files
git add .

# Create first commit
git commit -m "Initial commit: KS-DFT solver implementation"

# Connect to GitHub (replace YOUR_USERNAME)
git remote add origin https://github.com/YOUR_USERNAME/KS-DFT-Solver.git

# Rename branch to main
git branch -M main

# Push to GitHub
git push -u origin main
```

### Step 5: Authenticate

**If using HTTPS:** You'll need a Personal Access Token (PAT)
1. Go to https://github.com/settings/tokens
2. Click 'Generate new token (classic)'
3. Select 'repo' scope
4. Copy the token
5. Use it as your password when pushing

**If using SSH:** (Recommended for frequent use)
```bash
# Generate key
ssh-keygen -t ed25519 -C "your.email@example.com"

# Copy public key
cat ~/.ssh/id_ed25519.pub

# Add to GitHub: Settings → SSH keys → New SSH key
```

### Step 6: Verify

Go to https://github.com/YOUR_USERNAME/KS-DFT-Solver

You should see all your files!

---

## Project Structure

```
KS-DFT-Solver/
├── README.md              ← Main documentation
├── LICENSE                ← MIT License
├── CONTRIBUTING.md        ← Contribution guidelines
├── CHANGELOG.md           ← Version history
├── Makefile               ← Build automation
├── setup.sh               ← Quick setup script
├── .gitignore             ← Files to ignore
├── GITHUB_TUTORIAL.md     ← Detailed GitHub guide
│
├── src/                   ← Source code
│   └── KS_solver.cpp      ← Main solver
│
├── include/               ← Header files
│   └── atom_database.h    ← Atomic configurations
│
├── examples/              ← Examples and scripts
│   ├── README.md
│   └── visualize.py       ← Plotting script
│
└── data/                  ← Output data (gitignored)
```

---

## Common Commands

### Making Changes
```bash
git status                  # Check what's changed
git add filename.cpp        # Stage specific file
git add .                   # Stage all changes
git commit -m "message"     # Commit changes
git push                    # Upload to GitHub
```

### Viewing History
```bash
git log                     # See commit history
git log --oneline           # Compact view
git diff                    # See unstaged changes
```

### Branches
```bash
git branch                  # List branches
git checkout -b new-feature # Create new branch
git checkout main           # Switch to main
git merge new-feature       # Merge branch
```

### Syncing
```bash
git pull                    # Download changes
git fetch                   # Check for updates
```

---

## Troubleshooting

### "Permission denied"
→ Set up SSH keys or use Personal Access Token

### "Repository not found"
→ Check URL: `git remote -v`

### "Failed to push"
→ Pull first: `git pull origin main --rebase`

### "Conflicts"
→ Edit conflicted files, then:
```bash
git add .
git commit -m "Resolve conflicts"
git push
```

---

## Making Your Repo Professional

### Add Topics
On GitHub: Settings → Topics
Add: `dft`, `quantum-chemistry`, `physics`, `cpp`, `computational-physics`

### Create First Release
```bash
git tag -a v1.0.0 -m "First release"
git push origin v1.0.0
```
Then on GitHub: Releases → Draft new release

### Add GitHub Actions (CI/CD)
Create `.github/workflows/build.yml`:
```yaml
name: Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Install dependencies
      run: sudo apt-get install libeigen3-dev
    - name: Build
      run: make
```

---

## Quick Command Cheat Sheet

```bash
# Setup (once)
git init
git remote add origin <URL>

# Daily workflow
git pull                    # Start of day
git add .                   # Stage changes
git commit -m "message"     # Commit
git push                    # Share changes

# Check status
git status                  # What's changed?
git log --oneline           # History

# Branches
git checkout -b feature     # New branch
git push -u origin feature  # Push branch
git checkout main           # Back to main
```

---

## Next Steps

1. ✅ Upload to GitHub (you're doing this!)
2. Add a nice README badge
3. Write more examples
4. Create releases
5. Enable GitHub Actions
6. Add collaborators
7. Share with community!

---

## Resources

- **Git Basics**: https://git-scm.com/book/en/v2/Getting-Started
- **GitHub Docs**: https://docs.github.com
- **Git Cheat Sheet**: https://education.github.com/git-cheat-sheet-education.pdf
- **Interactive Git**: https://learngitbranching.js.org/

---

**Questions?** Open an issue on GitHub or consult GITHUB_TUTORIAL.md for detailed explanations.
