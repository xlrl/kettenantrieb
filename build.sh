#!/bin/sh
cmake -H. -B_build -GNinja
cmake --build _build
