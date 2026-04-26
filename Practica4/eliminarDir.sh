#!/bin/bash

echo $1

ls $1

for archivo in "$1"/*
do
    nombre=$(basename "$archivo")
    echo "$nombre"

    longitud=${#nombre}
    contador=0

    for (( i=0; i<longitud; i++ ))
    do
        caracter=${nombre:$i:1}

        if [[ "$caracter" == "i" && $i -eq 1 ]]
        then
            contador=$((contador + 1))
        fi

        if [[ "$caracter" == "u" && $i -eq 3 ]]
        then
            contador=$((contador + 1))
        fi

        echo "$caracter"
    done

    echo "$contador"

    if [ $contador -eq 2 ]
    then
        rm "$archivo"
    fi
done

