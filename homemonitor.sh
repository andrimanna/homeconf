#!/bin/bash
until homebridge -I; do
echo "Homebridge è crashato con codice $?.  Rieseguo..." >&2
sleep 1
done                                                                                                                         
