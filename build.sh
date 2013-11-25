#!/bin/sh
g++ -lGL -lGLU -lglut -std=gnu++11 -lboost_system -lpng image.cpp pong.cpp PaddleBehavior.cpp -o pong