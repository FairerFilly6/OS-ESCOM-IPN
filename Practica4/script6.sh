#!/bin/bash
clear

if [ -z "$1" ]
then
    echo "Uso: ./script6.sh <directorio>"
    exit 1
fi

if [ -d "$1" ]
then
    echo "El directorio $1 existe."
    echo ""
    echo "----------------------------- Archivos en el directorio $1 -----------------------------"
    contador=0
    for archivo in "$1"/*
    do
        if [ -f "$archivo" ]
        then
            echo "Archivo: $archivo"
            contador=$((contador + 1))
        fi
    done
    echo ""
    echo "Número total de archivos en el directorio $1: $contador"
else
    echo "El directorio $1 no existe."
fi
echo ""
echo "------------------ Script6 terminado ----------------"