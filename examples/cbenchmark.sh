#!/bin/bash
# see https://github.com/aras-p/ClangBuildAnalyzer
# Example:
#	ClangBuildAnalyzerLoc=~/prog/ClangBuildAnalyzer/build ArtifactLoc=build_clang examples/cbenchmark.sh
# Where:
#	ClangBuildAnalyzerLoc is the directory where ClangBuildAnalyzer can be found
# and
#	ArtifactLoc is the directory where conjure_enum is built

$ClangBuildAnalyzerLoc/ClangBuildAnalyzer --all $ArtifactLoc/CMakeFiles/cbenchmark.dir cbenchmark.dat; $ClangBuildAnalyzerLoc/ClangBuildAnalyzer --analyze cbenchmark.dat
