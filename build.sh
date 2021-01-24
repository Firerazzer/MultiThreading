#!/bin/bash
rm service/bin/*
rm capteur/bin/*
rm watchdog/bin/*

g++ service/src/*.cpp -o service/bin/Service -lpthread
g++ capteur/src/*.cpp -o capteur/bin/Capteur -lpthread
g++ watchdog/src/*.cpp -o watchdog/bin/Watchdog -lpthread