#!/bin/sh
# UTF-8に漢字コード変換するスクリプト
#
# プログラミング環境
find -name '*.c' | xargs nkf --overwrite -w
find -name '*.s' | xargs nkf --overwrite -w
find -name '*.h' | xargs nkf --overwrite -w
find -name '*.x' | xargs nkf --overwrite -w
find -name 'Makefile*' | xargs nkf --overwrite -w
find -name 'make*' | xargs nkf --overwrite -w
#
# tex環境
#find -name '*.txt' | xargs nkf --overwrite -w
#find -name '*.sty' | xargs nkf --overwrite -w
#find -name '*.tex' | xargs nkf --overwrite -w
