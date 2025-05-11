#!/bin/bash

# 比较文件对并输出相同内容的文件
diff -q gol_output_0.txt gol_output_serial_0.txt && echo "gol_output_0.txt and gol_output_serial_0.txt are identical"
diff -q gol_output_10.txt gol_output_serial_10.txt && echo "gol_output_10.txt and gol_output_serial_10.txt are identical"
diff -q gol_output_20.txt gol_output_serial_20.txt && echo "gol_output_20.txt and gol_output_serial_20.txt are identical"
diff -q gol_output_30.txt gol_output_serial_30.txt && echo "gol_output_30.txt and gol_output_serial_30.txt are identical"
diff -q gol_output_40.txt gol_output_serial_40.txt && echo "gol_output_40.txt and gol_output_serial_40.txt are identical"
diff -q gol_output_50.txt gol_output_serial_50.txt && echo "gol_output_50.txt and gol_output_serial_50.txt are identical"
diff -q gol_output_60.txt gol_output_serial_60.txt && echo "gol_output_60.txt and gol_output_serial_60.txt are identical"
diff -q gol_output_70.txt gol_output_serial_70.txt && echo "gol_output_70.txt and gol_output_serial_70.txt are identical"
diff -q gol_output_80.txt gol_output_serial_80.txt && echo "gol_output_80.txt and gol_output_serial_80.txt are identical"
diff -q gol_output_90.txt gol_output_serial_90.txt && echo "gol_output_90.txt and gol_output_serial_90.txt are identical"
