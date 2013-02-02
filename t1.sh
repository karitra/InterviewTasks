#!/usr/bin/env bash
#

tmp=`tempfile -p t1wrk`

for i in {1..1024}; do
	echo -n "$i "
done >> $tmp

for i in {1026..2047}; do
	echo -n "$i "
done >> $tmp

for i in {2049..4096}; do
	echo -n "$i "
done >> $tmp

cat $tmp | ./t1 | tail -n 1
> $tmp

for i in {1..1024}; do
	echo -n "$i "
done >> $tmp

for i in {1026..2047}; do
	echo -n "$i "
done >> $tmp

for i in {2049..4097}; do
	echo -n "$i "
done >> $tmp

cat $tmp | ./t1 | tail -n 1
> $tmp


for i in {1..1025}; do
	echo -n "$i "
done >> $tmp

for i in {1027..2048}; do
	echo -n "$i "
done >> $tmp

for i in {2050..4096}; do
	echo -n "$i "
done >> $tmp

cat $tmp | ./t1 | tail -n 1
> $tmp

for i in {1..1025}; do
	echo -n "$i "
done >> $tmp

for i in {1027..2048}; do
	echo -n "$i "
done >> $tmp

for i in {2050..4097}; do
	echo -n "$i "
done >> $tmp

cat $tmp | ./t1 | tail -n 1
> $tmp

for i in {2..1025}; do
	echo -n "$i "
done >> $tmp

for i in {1027..65536}; do
	echo -n "$i "
done >> $tmp

cat $tmp | ./t1 | tail -n 1
rm $tmp

