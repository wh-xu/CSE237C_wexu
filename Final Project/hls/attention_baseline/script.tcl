############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 2013 Xilinx Inc. All rights reserved.
############################################################
open_project hls
set_top attention
add_files attention.h
add_files attention.cpp

add_files -tb input.input.dat
add_files -tb input.weight_K.dat
add_files -tb input.weight_Q.dat
add_files -tb input.weight_V.dat
add_files -tb output.output.dat

add_files -tb attention_test.cpp
open_solution "solution1"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default


