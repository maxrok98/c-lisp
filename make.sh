#!/bin/sh

gcc -g -Wall interpret.c tokenizer.c parser.c eval.c env.c utils.c debug_utils.c -o interpret
