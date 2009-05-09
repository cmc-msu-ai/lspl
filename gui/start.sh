export RML="$(pwd)/data"
export LD_LIBRARY_PATH="$(pwd)/bin"

java -cp "bin/lspl.jar:bin/lspl-gui.jar:lib/org.eclipse.swt.gtk.linux.jar:lib/org.eclipse.jface.jar:lib/org.eclipse.equinox.common.jar:lib/org.eclipse.core.commands.jar:lib/org.apache.poi.jar:lib/org.apache.poi-ooxml.jar:lib/org.apache.ooxml-schemas.jar:lib/org.apache.xmlbeans.jar:lib/org.apache.poi-scratchpad.jar:lib/org.apache.log4j.jar:lib/org.dom4j.jar:lib" ru.lspl.gui.MainWindow

