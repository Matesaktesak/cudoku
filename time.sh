#!/bin/bash
echo "Running timed run"
for i in {1..10000}
do
    ./build/cudoku solve preset4 -s
done