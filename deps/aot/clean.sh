#!/bin/sh

find | egrep "\\.d$|\\.otst$|\\.orst$|\\.ormst$|\\.a$|\\.exe$|\\.dll$" | xargs rm
