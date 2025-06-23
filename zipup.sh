#!/bin/bash

# Name of the output archive (you can change this)
ARCHIVE_NAME="JMR800-v2.zip"

# Ensure we're in a git repository
if ! git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
  echo "Error: This directory is not a Git repository."
  exit 1
fi

# Create a zip archive that excludes files from .gitignore
git archive --format=zip --output="$ARCHIVE_NAME" HEAD

echo "Created $ARCHIVE_NAME excluding files in .gitignore"
