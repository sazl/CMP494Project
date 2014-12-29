#!/bin/sh

./clean.sh && ./build.sh && cd bench && python run.py
