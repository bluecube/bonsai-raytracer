#!/bin/sh

cd $(dirname $0)/..

make DEBUG=0 clean all

cd ../server

./server.pl -w 200 --chunksize=3000000 ../test-scenes/thousandSpheres.scene &
sleep 2

cd ../client

rate=$(./bonsai-client localhost | tee /dev/stderr | sed -n 's/rate: \([[:digit:]]*\).*/\1/p')

sleep 1

echo
echo
echo "rate: $rate"
