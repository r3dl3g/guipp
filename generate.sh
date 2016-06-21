mkdir -p build/gui
cd build/gui
cmake "../.."
cd ../..
mkdir -p build/Test
pushd build/Test
cmake "../../test" 
cd ../..
