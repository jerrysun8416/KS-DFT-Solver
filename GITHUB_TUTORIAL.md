# Step-by-Step Tutorial: Uploading KS-DFT-Solver to GitHub

This tutorial will guide you through uploading your KS-DFT solver to GitHub with a professional layout.

## Prerequisites

Before starting, make sure you have:
- A GitHub account (create one at https://github.com/signup if needed)
- Git installed on your computer
- Command line / terminal access

### Installing Git

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install git
```

**MacOS:**
```bash
brew install git
# Or download from: https://git-scm.com/download/mac
```

**Windows:**
Download from: https://git-scm.com/download/win

### Verify Git Installation
```bash
git --version
# Should output: git version 2.x.x
```

## Step 1: Configure Git (First Time Only)

Set up your identity:
```bash
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

Verify configuration:
```bash
git config --list
```

## Step 2: Create a New Repository on GitHub

1. **Go to GitHub**: https://github.com
2. **Sign in** to your account
3. **Click the "+" icon** in the top-right corner
4. **Select "New repository"**
5. **Fill in the details:**
   - **Repository name**: `KS-DFT-Solver` (or your preferred name)
   - **Description**: "Self-consistent Kohn-Sham DFT solver for atomic systems with LDA"
   - **Visibility**: Choose Public or Private
   - **DO NOT** initialize with README, .gitignore, or license (we already have these)
6. **Click "Create repository"**

## Step 3: Prepare Your Local Repository

Navigate to your project directory:
```bash
cd /path/to/KS-DFT-Solver
```

Initialize git repository:
```bash
git init
```

## Step 4: Add Files to Git

Add all files:
```bash
git add .
```

Check what will be committed:
```bash
git status
```

You should see all your files listed in green.

## Step 5: Make Your First Commit

Create the initial commit:
```bash
git commit -m "Initial commit: KS-DFT solver with LDA implementation"
```

## Step 6: Connect to GitHub Remote

Copy your repository URL from GitHub. It will look like:
- HTTPS: `https://github.com/yourusername/KS-DFT-Solver.git`
- SSH: `git@github.com:yourusername/KS-DFT-Solver.git`

Add the remote repository:
```bash
git remote add origin https://github.com/yourusername/KS-DFT-Solver.git
```

Verify the remote:
```bash
git remote -v
```

## Step 7: Push to GitHub

Push your code to GitHub:
```bash
git branch -M main
git push -u origin main
```

**First time pushing?** You may be asked to authenticate:
- **HTTPS**: Enter your GitHub username and Personal Access Token (not password)
- **SSH**: Requires SSH key setup (see below)

### Setting Up Personal Access Token (PAT)

If using HTTPS, you need a Personal Access Token:

1. Go to GitHub Settings: https://github.com/settings/tokens
2. Click "Generate new token" → "Generate new token (classic)"
3. Give it a name: "KS-DFT-Solver"
4. Select scopes: Check `repo` (full control of private repositories)
5. Click "Generate token"
6. **COPY THE TOKEN** (you won't see it again!)
7. Use this token as your password when pushing

## Step 8: Verify Upload

1. Go to your repository on GitHub
2. Refresh the page
3. You should see all your files:
   ```
   KS-DFT-Solver/
   ├── README.md
   ├── LICENSE
   ├── CONTRIBUTING.md
   ├── CHANGELOG.md
   ├── Makefile
   ├── .gitignore
   ├── src/
   │   └── KS_solver.cpp
   ├── include/
   │   └── atom_database.h
   ├── examples/
   │   ├── README.md
   │   └── visualize.py
   └── data/
   ```

## Step 9: Customize Your Repository

### Add Repository Topics

1. Click the ⚙️ gear icon next to "About" on your repository page
2. Add topics: `dft`, `quantum-chemistry`, `physics`, `computational-physics`, `cpp`, `numerov-method`, `kohn-sham`, `atomic-structure`
3. Click "Save changes"

### Edit Repository Description

Add a short description:
```
Self-consistent Kohn-Sham DFT solver for atomic systems (H-Ca) with LDA exchange-correlation
```

### Add Website (Optional)

If you have documentation or a project page, add it here.

## Step 10: Create Release Tags (Optional)

Tag your first release:
```bash
git tag -a v1.0.0 -m "First stable release"
git push origin v1.0.0
```

On GitHub:
1. Go to "Releases" tab
2. Click "Draft a new release"
3. Select tag `v1.0.0`
4. Title: "v1.0.0 - Initial Release"
5. Description: Summarize features
6. Click "Publish release"

## Common Git Commands for Future Updates

### Making Changes

1. **Check status:**
```bash
git status
```

2. **Add changes:**
```bash
git add filename.cpp          # Add specific file
git add .                     # Add all changed files
```

3. **Commit changes:**
```bash
git commit -m "Description of changes"
```

4. **Push to GitHub:**
```bash
git push
```

### Creating Branches

For new features:
```bash
git checkout -b feature/new-feature
# Make changes
git add .
git commit -m "Add new feature"
git push -u origin feature/new-feature
```

Then create a Pull Request on GitHub.

### Updating from GitHub

If collaborating or working on multiple machines:
```bash
git pull origin main
```

### Viewing History

```bash
git log                       # View commit history
git log --oneline --graph     # Compact view with graph
```

## Setting Up SSH (Alternative to HTTPS)

### Generate SSH Key

```bash
ssh-keygen -t ed25519 -C "your.email@example.com"
# Press Enter to accept default location
# Enter passphrase (optional but recommended)
```

### Add SSH Key to GitHub

1. Copy your public key:
```bash
cat ~/.ssh/id_ed25519.pub
# Copy the entire output
```

2. Go to GitHub Settings → SSH and GPG keys
3. Click "New SSH key"
4. Paste your key and give it a title
5. Click "Add SSH key"

### Test SSH Connection

```bash
ssh -T git@github.com
# Should output: Hi username! You've successfully authenticated...
```

### Change Remote to SSH

```bash
git remote set-url origin git@github.com:yourusername/KS-DFT-Solver.git
```

## Troubleshooting

### Problem: "Permission denied (publickey)"
**Solution**: Set up SSH keys (see above) or use HTTPS with PAT

### Problem: "Repository not found"
**Solution**: Check repository name and URL are correct

### Problem: "Failed to push some refs"
**Solution**: Pull changes first:
```bash
git pull origin main --rebase
git push
```

### Problem: Large files won't upload
**Solution**: GitHub has a 100MB file size limit. Use Git LFS for large files:
```bash
git lfs install
git lfs track "*.dat"
git add .gitattributes
git commit -m "Add Git LFS"
```

## Best Practices

1. **Commit often**: Make small, logical commits
2. **Write clear messages**: Describe what and why, not how
3. **Use branches**: Don't work directly on main for big changes
4. **Pull before push**: Always sync with remote before pushing
5. **Review before commit**: Check `git diff` before committing
6. **Use .gitignore**: Don't commit build artifacts or data files

## Example Workflow

```bash
# Start work day
git pull origin main

# Create feature branch
git checkout -b feature/add-gga-functional

# Make changes, test
vim src/KS_solver.cpp

# Check what changed
git diff

# Stage and commit
git add src/KS_solver.cpp
git commit -m "Add GGA exchange-correlation functional

- Implement PBE functional
- Add gradient calculations
- Update documentation"

# Push feature branch
git push -u origin feature/add-gga-functional

# Create Pull Request on GitHub
# After review and merge, clean up
git checkout main
git pull origin main
git branch -d feature/add-gga-functional
```

## Resources

- **Git Documentation**: https://git-scm.com/doc
- **GitHub Guides**: https://guides.github.com/
- **Pro Git Book**: https://git-scm.com/book/en/v2
- **Git Cheat Sheet**: https://education.github.com/git-cheat-sheet-education.pdf

## Congratulations!

Your KS-DFT-Solver is now on GitHub with a professional layout! 🎉

You now have:
- ✅ Version control for your code
- ✅ Public portfolio piece
- ✅ Collaboration platform
- ✅ Professional project structure
- ✅ Documentation and examples

Share your repository link with colleagues, on your CV, or in publications!
