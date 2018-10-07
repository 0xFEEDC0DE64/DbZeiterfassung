# DbZeiterfassung
Time tracking tool for work with plugin support

[![Build Status](https://travis-ci.org/0xFEEDC0DE64/DbZeiterfassung.svg?branch=master)](https://travis-ci.org/0xFEEDC0DE64/DbZeiterfassung) [![Codacy Badge](https://api.codacy.com/project/badge/Grade/7221b324857c41dca8939264dd1d3d3e)](https://www.codacy.com/app/0xFEEDC0DE64/DbZeiterfassung?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=0xFEEDC0DE64/DbZeiterfassung&amp;utm_campaign=Badge_Grade)

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
