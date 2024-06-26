# -------------------------------------------------------------------
# -- Clock
# -------------------------------------------------------------------
set_property PACKAGE_PIN W5 [get_ports SYS_CLK_100MHZ]
set_property IOSTANDARD LVCMOS33 [get_ports SYS_CLK_100MHZ]
create_clock -period 10.000 -name sys_clk_100mhz [get_ports SYS_CLK_100MHZ]
# -------------------------------------------------------------------
# -- Push buttons
# -------------------------------------------------------------------
set_property IOSTANDARD LVCMOS33 [get_ports RESET]
set_property PACKAGE_PIN U18 [get_ports RESET]
set_property IOSTANDARD LVCMOS33 [get_ports UP_BTN]
set_property PACKAGE_PIN T18 [get_ports UP_BTN]
set_property IOSTANDARD LVCMOS33 [get_ports DOWN_BTN]
set_property PACKAGE_PIN U17 [get_ports DOWN_BTN]
# -------------------------------------------------------------------
# -- 7-Segment signals
# -------------------------------------------------------------------
set_property IOSTANDARD LVCMOS33 [get_ports AN*]
set_property PACKAGE_PIN U2 [get_ports AN3]
set_property PACKAGE_PIN U4 [get_ports AN2]
set_property PACKAGE_PIN V4 [get_ports AN1]
set_property PACKAGE_PIN W4 [get_ports AN0]
set_property IOSTANDARD LVCMOS33 [get_ports C*]
set_property PACKAGE_PIN W7 [get_ports CA]
set_property PACKAGE_PIN W6 [get_ports CB]
set_property PACKAGE_PIN U8 [get_ports CC]
set_property PACKAGE_PIN V8 [get_ports CD]
set_property PACKAGE_PIN U5 [get_ports CE]
set_property PACKAGE_PIN V5 [get_ports CF]
set_property PACKAGE_PIN U7 [get_ports CG]
set_property IOSTANDARD LVCMOS33 [get_ports DP]
set_property PACKAGE_PIN V7 [get_ports DP]

#set_property BITSTREAM.ENCRYPTION.ENCRYPT YES [current_design]
#set_property BITSTREAM.ENCRYPTION.ENCRYPTKEYSELECT BBRAM [current_design]
#set_property BITSTREAM.ENCRYPTION.KEY0 256'hbf815623218c7a8ac127d3d0a2852e9e02ffe0297657de4b68dbd5d4fe51f24c [current_design]
#set_property BITSTREAM.ENCRYPTION.HKEY 256'3d5723fa0e8e03de4eb1b065d25bc70c9fb05d227d368a27955677f08e6f6286[current_design]
#set_property BITSTREAM.ENCRYPTION.STARTCBC 128'h11d5dabe1cf80833420ecccc5d6476d1[current_design]