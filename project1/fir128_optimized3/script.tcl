############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project optimized3
set_top fir
add_files fir.h
add_files fir.cpp
add_files -tb input.dat -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb out.gold.dat -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
add_files -tb fir_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "loop_fission_II_1"
set_part {xc7vx485tffg1157-1}
create_clock -period 10 -name default
#source "./optimized3/loop_fission_II_1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
