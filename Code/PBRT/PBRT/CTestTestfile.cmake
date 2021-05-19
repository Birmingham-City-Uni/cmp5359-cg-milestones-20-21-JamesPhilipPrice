# CMake generated Testfile for 
# Source directory: E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone
# Build directory: E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/PBRT
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pbrt_unit_test "pbrt_test")
set_tests_properties(pbrt_unit_test PROPERTIES  _BACKTRACE_TRIPLES "E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/CMakeLists.txt;559;ADD_TEST;E:/Projects/Github/cmp5359-cg-milestones-20-21-JamesPhilipPrice/Code/PBRT/pbrt-v3_clone/CMakeLists.txt;0;")
subdirs("src/ext/zlib")
subdirs("src/ext/openexr")
subdirs("src/ext/glog")
subdirs("src/ext/ptex")
