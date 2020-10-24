############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2019 Xilinx, Inc. All Rights Reserved.
############################################################
open_project cordiccart2pol
set_top cordiccart2pol
add_files cordiccart2pol.cpp
add_files cordiccart2pol.h
add_files -tb cordiccart2pol_test.cpp -cflags "-Wno-unknown-pragmas" -csimflags "-Wno-unknown-pragmas"
open_solution "baseline"
set_part {xc7vx485t-ffg1157-1}
create_clock -period 10 -name default
#source "./cordiccart2pol/baseline/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
