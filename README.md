# DbZeiterfassung
DbZeiterfassung git module for use in DbSoftware (https://github.com/0xFEEDC0DE64/DbSoftware)

# Building from source
```Shell
git clone --recursive https://github.com/0xFEEDC0DE64/DbSoftware.git
mkdir build_DbSoftware
cd build_DbSoftware
qmake CONFIG+=ccache ../DbSoftware
make -j$(nproc) sub-DbZeiterfassung sub-DbZeiterfassung-install_subtargets
./bin/zeiterfassung
```