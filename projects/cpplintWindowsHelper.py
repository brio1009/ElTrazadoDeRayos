#!/usr/bin/python

import sys
import subprocess
from os import walk

# Define paths etc.
srcPath = '..\\..\\src\\RayTracerLib'

# Get all the files.
def getAllFiles():
  # We walk over the directory because we also want subfiles.
  f = []
  for (dirPath, dirNames, fileNames) in walk(srcPath):
    # We need to walk over every files to add the complete path.
    for fileName in fileNames:
      f.append(dirPath + '\\' + fileName)
  return f
  
# Filter out cpp files.
def getAllSourceFiles():
  files = getAllFiles()
  extensions = ('c', 'cc', 'cpp', 'h', 'hpp')
  return [file for file in files if file.endswith(extensions)]

# Check the given file with the linter.
def lintFile(fileName):
  try:
    subprocess.check_call(['py', '..\\cpplint.py', '--root=src', fileName])
  except subprocess.CalledProcessError as error:
    return False
  return True

# Main.
def main():
  # Get the source files.
  srcFiles = getAllSourceFiles()
  filesToCheck = [file for file in srcFiles if file]

  numBadFiles = 0
  for file in filesToCheck:
    if not lintFile(file):
      numBadFiles += 1
  print '\nBad files: ' + str(numBadFiles) + '\n'
  sys.exit(numBadFiles)

# Execute main.
if __name__ == '__main__':
  main()