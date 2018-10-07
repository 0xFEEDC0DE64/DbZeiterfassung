TEMPLATE = subdirs

SUBDIRS += plugins \
           zeiterfassung \
           zeiterfassungcorelib \
           zeiterfassungguilib \
           zeiterfassungnetworklib

plugins.depends += zeiterfassungcorelib zeiterfassungguilib zeiterfassungnetworklib
zeiterfassung.depends += zeiterfassungcorelib zeiterfassungguilib zeiterfassungnetworklib
zeiterfassungguilib.depends += zeiterfassungcorelib zeiterfassungnetworklib
