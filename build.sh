rm -rf build
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="/usr/share/R;/usr/lib/R" -DPYTHON_EXECUTABLE=$(which python) -DCMAKE_PREFIX_PATH="/home/stu11/s12/ltm4163/miniconda3/lib/python3.12/site-packages/pybind11/share/cmake/pybind11" ..
make
