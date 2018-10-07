# DbZeiterfassung
Time tracking tool for work with plugin support

## Building from source
This project can only be built as part of the project structure [DbSoftware](https://github.com/0xFEEDC0DE64/DbSoftware)

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
