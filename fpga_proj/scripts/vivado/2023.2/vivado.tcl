##--------------------------------------------------------------------------------
# @file     build.tcl
# @brief    File holds the TCL script to regenerate the complete Vivado project
#
# This file is used to regenerate the complete Vivado project from its sources.
# Please call this file from build.bat script.
#
# @author   Jurgen Gerritsen
# $Date: 2022-10-12 13:31:46 +0200 (Wed, 12 Oct 2022) $
# $Revision: 53374 $
#
# Copyright Core|Vision. All rights Reserved.
# The use of this file, whole or a part, is subjected to licensing terms.
# --------------------------------------------------------------------------------

# --------------------------------------------------------------------------------
# Get project parameters from batch file
# --------------------------------------------------------------------------------
if {$argc != 6} {
  puts "This script requires 6 arguments to be input."
  puts "These arguments are organized as follows:"
  puts "Argument 0: Project name"
  puts "Argument 1: Board name"
  puts "Argument 2: Synthesis, should the design be synthesized automatically (y = yes, n = no)"
  puts "Argument 3: Implementation, should the design be implemented automatically (y = yes, n = no)"
  puts "Argument 4: Bitstream, should the design bitstream be generated automatically (y = yes, n = no)"
  puts "Argument 5: Start_gui, should the script start the GUI when done (y = yes, n = no)"
  puts "For example, vivado -mode batch -source \"./build.tcl\" -notrace -tclargs seven_seg_cntr basys3 y y y y"
  return
} else {
  set proj_name [lindex $argv 0]
  set board_name [lindex $argv 1]
  set synth [lindex $argv 2]
  set impl [lindex $argv 3]
  set bitstream [lindex $argv 4]
  set start_gui_when_done [lindex $argv 5]
}
# --------------------------------------------------------------------------------


# --------------------------------------------------------------------------------
# Set default settings
# --------------------------------------------------------------------------------
set scripts_dir [file dirname [file normalize [info script]]]

set origin_dir [file normalize $scripts_dir/../../../]
set working_directory "$origin_dir/GEN_$board_name"

puts "INFO: origin_dir directory is $origin_dir"
puts "INFO: working_directory is $working_directory"
puts "INFO: board_name is $board_name"

set hdl_path "$origin_dir/src/shared/vhdl"
set constraints_path "$origin_dir/src/$board_name/constr"

set target_constraint_filename "$constraints_path/seven_seg_cntr.xdc"


# --------------------------------------------------------------------------------
# overload default settings with target build settings
# --------------------------------------------------------------------------------

source "$origin_dir/src/$board_name/build_settings.tcl"

# --------------------------------------------------------------------------------
# Start
# --------------------------------------------------------------------------------


# Check the Vivado version
set current_vivado_version [version -short]
if { [string first $required_vivado_version $current_vivado_version] == -1 } {
	puts ""
	puts "WARNING: The required/tested Vivado version is <$required_vivado_version>, the current Vivado version is <$current_vivado_version>"
}


file mkdir "$origin_dir/GEN_$board_name"

cd $working_directory
puts "INFO: Switched to working_directory $working_directory"

# Create project
create_project -part $proj_part $proj_name "$working_directory"

# --------------------------------------------------------------------------------
# Set project properties
# --------------------------------------------------------------------------------
set obj [get_projects $proj_name]
set_property "default_lib" "xil_defaultlib" $obj
set_property "simulator_language" "Mixed" $obj
set_property "target_language" "VHDL" $obj

# set add board if defined
if { [info exists proj_board] } {
	set_property "board_part" $proj_board $obj
}

# --------------------------------------------------------------------------------
# Create 'sources_1' fileset (if not found)
# --------------------------------------------------------------------------------
if {[string equal [get_filesets -quiet sources_1] ""]} {
  create_fileset -srcset sources_1}

# --------------------------------------------------------------------------------
# Create 'sim_1' fileset (if not found)
# --------------------------------------------------------------------------------
if {[string equal [get_filesets -quiet sim_1] ""]} {
  create_fileset -simset sim_1
}

# Get all HDL sources and sort them in RTL and testbench sources
set vhdl_files [glob -nocomplain -type {f} -directory "$hdl_path" "*.vhd"]
# Create empty lists to prevent error on non-existent variables
set testbench_sources [list]
set rtl_sources [list]
if {$vhdl_files != {} } {
  puts "INFO: VHDL files found:"
  foreach file $vhdl_files {
    if {[string match -nocase "*_tb.vhd" $file] || [string match -nocase "*_pkg_tb.vhd" $file] || [string match -nocase "*_tb_pkg.vhd" $file]} {
      lappend testbench_sources $file
    } else {
      lappend rtl_sources $file
    }
  }

  # Add all VHDL RTL sources to project
  set obj [get_filesets sources_1]
  puts "INFO: VHDL RTL sources:"
  foreach file $rtl_sources {
    puts $file
    add_files -norecurse -fileset $obj $file
    set file_obj [get_files -of_objects [get_filesets sources_1] [list "*$file"]]
    set_property "file_type" "VHDL" $file_obj
  }

  # Add all VHDL testbench sources to project
  set obj [get_filesets sim_1]
  puts "INFO: VHDL testbench sources:"
  foreach file $testbench_sources {
    puts $file
    add_files -norecurse -fileset $obj $file
    set file_obj [get_files -of_objects [get_filesets sim_1] [list "*$file"]]
    set_property "file_type" "VHDL" $file_obj
  }
} else {
  puts "WARNING: No VHDL RTL files found!"
}

# --------------------------------------------------------------------------------
# Add/Import constraint files and set constraint file properties
# --------------------------------------------------------------------------------
# Create 'constrs_1' fileset (if not found)
if {[string equal [get_filesets -quiet constrs_1] ""]} {
  create_fileset -constrset constrs_1
}

# Set 'constrs_1' fileset object
set obj [get_filesets constrs_1]

set xdc_files [glob -nocomplain -type {f} -directory "$constraints_path" "*.xdc"]
if {$xdc_files != {} } {
  puts "INFO: XDC files found:"
  foreach file $xdc_files {
    puts "INFO: $file"
    add_files -norecurse -fileset $obj $file

    set file_obj [get_files -of_objects [get_filesets constrs_1] [list "*$file"]]
    set_property "file_type" "XDC" $file_obj
  }

  # set target constraints file
  if {[file exists $target_constraint_filename]} {
    # Set 'constrs_1' fileset properties
    set obj [get_filesets constrs_1]
    set_property "target_constrs_file" "$target_constraint_filename" $obj
  } else {
    puts "WARNING: target constraint file not found! File has to be named $target_constraint_filename"
  }
} else {
  puts "WARNING: No XDC constraint files found!"
}

# --------------------------------------------------------------------------------
# Create 'synth_1' run (if not found)
# --------------------------------------------------------------------------------
set synth_flow "Vivado Synthesis [string range [version -short] 0 3]"

if {[string equal [get_runs -quiet synth_1] ""]} {
  create_run -name synth_1 -part $proj_part -flow $synth_flow -strategy "Vivado Synthesis Defaults" -constrset constrs_1
} else {
  set_property strategy "Vivado Synthesis Defaults" [get_runs synth_1]
  set_property flow $synth_flow [get_runs synth_1]
}
set obj [get_runs synth_1]
# Do not flatten the hierarchy (easier debugging because signal names remain intact)
set_property STEPS.SYNTH_DESIGN.ARGS.FLATTEN_HIERARCHY none [get_runs synth_1]

# set the current synth run
current_run -synthesis [get_runs synth_1]

# --------------------------------------------------------------------------------
# Create 'impl_1' run (if not found)
# --------------------------------------------------------------------------------
set impl_flow "Vivado Implementation [string range [version -short] 0 3]"

if {[string equal [get_runs -quiet impl_1] ""]} {
#  create_run -name impl_1 -part $proj_part -flow $impl_flow -strategy "Vivado Implementation Defaults" -constrset constrs_1 -parent_run synth_1
  create_run -name impl_1 -part $proj_part -flow $impl_flow -strategy "Performance_ExplorePostRoutePhysOpt" -constrset constrs_1 -parent_run synth_1
} else {
#  set_property strategy "Vivado Implementation Defaults" [get_runs impl_1]
  set_property strategy "Performance_ExplorePostRoutePhysOpt" [get_runs impl_1]
  set_property flow $impl_flow [get_runs impl_1]
}
set obj [get_runs impl_1]

# set the current impl run
current_run -implementation [get_runs impl_1]

puts "INFO: Project created: $proj_name"

# Set the bitfile name
set bitfilename "$proj_name.bit"
set mcsfilename "$proj_name.mcs"

# --------------------------------------------------------------------------------
# Synthesize design
# --------------------------------------------------------------------------------
if {$synth == y || $impl == y || $bitstream == y} then {
  puts "INFO: Synthesizing design"
  launch_runs synth_1
  wait_on_run synth_1
}
# --------------------------------------------------------------------------------


# --------------------------------------------------------------------------------
# Implement design
# --------------------------------------------------------------------------------
if {$impl == y || $bitstream == y } then {
	puts "INFO: Implementing design"
	launch_runs impl_1

	wait_on_run impl_1
	open_run impl_1
	puts "INFO: Write utilization report"
	report_utilization -hierarchical -hierarchical_depth 6 -file "$working_directory/utilization_hierarchical_depth6.txt"

}
# --------------------------------------------------------------------------------

# --------------------------------------------------------------------------------
# Generate bitstream and utilization log
# --------------------------------------------------------------------------------
if {$bitstream == y} then {
	puts "Writing the bitfile to the GEN_$board_name folder"
	write_bitstream -force $working_directory/${proj_name}.runs/impl_1/$bitfilename
  write_cfgmem  -format mcs -size 4 -interface SPIx1 -loadbit [list up 0x00000000 $working_directory/${proj_name}.runs/impl_1/$bitfilename] -file $working_directory/${proj_name}.runs/impl_1/$mcsfilename
}

# --------------------------------------------------------------------------------

# --------------------------------------------------------------------------------
# Start the GUI after creating the project
# --------------------------------------------------------------------------------
if {$start_gui_when_done == y} then {
  start_gui
  # report the timing summary when design was implemented
  if {$impl == y || $bitstream == y} then {
    report_timing_summary -delay_type min_max -report_unconstrained -check_timing_verbose -max_paths 10 -input_pins -name timing_1
  }
}
# --------------------------------------------------------------------------------
