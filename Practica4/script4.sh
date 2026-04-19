#!/bin/bash
clear
select i in Directorio DirectorioExtendido Salir
do case $i in
        Directorio) ls;;
        DirectorioExtendido) ls -l;;
        Salir) break;;
    esac
done
#
for i in hola como estan todos
do
    echo -n $i
    echo -n " "
done