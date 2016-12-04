#!/bin/sh

#convertImage <pcx file> <output file> <imageData name> <palette file> <palette name>

#EXAMPLE ./convertImage.sh level1.pcx level1.bin level1Data level1_pal.bin level1Palette

#PATHS TO EXTERNAL PROGRAMS. LEAVE BLANK IF THEY ARE IN YOUR PATH

GFX2GBA_PATH=""
GBACRUSHERCL_PATH=""
BIN2O_PATH=""


#SCRIPT BEGINS

if [ $# -eq 0 ]
then
    echo "convertImage <pcx file> <output file> <imageData name> <palette file> <palette name>"
else 
    echo
    export LC_ALL="en_US"


    PCX_FILE="../$1"
	
    IMAGE_BIN=$2
    IMAGE_VAR=$3
    
    PAL_BIN=$4
    PAL_VAR=$5
    
        
        # create output directory
    
    mkdir $IMAGE_VAR
    echo "Output directory ./$IMAGE_VAR/ created" > output.txt
    echo "Output directory ./$IMAGE_VAR/ created"
    cd $IMAGE_VAR

	# convert image to raw binary data and palette
    
    echo "Converting image to binary (gfx2gba)"
    CMD="wine ../gfx2gba.exe $PCX_FILE $IMAGE_BIN -v -s 2 -n -c $IMAGE_VAR -b -p $PAL_BIN"
    echo "command line: $CMD" >> output.txt
    echo "" >> output.txt
    wine ../gfx2gba.exe $PCX_FILE $IMAGE_BIN -v -z 16 -m -n -c $IMAGE_VAR -b -p $PAL_BIN | more >> output.txt
    
    
	# compress image data using gbacrusher
    echo "" >> output.txt
    echo "Compressing image data, lz77 VRAM Safe (GBACrusherCL)"
    CMD="wine ../GBACrusherCL.exe -LV $IMAGE_BIN -O $IMAGE_BIN.lz77"
    echo "command line: $CMD" >> output.txt
    echo "" >> output.txt
    wine ../GBACrusherCL.exe -LV $IMAGE_BIN -O $IMAGE_BIN.lz77 | more >> output.txt

    
        # create object files
    echo "" >> output.txt
    echo "Creating object files (bin2o)"
    CMD="wine ../bin2o.exe /const /interwork $IMAGE_BIN.lz77 $IMAGE_BIN.lz77.o $IMAGE_VAR"
    echo "command line: $CMD" >> output.txt
    echo "" >> output.txt
    wine ../bin2o.exe /const /interwork $IMAGE_BIN.lz77 $IMAGE_BIN.lz77.o $IMAGE_VAR | more >> output.txt
    
    CMD="wine ../bin2o.exe /const /interwork $PAL_BIN $PAL_BIN.o $PAL_VAR" 
    echo "command line: $CMD" >> output.txt
    echo "" >> output.txt
    wine ../bin2o.exe /const /interwork $PAL_BIN $PAL_BIN.o $PAL_VAR | more >> output.txt
    
    
	# clean up by deleting *.bin and *.lz77 
    echo "Cleaning up"
    CMD="rm *.bin *.lz77"
    echo "command line: $CMD" >> output.txt
    echo "" >> output.txt
    rm *.bin *.lz77 | more >> output.txt
    
    
        # say goodbye
    cd ..
    echo "The object files are stored in: ./$IMAGE_VAR/"
    echo
fi


#SCRIPT ENDS
