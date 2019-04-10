# How to use:
# docker build -t zeiterfassung .
# docker run -it --rm -e DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix -v $HOME/.config/db-software:/root/.config/db-software --net host zeiterfassung

# Troubleshooting:
# X-Server permission errors: xhost +SI:localuser:root


FROM archlinux/base

RUN pacman -Sy --noconfirm qt5-base qt5-multimedia

RUN pacman -S --noconfirm git gcc make cmake qt5-tools qt5-translations

RUN cd /opt \
 && git clone https://github.com/0xFEEDC0DE64/DbSoftware.git \
 && cd DbSoftware \
 && git submodule update --init --recursive 3rdparty/Quazip DbSketch libs/DbCoreLib libs/DbGuiLib libs/DbNetworkLib

ADD . /opt/DbSoftware/DbZeiterfassung

RUN mkdir /opt/build_DbSoftware \
 && cd /opt/build_DbSoftware \
 && cmake ../DbSoftware \
 && make -j8 zeiterfassung



FROM archlinux/base

RUN pacman -Sy --noconfirm qt5-base qt5-multimedia

RUN pacman -S --noconfirm ttf-dejavu

COPY --from=0 /opt/build_DbSoftware /opt/build_DbSoftware

CMD /opt/build_DbSoftware/bin/zeiterfassungclient
