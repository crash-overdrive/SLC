#!/usr/bin/env bash

printf "{\n"

for node in $(find $1 -mindepth 1 -maxdepth 1 | sort); do
  if [[ -d ${node} ]]; then
    printf "{\n"
    for file in $(find ${node} -type f); do
      printf "\"$(sed "s|$1/||g" <<< ${file})\",\n"
    done
    printf "},\n"
  fi
  if [[ -f ${node} ]]; then
    printf "{\n"
    printf "\"$(sed "s|$1/||g" <<< ${node})\",\n"
    printf "},\n"
  fi
done

printf "}\n"
