#!/usr/bin/python

import sys
import subprocess
import os
from os import walk

# Define paths etc.
# Need feedback if this works on windows.
srcPath = os.path.join('..', '..', 'src', 'RayTracerLib')

# Get all the files.
def getAllFiles():
  # We walk over the directory because we also want subfiles.
  f = []
  for (dirPath, dirNames, fileNames) in walk(srcPath):
    # We need to walk over every files to add the complete path.
    for fileName in fileNames:
      f.append(os.path.join(dirPath, fileName))
  return f
  
# Filter out cpp files.
def getAllSourceFiles():
  files = getAllFiles()
  extensions = ('c', 'cc', 'cpp', 'h', 'hpp')
  return [file for file in files if file.endswith(extensions)]

# Check the given file with the linter.
def lintFile(filesToCheck):
  try:
    subprocess.check_call([sys.executable, os.path.join('..', 'cpplint.py'), '--root=src'] + filesToCheck)
  except subprocess.CalledProcessError as error:
    return False
  return True

# Main.
def main():
  # Get the source files.
  srcFiles = getAllSourceFiles()
  filesToCheck = [file for file in srcFiles if file]
  # Return a error code if something failes.
  if lintFile(filesToCheck):
    sys.exit(0)
  else:
    sys.exit(1)

# Execute main.
if __name__ == '__main__':
  main()
