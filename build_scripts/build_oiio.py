import os
import subprocess
from sys import platform
from pathlib import Path

########## oiio ##########
print_msg("Building openimageio...")


if platform == "linux":
	#TODO: x64-linux-dynamic's boost has relocation errors. Investigate
	triplet_dir = os.path.abspath(__file__)+"/triplets"
	subprocess.run([vcpkg_root +"/vcpkg","install","openimageio",f"--overlay-triplets=\"{triplet_dir}\""],check=True)
	cmake_args.append("-DDEPENDENCY_OPENIMAGEIO_INCLUDE=" +deps_dir +"/vcpkg/installed/x64-linux/include")
	cmake_args.append("-DDEPENDENCY_OPENIMAGEIO_LIBRARY=" +deps_dir +"/vcpkg/installed/x64-linux/lib/libOpenImageIO.a")
else:
	subprocess.run([vcpkg_root +"/vcpkg","install","openimageio"],check=True)
	cmake_args.append("-DDEPENDENCY_OPENIMAGEIO_INCLUDE=" +deps_dir +"/vcpkg/installed/x64-windows/include")
	cmake_args.append("-DDEPENDENCY_OPENIMAGEIO_LIBRARY=" +deps_dir +"/vcpkg/installed/x64-windows/lib/OpenImageIO.lib")
