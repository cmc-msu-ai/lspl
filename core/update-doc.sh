#!/bin/sh

doxygen LsplCore.doxyfile
cd doc && ./upload.sh
