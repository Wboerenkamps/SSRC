----------------------------------------------------------------------------------
--!
--! @filename   bcd_counter.vhd
--! @brief      File holds the entity bcd_counter
--!
--! @details    bcd_counter is used to implement a one-digit BCD counter. These
--!             counters are ideal to use to output a counter value to a 7-segment
--!             display, for example, because you need to drive one digit (in the
--!             range of 0 to 9) at a time.
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
--! @brief     bcd_counter is used to implement a one-digit BCD counter
--!
--! bcd_counter is used to implement a one-digit BCD counter that is capable of
--! counting both up and down, and having a carry input/output in order to be able
--! to chain multiple one-digit counters into a multi-digit BCD counter.
----------------------------------------------------------------------------------
entity bcd_counter is
  --generic (
  --);
  port (
    RESET                       : in  std_logic;                                      --! Active high reset signal to reset seven_seg_mux
    SYS_CLK_100MHZ              : in  std_logic;                                      --! System clock running at 100 MHz
    -- Input signals to increment/decrement the counter
    INCR_IN                     : in  std_logic;
    DECR_IN                     : in  std_logic;
    -- Output signals to carry/decrement the next digit, and to output the current value.
    CNT_OUT                     : out std_logic_vector(3 downto 0);                   --! Counter output for this digit
    CARRY_OUT                   : out std_logic;                                      --! Carry output (counter overflow and reset)
    DECR_OUT                    : out std_logic                                       --! Decrement output (counter underflow and reset to 9)
  );
end bcd_counter;

----------------------------------------------------------------------------------
-- Architecture
----------------------------------------------------------------------------------
architecture behavioral of bcd_counter is

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
signal cnt_out_i              : unsigned(3 downto 0) := (others => '0');

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
  --! @brief    sub_cnt_mux_proc is used to implement a "subsampling" counter
  --!
  --! This process is used to implement a counter that is used to reduce the frequency
  --! at which the switching between the different 7-segments occurs.
  ----------------------------------------------------------------------------------
  sub_cnt_mux_proc: process(SYS_CLK_100MHZ) is
  begin
    if rising_edge(SYS_CLK_100MHZ) then
      -- Carry/decrement signals are active for only one clock-cycle.
      CARRY_OUT         <= '0';
      DECR_OUT          <= '0';
      if RESET = '1' then
        cnt_out_i       <= (others => '0');
      -- When increment input is active, and no decrement needs to happen at the same time,
      -- increment, and wrap to 0 and assert the Carry output when the counter is at 9, and
      -- another increment is requested.
      elsif (INCR_IN = '1') and (DECR_IN = '0') then
        if (cnt_out_i = 9)  then
          cnt_out_i     <= (others => '0');
          CARRY_OUT     <= '1';
        else
          cnt_out_i     <= cnt_out_i +1;
        end if;
      -- When the decrement input is active, and no increment needs to happen at the same time,
      -- decrement, and wrap to 9 and assert the Decrement output when the counter is at 0, and
      -- another decrement is requested.
      elsif (INCR_IN = '0') and (DECR_IN = '1') then
        if (cnt_out_i = 0) then
          DECR_OUT      <= '1';
          cnt_out_i     <= to_unsigned(9, cnt_out_i'length);
        else
          cnt_out_i     <= cnt_out_i -1;
        end if;
      -- Don't do anything when both increment and decrement inputs are active at the same time
      end if;
    end if;
  end process;

  CNT_OUT               <= std_logic_vector(cnt_out_i);

end behavioral;