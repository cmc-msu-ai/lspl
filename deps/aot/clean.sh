#!/bin/sh

find | egrep "\\.d$|\\.otst$|\\.orst$|\\.a$" | xargs rm
