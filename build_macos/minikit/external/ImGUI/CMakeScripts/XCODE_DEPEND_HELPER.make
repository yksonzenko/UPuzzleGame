# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.ImGui.Debug:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/ImGUI/Debug/libImGui.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/ImGUI/Debug/libImGui.a


PostBuild.ImGui.Release:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/ImGUI/Release/libImGui.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/ImGUI/Release/libImGui.a




# For each target create a dummy ruleso the target does not have to exist
