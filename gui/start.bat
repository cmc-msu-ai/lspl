set RML="..\data"

cd bin
java -cp "lspl.jar;lspl-gui.jar;..\lib\org.eclipse.swt.win32.win32.jar;..\lib\org.eclipse.jface.jar;..\lib\org.eclipse.equinox.common.jar;..\lib\org.eclipse.core.commands.jar;..\lib\org.apache.poi.jar;..\lib\org.apache.poi-ooxml.jar;..\lib\org.apache.ooxml-schemas.jar;..\lib\org.apache.xmlbeans.jar;..\lib\org.apache.poi-scratchpad.jar;..\lib\org.apache.log4j.jar;..\lib\org.dom4j.jar;..\lib" ru.lspl.gui.MainWindow
cd ..