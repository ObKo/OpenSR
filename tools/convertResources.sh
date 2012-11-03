#!/bin/bash

#  This script converts Photoshop's PSD files of planet textures 
# from original game to png images with alpha channel.
#  This script uses psdparse (http://www.telegraphics.com.au/svn/psdparse/)

RESOURCE_CONVETER=opensr-resourceconverter
PSD_PARSE=psdparse
INPUT_DIR=data
OUTPUT_DIR=data/ORC
TEMP_DIR=/tmp

PLANET_FILES=(
'Planet2/180/04x180.psd' \
'Planet2/180/17x180.psd' \
'Planet2/180/23x180.psd' \
'Planet2/180/cloud/r02.psd' \
'Planet2/180/cloud/c07.psd' \
'Planet2/180/cloud/c09.psd' \
'Planet2/180/cloud/c01.psd' \
'Planet2/180/cloud/c10.psd' \
'Planet2/180/cloud/c03.psd' \
'Planet2/180/cloud/c04.psd' \
'Planet2/180/cloud/b01.psd' \
'Planet2/180/cloud/r01.psd' \
'Planet2/180/cloud/c06.psd' \
'Planet2/180/cloud/y02.psd' \
'Planet2/180/cloud/c05.psd' \
'Planet2/180/cloud/c08.psd' \
'Planet2/180/cloud/c02.psd' \
'Planet2/180/21x180.psd' \
'Planet2/180/03x180.psd' \
'Planet2/180/13x180.psd' \
'Planet2/180/22x180.psd' \
'Planet2/180/14x180.psd' \
'Planet2/180/05x180.psd' \
'Planet2/180/20x180.psd' \
'Planet2/180/02x180.psd' \
'Planet2/180/06x180.psd' \
'Planet2/180/01x180.psd' \
'Planet2/180/29x180.psd' \
'Planet2/180/12x180.psd' \
'Planet2/180/15x180.psd' \
'Planet2/180/16x180.psd' \
'Planet2/180/27x180.psd' \
'Planet2/180/08x180.psd' \
'Planet2/180/24x180.psd' \
'Planet2/180/07x180.psd' \
'Planet2/180/09x180.psd' \
'Planet2/180/19x180.psd' \
'Planet2/180/28x180.psd' \
'Planet2/180/26x180.psd' \
'Planet2/180/25x180.psd' \
'Planet2/180/11x180.psd' \
'Planet2/180/18x180.psd' \
'Planet2/180/10x180.psd' \
'Planet2/140/27x140.psd' \
'Planet2/140/26x140.psd' \
'Planet2/140/09x140.psd' \
'Planet2/140/24x140.psd' \
'Planet2/140/cloud/r02.psd' \
'Planet2/140/cloud/c09.psd' \
'Planet2/140/cloud/c01.psd' \
'Planet2/140/cloud/c10.psd' \
'Planet2/140/cloud/c03.psd' \
'Planet2/140/cloud/c04.psd' \
'Planet2/140/cloud/b01.psd' \
'Planet2/140/cloud/y01.psd' \
'Planet2/140/cloud/r01.psd' \
'Planet2/140/cloud/c06.psd' \
'Planet2/140/cloud/y02.psd' \
'Planet2/140/cloud/c05.psd' \
'Planet2/140/cloud/c08.psd' \
'Planet2/140/cloud/c02.psd' \
'Planet2/140/36x140.psd' \
'Planet2/140/30x140.psd' \
'Planet2/140/04x140.psd' \
'Planet2/140/29x140.psd' \
'Planet2/140/19x140.psd' \
'Planet2/140/38x140.psd' \
'Planet2/140/34x140.psd' \
'Planet2/140/15x140.psd' \
'Planet2/140/17x140.psd' \
'Planet2/140/22x140.psd' \
'Planet2/140/01x140.psd' \
'Planet2/140/08x140.psd' \
'Planet2/140/23x140.psd' \
'Planet2/140/32x140.psd' \
'Planet2/140/25x140.psd' \
'Planet2/140/35x140.psd' \
'Planet2/140/12x140.psd' \
'Planet2/140/18x140.psd' \
'Planet2/140/33x140.psd' \
'Planet2/140/05x140.psd' \
'Planet2/140/14x140.psd' \
'Planet2/140/20x140.psd' \
'Planet2/140/11x140.psd' \
'Planet2/140/02x140.psd' \
'Planet2/140/03x140.psd' \
'Planet2/140/31x140.psd' \
'Planet2/140/37x140.psd' \
'Planet2/140/21x140.psd' \
'Planet2/140/13x140.psd' \
'Planet2/140/16x140.psd' \
'Planet2/140/28x140.psd' \
'Planet2/140/10x140.psd' \
'Planet2/140/07x140.psd' \
'Planet2/140/06x140.psd' \
'Planet2/200/47x200.psd' \
'Planet2/200/43x200.psd' \
'Planet2/200/32x200.psd' \
'Planet2/200/35x200.psd' \
'Planet2/200/cloud/c07.psd' \
'Planet2/200/cloud/c09.psd' \
'Planet2/200/cloud/c01.psd' \
'Planet2/200/cloud/c10.psd' \
'Planet2/200/cloud/c03.psd' \
'Planet2/200/cloud/c04.psd' \
'Planet2/200/cloud/b01.psd' \
'Planet2/200/cloud/y01.psd' \
'Planet2/200/cloud/r01.psd' \
'Planet2/200/cloud/c06.psd' \
'Planet2/200/cloud/y02.psd' \
'Planet2/200/cloud/c05.psd' \
'Planet2/200/cloud/c08.psd' \
'Planet2/200/cloud/c02.psd' \
'Planet2/200/49x200.psd' \
'Planet2/200/46x200.psd' \
'Planet2/200/03x200.psd' \
'Planet2/200/15x200.psd' \
'Planet2/200/48x200.psd' \
'Planet2/200/31x200.psd' \
'Planet2/200/26x200.psd' \
'Planet2/200/13x200.psd' \
'Planet2/200/25x200.psd' \
'Planet2/200/27x200.psd' \
'Planet2/200/16x200.psd' \
'Planet2/200/45x200.psd' \
'Planet2/200/36x200.psd' \
'Planet2/200/22x200.psd' \
'Planet2/200/23x200.psd' \
'Planet2/200/02x200.psd' \
'Planet2/200/14x200.psd' \
'Planet2/200/39x200.psd' \
'Planet2/200/08x200.psd' \
'Planet2/200/12x200.psd' \
'Planet2/200/40x200.psd' \
'Planet2/200/19x200.psd' \
'Planet2/200/38x200.psd' \
'Planet2/200/24x200.psd' \
'Planet2/200/33x200.psd' \
'Planet2/200/10x200.psd' \
'Planet2/200/44x200.psd' \
'Planet2/200/04x200.psd' \
'Planet2/200/29x200.psd' \
'Planet2/200/05x200.psd' \
'Planet2/200/09x200.psd' \
'Planet2/200/01x200.psd' \
'Planet2/200/18x200.psd' \
'Planet2/200/28x200.psd' \
'Planet2/200/17x200.psd' \
'Planet2/200/21x200.psd' \
'Planet2/200/42x200.psd' \
'Planet2/200/20x200.psd' \
'Planet2/200/37x200.psd' \
'Planet2/200/07x200.psd' \
'Planet2/200/06x200.psd' \
'Planet2/200/41x200.psd' \
'Planet2/200/34x200.psd' \
'Planet2/200/11x200.psd' \
'Planet2/200/30x200.psd' \
'Planet2/160/25x160.psd' \
'Planet2/160/cloud/r02.psd' \
'Planet2/160/cloud/c07.psd' \
'Planet2/160/cloud/c09.psd' \
'Planet2/160/cloud/c01.psd' \
'Planet2/160/cloud/c10.psd' \
'Planet2/160/cloud/c03.psd' \
'Planet2/160/cloud/c04.psd' \
'Planet2/160/cloud/b01.psd' \
'Planet2/160/cloud/y01.psd' \
'Planet2/160/cloud/r01.psd' \
'Planet2/160/cloud/c06.psd' \
'Planet2/160/cloud/y02.psd' \
'Planet2/160/cloud/c05.psd' \
'Planet2/160/cloud/c08.psd' \
'Planet2/160/cloud/c02.psd' \
'Planet2/160/38x160.psd' \
'Planet2/160/11x160.psd' \
'Planet2/160/19x160.psd' \
'Planet2/160/02x160.psd' \
'Planet2/160/26x160.psd' \
'Planet2/160/43x160.psd' \
'Planet2/160/30x160.psd' \
'Planet2/160/05x160.psd' \
'Planet2/160/41x160.psd' \
'Planet2/160/28x160.psd' \
'Planet2/160/34x160.psd' \
'Planet2/160/21x160.psd' \
'Planet2/160/14x160.psd' \
'Planet2/160/17x160.psd' \
'Planet2/160/03x160.psd' \
'Planet2/160/39x160.psd' \
'Planet2/160/35x160.psd' \
'Planet2/160/10x160.psd' \
'Planet2/160/33x160.psd' \
'Planet2/160/13x160.psd' \
'Planet2/160/24x160.psd' \
'Planet2/160/20x160.psd' \
'Planet2/160/36x160.psd' \
'Planet2/160/16x160.psd' \
'Planet2/160/08x160.psd' \
'Planet2/160/18x160.psd' \
'Planet2/160/42x160.psd' \
'Planet2/160/40x160.psd' \
'Planet2/160/27x160.psd' \
'Planet2/160/44x160.psd' \
'Planet2/160/31x160.psd' \
'Planet2/160/32x160.psd' \
'Planet2/160/01x160.psd' \
'Planet2/160/29x160.psd' \
'Planet2/160/37x160.psd' \
'Planet2/160/04x160.psd' \
'Planet2/160/22x160.psd' \
'Planet2/160/09x160.psd' \
'Planet2/160/15x160.psd' \
'Planet2/160/07x160.psd' \
'Planet2/160/06x160.psd' \
'Planet2/160/46x160.psd' \
'Planet2/160/45x160.psd' \
'Planet2/160/23x160.psd' \
'Planet2/160/12x160.psd' \
'Planet2/Solar/Venus.psd' \
'Planet2/Solar/Neptune.psd' \
'Planet2/Solar/cloud' \
'Planet2/Solar/cloud/r02.psd' \
'Planet2/Solar/cloud/c07.psd' \
'Planet2/Solar/cloud/c10.psd' \
'Planet2/Solar/cloud/c04.psd' \
'Planet2/Solar/cloud/c05.psd' \
'Planet2/Solar/cloud/c08.psd' \
'Planet2/Solar/cloud/c02.psd' \
'Planet2/Solar/Saturn.psd' \
'Planet2/Solar/Earth.psd' \
'Planet2/Solar/Mars.psd' \
'Planet2/Solar/Jupiter.psd' \
'Planet2/Solar/Mercur.psd' \
'Planet2/120/07x120.psd' \
'Planet2/120/25x120.psd' \
'Planet2/120/27x120.psd' \
'Planet2/120/33x120.psd' \
'Planet2/120/cloud/r02.psd' \
'Planet2/120/cloud/c07.psd' \
'Planet2/120/cloud/c09.psd' \
'Planet2/120/cloud/c01.psd' \
'Planet2/120/cloud/c10.psd' \
'Planet2/120/cloud/c03.psd' \
'Planet2/120/cloud/c04.psd' \
'Planet2/120/cloud/b01.psd' \
'Planet2/120/cloud/y01.psd' \
'Planet2/120/cloud/r01.psd' \
'Planet2/120/cloud/c06.psd' \
'Planet2/120/cloud/y02.psd' \
'Planet2/120/cloud/c05.psd' \
'Planet2/120/cloud/c08.psd' \
'Planet2/120/cloud/c02.psd' \
'Planet2/120/18x120.psd' \
'Planet2/120/15x120.psd' \
'Planet2/120/03x120.psd' \
'Planet2/120/31x120.psd' \
'Planet2/120/13x120.psd' \
'Planet2/120/04x120.psd' \
'Planet2/120/24x120.psd' \
'Planet2/120/01x120.psd' \
'Planet2/120/06x120.psd' \
'Planet2/120/08x120.psd' \
'Planet2/120/28x120.psd' \
'Planet2/120/38x120.psd' \
'Planet2/120/32x120.psd' \
'Planet2/120/34x120.psd' \
'Planet2/120/37x120.psd' \
'Planet2/120/30x120.psd' \
'Planet2/120/02x120.psd' \
'Planet2/120/09x120.psd' \
'Planet2/120/35x120.psd' \
'Planet2/120/29x120.psd' \
'Planet2/120/05x120.psd' \
'Planet2/120/19x120.psd' \
'Planet2/120/11x120.psd' \
'Planet2/120/10x120.psd' \
'Planet2/120/22x120.psd' \
'Planet2/120/36x120.psd' \
'Planet2/120/26x120.psd' \
'Planet2/120/17x120.psd' \
'Planet2/120/23x120.psd' \
'Planet2/120/16x120.psd' \
'Planet2/120/14x120.psd' \
'Planet2/120/12x120.psd' \
'Planet2/120/21x120.psd'  \
'Planet2/120/20x120.psd'
)

convert_planets () 
{ 
  for planet in "${PLANET_FILES[@]}"
  do
    if [ -e "$INPUT_DIR/DATA/$planet" ]
    then
      name=$(basename $planet)
      dir=$(dirname $planet)
      extension="${name##*.}"
      filename="${name%.*}"
      echo "$INPUT_DIR/DATA/$planet -> $OUTPUT_DIR/$dir/$filename.png"
      $PSD_PARSE -d $TEMP_DIR -w -q -n $INPUT_DIR/DATA/$planet
      if [ -e "$TEMP_DIR/$name.png" ]
      then
        mkdir -p $OUTPUT_DIR/$dir
        if [ -e "$TEMP_DIR/$name.1.png" ]
        then
          $RESOURCE_CONVETER create_planet $TEMP_DIR/$name.png $TEMP_DIR/$name.1.png $OUTPUT_DIR/$dir/$filename.png
          rm $TEMP_DIR/$name.png $TEMP_DIR/$name.1.png
        else
          cp $TEMP_DIR/$name.png $OUTPUT_DIR/$dir/$filename.png
          rm $TEMP_DIR/$name.png
        fi
      fi
    fi
  done
}

convert_planets
