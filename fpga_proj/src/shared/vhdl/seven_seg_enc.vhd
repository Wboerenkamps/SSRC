----------------------------------------------------------------------------------
--!
--! @filename   seven_seg_enc.vhd
--! @brief      File holds the entity seven_seg_enc
--!
--! @details    seven_seg_enc is used to encode one single 7-segment element. A complete
--!             "display" can consist of multiple elements. These elements will
--!             then need to be muxed/connected outside of this encoder.
--!
--! @copyright  (c) Core|Vision B.V.,
--!             Cereslaan 10b,
--!             5384 VT Heesch,
--!             The Netherlands.
--!             All Rights Reserved.
--!
--! @customer   Avans 's Hertogenbosch
--! @project    Security workshop
--! @module     seven_seg_cntr
--!
--! @author     Jurgen Gerritsen
--! @svnversion $Date$
--!             $Revision$
--!
----------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use ieee.numeric_std.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library unisim;
--use unisim.vcomponents.all;

library work;
use work.seven_seg_cntr_pkg.all;


----------------------------------------------------------------------------------
--! @brief     seven_seg_drv is used to drive one 7-segment element.
--!
--! seven_seg_drv receives a one digit decimal input, and makes sure the proper 7-segment
--! elements are driven to display this decimal. It also has an input to be able
--! to drive the dot element.
----------------------------------------------------------------------------------
entity seven_seg_enc is
  --generic (
  --);
  port (
    SYS_CLK_100MHZ              : in  std_logic;                                      --! System clock running at 100 MHz
    -- Input signals that have to be written to the 7-segment display
    INT_IN                      : in  std_logic_vector(3 downto 0);                   --! Unsigned integer input value that has to be displayed
    DOT_IN                      : in  std_logic;
    -- 7-Segment output signals
    CAT_OUT                     : out std_logic_vector(VECTOR_SIZE-1 downto 0);       --! Cathode outputs
    DP_OUT                      : out std_logic
  );
end seven_seg_enc;

----------------------------------------------------------------------------------
-- Architecture
----------------------------------------------------------------------------------
architecture behavioral of seven_seg_enc is

----------------------------------------------------------------------------------
-- Constants
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Type definitions
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Component declarations
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Signal declarations
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Functions
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Procedures
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Attributes
----------------------------------------------------------------------------------

----------------------------------------------------------------------------------
-- Functionality
----------------------------------------------------------------------------------
begin
  ----------------------------------------------------------------------------------
  --! @brief    seven_seg_enc_proc is used to register the input integer to the 7-segment output.
  --!
  --! This process is used to register the input integer into the desired 7-segment output.
  ----------------------------------------------------------------------------------
  seven_seg_enc_proc : process(SYS_CLK_100MHZ)
  begin
    if rising_edge(SYS_CLK_100MHZ) then
      -- Check if the integer input is within bounds of the expected integer.
      if (to_integer(unsigned(INT_IN)) >= 0) and (to_integer(unsigned(INT_IN)) <= 9) then
        CAT_OUT <= SEG_OUT_ENC(to_integer(unsigned(INT_IN)));
      -- If not, write the invalid pattern
      else
        CAT_OUT <= SEG_OUT_INVALID;
      end if;
      -- Register the dot input.
      DP_OUT <= DOT_IN;
    end if;
  end process;

end behavioral;