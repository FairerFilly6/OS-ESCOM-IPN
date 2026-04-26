#!/bin/bash

segundo=$(date +%S)
intentos=0
numero=$((segundo % 10 + 1))
echo $numero

echo "Adivina el número del 1 al 10:"
read intento


while [ $intento -ne $numero ]
do
    intentos=$((intentos + 1))
    if [ $intento -gt $numero ]
    then
        echo "El numero a adivinar es menor"
        echo "Adivina el número del 1 al 10:"
        read intento
    fi
    if [ $intento -lt $numero ]
    then
        echo "El numero a adivinar es mayor"
        echo "Adivina el número del 1 al 10:"
        read intento
    fi
    
done

echo "Correcto!"
echo "Intentos:"
echo $intentos