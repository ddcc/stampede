#!/bin/bash

#set -e

LABEL="[A-Za-z0-9() -_]+: [0-9]+"

if [ $# -lt 1 ]; then
  echo "$0 <dir>"
  exit
fi

INNER=0
LOG="${1}/abort0.csv"
for i in "${1}/"*.log; do
    FILENAME=$(basename $i)
    FILENAME=${FILENAME%.*}

    if [ $INNER -eq 0 ]; then
      echo -e "Benchmark\nTime" > "${LOG}"
      grep -E "${LABEL}" "${1}/labyrinth.log" | cut -d : -f 1 >> "${LOG}"
    fi

    grep -E "[tT]ime[a-z:= ]* |${LABEL}" ${i} | sed -r "s/[A-Za-z0-9 ()_-]*[:= ]+//" | (echo ${FILENAME} && cat) | paste -d, "${LOG}" - > "${LOG}.tmp"
    mv "${LOG}.tmp" "${LOG}"

    INNER=$((INNER+1))
done
