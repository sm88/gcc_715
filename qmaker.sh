#!/bin/bash
rm *.pro
qmake -project -o specviz.pro
sed -i '5i QT += core gui' specviz.pro 
sed -i '6i greaterThan(QT_MAJOR_VERSION, 4): QT += widgets' specviz.pro
