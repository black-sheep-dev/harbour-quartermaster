#!/bin/bash

app="harbour-quartermaster"

sizes=(86 108 128 172 512)

# optimze svg
scour $app.svg $app-optimized.svg

# copy optimized svg to scaleable folder
cp $app-optimized.svg scaleable/$app.svg
rm $app-optimized.svg

# convert to png
for size in "${sizes[@]}";
do
convert -background none -size $size"x"$size $app.svg "$size"x"$size"/$app.png
done
