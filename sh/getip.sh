#!/bin/bash
ifconfig | grep "inet " | grep -v 127 | awk '{print $2}'

