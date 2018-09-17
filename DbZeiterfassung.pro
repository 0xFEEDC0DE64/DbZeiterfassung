TEMPLATE = subdirs

SUBDIRS += plugins \
           zeiterfassung \
           zeiterfassungcorelib \
           zeiterfassungguilib

plugins.depends += zeiterfassungcorelib zeiterfassungguilib
zeiterfassung.depends += zeiterfassungcorelib zeiterfassungguilib
zeiterfassungguilib.depends += zeiterfassungcorelib
