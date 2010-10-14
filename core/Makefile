include ../common/common.make

doxygen:
	doxygen LsplCore.doxyfile

upload-doxygen: doyxgen
	cd doc && scp -r html/* lspl@s.lspl.ru:/home/lspl/data/www/static.lspl.ru/doc-core