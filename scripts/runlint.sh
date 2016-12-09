#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

python ${DIR}/cpplint.py \
--recursive \
--extensions=h,cpp \
${DIR}/../include/ ${DIR}/../src/ ${DIR}/../test/
