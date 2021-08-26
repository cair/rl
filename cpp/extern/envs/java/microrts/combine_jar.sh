#!/usr/bin/env bash

# https://github.com/santiontanon/microrts/tree/db59145a0872f500f7ca4d128109cb74455d0be0#generating-a-jar-file
find $1 -name "*.jar" | xargs -n 1 jar xvf
jar xvf $2
jar cvf $2 $(find . -name '*.class' -type f)