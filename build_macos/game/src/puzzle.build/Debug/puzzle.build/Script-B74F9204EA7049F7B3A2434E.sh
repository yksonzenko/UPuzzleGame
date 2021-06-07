#!/bin/sh
make -C /Users/losik/Downloads/UcodePuzzle/build/game/src -f /Users/losik/Downloads/UcodePuzzle/build/game/src/CMakeScripts/puzzle_postBuildPhase.make$CONFIGURATION OBJDIR=$(basename "$OBJECT_FILE_DIR_normal") all
