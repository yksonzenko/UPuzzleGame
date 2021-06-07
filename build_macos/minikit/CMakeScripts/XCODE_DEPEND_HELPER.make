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


PostBuild.MiniKit.Framework.Debug:
/Users/losik/Downloads/UcodePuzzle/build/minikit/framework/Debug/libMiniKit.Framework.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/framework/Debug/libMiniKit.Framework.a


PostBuild.SDL2-static.Debug:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Debug/libSDL2d.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Debug/libSDL2d.a


PostBuild.SDL2main.Debug:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Debug/libSDL2maind.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Debug/libSDL2maind.a


PostBuild.ImGui.Release:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/ImGUI/Release/libImGui.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/ImGUI/Release/libImGui.a


PostBuild.MiniKit.Framework.Release:
/Users/losik/Downloads/UcodePuzzle/build/minikit/framework/Release/libMiniKit.Framework.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/framework/Release/libMiniKit.Framework.a


PostBuild.SDL2-static.Release:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Release/libSDL2.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Release/libSDL2.a


PostBuild.SDL2main.Release:
/Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Release/libSDL2main.a:
	/bin/rm -f /Users/losik/Downloads/UcodePuzzle/build/minikit/external/SDL2/Release/libSDL2main.a




# For each target create a dummy ruleso the target does not have to exist
