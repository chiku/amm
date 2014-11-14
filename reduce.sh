#!/bin/sh

ls -hal amm
strip -sxXv amm
ls -hal amm
upx -9 --ultra-brute -v amm
ls -hal amm
