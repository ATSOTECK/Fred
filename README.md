Fred
====

Driver station for the SL9 krew.


BUILDING WINDOWS:
You will ned Qt http://download.qt-project.org/official_releases/qt/5.1/5.1.1/qt-windows-opensource-5.1.1-msvc2010-x86-offline.exe
OpenCV http://opencv.org/downloads.html
And Visual Studio http://go.microsoft.com/?linkid=9709949

Install Visual Studio, you don't have to open it or anything just install it.
Then install OpenCV, when it asks where just select the C drive, just the C drive not a folder in it.
Then insall Qt.

If it has worked you should be able to open the projet. Click no on the prompt.
Then just click configure project (if it is there).
Switch to release mode.
After that you should be in the edit more, click projects on the left and disable shadow build.

Now you can build it. It will fail. Copy the dlls from the dlls folder into the newly created 
FredBin folder which should be one directory up.

BUILDING MAC:
Install Qt http://download.qt-project.org/official_releases/qt/5.1/5.1.1/qt-mac-opensource-5.1.1-clang-offline.dmg

Coming up, have to re-work out how to do this. It's been about 9 months since I first built it for mac.

BUILDING LINUX:
Install OpenCV.
Install Qt.

Yay it works! (Why can't windows and mac have package managers and be cool like linux?)
