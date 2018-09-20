# DbZeiterfassung
DbZeiterfassung git module for use in DbSoftware (https://github.com/0xFEEDC0DE64/DbSoftware)

# Building from source
```Shell
git clone https://github.com/0xFEEDC0DE64/DbSoftware.git
cd DbSoftware
git submodule update --init --recursive 3rdparty/Quazip DbSketch DbZeiterfassung libs/DbCoreLib libs/DbGuiLib libs/DbNetworkLib
cd ..
mkdir build_DbSoftware
cd build_DbSoftware
qmake CONFIG+=ccache ../DbSoftware
make -j$(nproc) sub-DbZeiterfassung
make sub-DbZeiterfassung-install_subtargets
./bin/zeiterfassung
```
