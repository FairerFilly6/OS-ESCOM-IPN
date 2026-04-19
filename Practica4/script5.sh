#!/bin/bash
clear

NUEVO_SCRIPT="nuevo_script.sh"

echo "------------------ Creando archivo: $NUEVO_SCRIPT ----------------"
echo "#!/bin/bash" > $NUEVO_SCRIPT
echo "echo 'Hola, este es un nuevo script generado por el script5'" >> $NUEVO_SCRIPT
echo ""
echo "------------------ Archivo $NUEVO_SCRIPT creado ----------------"
ls -l 

echo ""
echo "------------------ Editando archivo: $NUEVO_SCRIPT ----------------"
echo "Abriendo el editor para agregar más contenido al script..."
nano $NUEVO_SCRIPT


chmod u+x $NUEVO_SCRIPT
echo ""
echo "------------------ Permisos cambiados en $NUEVO_SCRIPT ----------------"
ls -l

echo""
echo "------------------ Ejecutando archivo $NUEVO_SCRIPT ----------------"
./$NUEVO_SCRIPT

echo ""
echo "------------------ Script5 generador terminado ----------------"



