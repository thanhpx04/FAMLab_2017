#!/bin/bash
# Fill hole in binary images
#path:  /home/linh/Datasets/resize_256x192/tete/
FOLDER="/home/linh/Datasets/Morphometrics/pronotum/Images_without_grid_2/*"
SAVEFOLDER="/home/linh/Desktop/results/2017/pronotum/holefill/"
JPGARRAY=(${FOLDER})
TOTAL=${#JPGARRAY[@]}
EXECUTE="./MAELab_CI"
for((i=0; i<$TOTAL; i++))
do
	IMAGE="${JPGARRAY[$i]}"
	echo $IMAGE
	SAVEIMAGE="$SAVEFOLDER$(basename "${JPGARRAY[$i]}")"
	echo $SAVEIMAGE
	$EXECUTE "$IMAGE" "$SAVEIMAGE"
done
