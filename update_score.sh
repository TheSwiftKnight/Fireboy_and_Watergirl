#!/bin/bash
set -e

# Check if score.txt has been modified
if git diff --quiet HEAD score.txt; then
    echo "No changes in score.txt, skipping update."
    exit 0
fi

# If we're here, score.txt has been modified
echo "Pushing score to GitHub..."
git add score.txt
git commit -m "[Auto]Update score"
git push origin master