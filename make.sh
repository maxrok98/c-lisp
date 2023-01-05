#!/bin/sh

gcc -g -Wall interpret.c tokenizer.c parser.c utils.c debug_utils.c -o interpret
